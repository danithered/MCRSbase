/*
MCRS alapmodell fitness funkcio tesztelesre
egyed alapu (nem szekvencia alapu) replikatorok

Argomentumok: 21 (ncol nrow enzakt_num met_neigh_meret repl_neigh_meret ciklusszam mintavetel_gyak cellamintavetel_gyak alapfeltoltes emax tradeoffEE tradeoffEK kmin kmax phalal claimEmpty sd spec_limit diffuzioGyak pMutacio ment) Lsd.:Argomentumok beolvasasa resz
test: 10 10 2 0 0 10 2 2 0.8 1 0.2 0.4 2 4 0.1 0.001 0.1 0.01 0.01 0

main visszateresi ertekek:
    0: lefutott
    1: matrix lefoglalasi hiba
    2: replikacio hiba
    3: tul keves argomentum
    4: ilyen nevu fajl mar van -> futtasd ujra
*/

#include <mainheader.h>
#include <stdlib.h>
#include <randomgen.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>

#define NOEA 3



int main(int argc, char *argv[]) {
	//randomszam generator inicializalasa
	r = (gsl_rng *) gsl_rng_alloc (gsl_rng_mt19937);
	gsl_rng_set(r, time(&timer));
	/*gsl_rng * r = gsl_rng_alloc (gsl_rng_mt19937);
	gsl_rng_set(r, time(&timer));*/
    
	//ido kiirasa
	char idoki[30] = "\0";
	struct tm *ido;
	ido = localtime( &timer );
	sprintf(idoki, "%02d.%02d_%02d.%02d.%02d\0", (*ido).tm_mon, (*ido).tm_mday, (*ido).tm_hour, (*ido).tm_min, (*ido).tm_sec);
	printf("\nProgram futasa elkezdodott:\n%s\n", idoki);
    
	//Argomentumok beolvasasa
	int argnum = 11 + NOEA;
	if (argc < argnum) {
		printf("tul keves argomentum: %d szukseges\n", argnum);
		return(3);
	}
	
	int ncol=atoi(argv[1]), ciklusszam = atoi(argv[2]), mintavetel_gyak = atoi(argv[8]), matrixkiiratas_gyak = atoi(argv[9]), modszer = atoi(argv[10]);
	double met_neigh_meret = atof(argv[3]), repl_neigh_meret = atof(argv[4]), phalal = atof(argv[5]), claimEmpty = atof(argv[6]), diffuzioGyak = atof(argv[7]);
	char azon[20] = "\0";
	strcpy(azon, argv[11 + NOEA]);
    
    
	/* ncol: alapmatrix oszlopainak szama
	* ciklusszam: milyen sokaig fut a program
	* met_neigh_meret: metabolikus szomszedsag merete
	* repl_neigh_meret: metabolikus szomszedsag merete
	* phalal: extinkcio valsege
	* claimEmpty: az uresen maradas claim-ja
	* diffuzioGyak: milyen gyakran kovetekzik be diff esemeny
	* mintavetel_gyak: milyen gyakran irjon ki atlagadatokat: 0 soha, 1 minden generacioban, 2 minden 2. generacioban
	* matrixkiiratas_gyak: milyen gyakran irja ki a teljes matrixot
	* modszer: melyik fuggvennyel szamitsa ki a metabolizmus hatekonysagat
	* 	1: klasszikus, mertani atlag
	* 	2: minimum (by Gergo)
	* 	3: reciproc osszegek reciproca (by Sz Andras)
	* ... EA adatok ...
	* azon: egyedi azonosito
	*/
	
	printf("Argomentumok sikeresen beolvasva:\n%d %d %f %f %f %f %f %d %d %d %s\n", ncol, ciklusszam, met_neigh_meret, repl_neigh_meret, phalal, claimEmpty, diffuzioGyak, mintavetel_gyak, matrixkiiratas_gyak, modszer, azon);
    
    
	//Valtozok deklaralasa
	int meret=ncol*ncol, ciklus=0, iter=0, cella=0, met_neigh_cellaszam=0, repl_neigh_cellaszam=0, replikatornum=1, num_repl_neigh=0, nezett=0, sikeres=0, diffuzio_szam=0;
	double reciprocEnzakt_num = (double) 1/(double)NOEA ;
	char mappa[30]="OUT/\0", mentesmappa[30]="save\0", fajlnev[50]="\0", mfajlnev[50]="\0", kezdet[30]="\0", csvname[50]={0}, cellafajlnev[50]="\0", savetoR[50]="\0", savetoData[50]="\0", savetoE[50]="\0";
	
	double knezett=1;
	

	/*
	 * meret= ncol*nrow, az alapmatrix cellaszama
	 * ciklus: hanyadik iteracios ciklusnal jarunk (egy cikluson belul meret szamu iteracios lepes van)
	 * iter: egy cikluson belul hanyadik iteracios lepesnel tartunk
	 * cella: az epp nezett cella szama
	 * met_neigh_cellaszam: hany cellabol all a metabolikus szomszedsag
	 * repl_neigh_cellaszam: hany cellabol all a replikacios szomszedsag
	 * replikatornum: ??
	 * num_repl_neigh: ciklusszamlalo, vegigmegy a replikacios szomszedsagon
	 * nezett: hanyadik cellat nezzuk
	 * sikeres: kinek sikerult replikalodnia. Ha 0, akkor senkinek (claimEmpty), ha nagyobb, akkor annak a szomszednak
	 * diffuzio_szam: diffuziohoz szamlalo. Ha >0, akkor diffuzio tortenik.
	 * reciprocEnzakt_num: az enzimaktivitasok szamanak reciproka
	 * fajlnev: atlagadatok ebbe
	 * mfajlnev: ebbe mennek matrixok
	 */
	
	//Pointerek deklaralasa
	int *matrix, *met_neigh, *repl_neigh, *dif_neigh;
	double *claimek, *adatok, *inicEA;
	FILE *output, *fp, *moutput, *rngSave;
	struct stat st = {0}, stCsv = {0};


	/*
	* matrix: alapmatrix, az van benne milyen replikato tipus:
	*   0: ures
	*   1-NOEA: milyen aktivitasu
	*   -1: parazita
	* dif_neigh: diffuzios szomszedsag
	* met_neigh: metabolikus szomszédság mátrix
	* repl_neigh: replikacios szomszédság mátrix
	*/


	//valtozok kiszamitasa
	met_neigh_cellaszam = szomsz_meret(met_neigh_meret);
	repl_neigh_cellaszam = szomsz_meret(repl_neigh_meret);
	
	//matrixok lefoglalasa
	matrix = (int *) calloc(meret, sizeof(int));
	claimek=(double *) calloc((repl_neigh_cellaszam+1), sizeof(double));
	*(claimek) = claimEmpty;
	inicEA = (double *) calloc(NOEA, sizeof(double));
	
	if(!matrix ||!claimek) {
            printf("nem lehet lefoglalni egy matrix-ot(main)\n");
            return(1);
	}

	
	//szovegmuveletek
	strcat(mappa, azon);
	sprintf(mentesmappa, "%s/%s", mappa, "save");
	sprintf(fajlnev, "%s/%s.data\0", mappa, azon);
	sprintf(mfajlnev, "%s/%s_matrix.data\0", mappa, azon);
	sprintf(savetoR, "%s/saveR%s.bin\0", mentesmappa, azon);
	
	//log file kezd
	if (stat(mappa, &st) == -1) mkdir(mappa, 7777);
	if (stat(mentesmappa, &st) == -1) mkdir(mentesmappa, 7777);
	
	//fp = freopen("OUT/log", "a", stdout);
	
	//Kimenet fajlok nyitasa
	if (stat(fajlnev, &stCsv) != -1) {
		printf("\n%s ilyen nevu fajl mar van!\n", fajlnev);
		return(4);
	}
	if (stat(mfajlnev, &stCsv) != -1) {
		printf("\n%s ilyen nevu fajl mar van!\n", mfajlnev);
		return(4);
	}
	
	output = fopen(fajlnev, "a");
	moutput = fopen(mfajlnev, "a");
	rngSave = fopen(savetoR, "a");
	
	
	/******************************
	******** A LENYEG *************
	*******************************/
	
	//Matrixok feltoltese
	printf("Starting to inicialise matrix with probabilities:\n");
	for(ciklus=0; ciklus < NOEA; ciklus++){
		*(inicEA + ciklus) = atof(argv[argnum - NOEA + ciklus]) ;
		printf(" %f", *(inicEA + ciklus));
	}
	inicM(matrix, inicEA, NOEA, meret);
	printf("\nInicialisation closed\n");
    
	//szomszedsag matrixok feltoltese
	met_neigh= (int *) metNeighInic(meret, ncol, met_neigh_meret);
	repl_neigh= (int *) metNeighInic(meret, ncol, repl_neigh_meret);
	dif_neigh = (int *) difNeighInic(meret, ncol);
//	konzolraMatrix(met_neigh, met_neigh_cellaszam, meret);
//	konzolraMatrix(dif_neigh, 4, meret);
	
	
	//Kimenet
//  	konzolraMatrix(matrix, ncol, ncol);
	fprintf(output, "ncol ciklusszam met_neigh_meret repl_neigh_meret phalal claimEmpty diffuzioGyak mintavetel_gyak matrixkiiratas_gyak modszer");
	fprintf(moutput, "ncol ciklusszam met_neigh_meret repl_neigh_meret phalal claimEmpty diffuzioGyak mintavetel_gyak matrixkiiratas_gyak modszer");
	for(iter=0; iter < NOEA; iter++) {
		fprintf(output, " inicEA%d", iter+1);
		fprintf(moutput, " inicEA%d", iter+1);
	}
	fprintf(output, "\n%d %d %g %g %g %g %g %d %d %d", ncol, ciklusszam, met_neigh_meret, repl_neigh_meret, phalal, claimEmpty, diffuzioGyak, mintavetel_gyak, matrixkiiratas_gyak, modszer);
	fprintf(moutput, "\n%d %d %g %g %g %g %g %d %d %d", ncol, ciklusszam, met_neigh_meret, repl_neigh_meret, phalal, claimEmpty, diffuzioGyak, mintavetel_gyak, matrixkiiratas_gyak, modszer);
	for(iter=0; iter < NOEA; iter++) {
		fprintf(output, " %g", *(inicEA+iter) );
		fprintf(moutput, " %g", *(inicEA+iter) );
	}
	
	fprintf(output, "\n");
	fprintf(moutput, "\n#0\n");
	
	replikatornum = atlagadatok(matrix, meret, NOEA, 0, output);
	
	fajlbaMatrix(matrix, ncol, ncol, moutput);
	
	
	//Iteracio
	for(ciklus=1; ciklus <= ciklusszam && replikatornum; ciklus++) {
		diffuzio_szam=0;
		//REPLIKACIOS LEPES
		for(iter=0; iter < meret; iter++) {
			cella=gsl_rng_uniform_int(r, meret);
			if (*(matrix+cella) != 0) { //ha van benne vmi
				if(gsl_rng_uniform(r) < phalal) *(matrix+cella) = 0; //meghal
			}
			else { //ha nincs benne semmi
				for (num_repl_neigh=1; num_repl_neigh<repl_neigh_cellaszam; num_repl_neigh++) {
					nezett= *(repl_neigh+cella*repl_neigh_cellaszam+num_repl_neigh);
					if (*(matrix+nezett) > 0) { //ha van benne metabolikus replikator
						
						*(claimek+num_repl_neigh) = (metabolizmus(matrix, met_neigh, modszer, met_neigh_cellaszam, NOEA, reciprocEnzakt_num, nezett) * knezett) + *(claimek + num_repl_neigh - 1); // A kumulalt ertekeket szamitja ki!
						
//						printf("\n%g", *(claimek+num_repl_neigh));
//						printf("\nreciprocenzakt_num=%g", reciprocEnzakt_num);
					}
					else *(claimek+num_repl_neigh) = *(claimek + num_repl_neigh - 1);
				}
//				printf("\nClaimSum= %g", claimSum);
//				printf("\nnezett cella: %d", cella);
				//valaszto= gsl_rng_uniform_pos(r);
				
				sikeres = torottpalca(claimek, repl_neigh_cellaszam, gsl_rng_uniform_pos(r));
				if( sikeres ) {
					*(matrix + cella) = *(matrix + *(repl_neigh + cella*repl_neigh_cellaszam + sikeres));
				}
				
//				printf("\ntemplat: %d\ttipusa: %d, random= %g", *(repl_neigh + cella*repl_neigh_cellaszam + sikeres), *(matrix + *(repl_neigh + cella*repl_neigh_cellaszam + sikeres)), valaszto);
				
			}
			//DIFFUZIO
			for(diffuzio_szam += diffuzioGyak; diffuzio_szam>=1; diffuzio_szam--) {
//				printf("diffuzio in ciklus %d, iter %d\n", ciklus, iter);
//              konzolraMatrix(matrix, ncol, ncol);
                diffTM (matrix, dif_neigh, gsl_rng_uniform_int(r, meret));
//				konzolraMatrix(matrix, ncol, ncol);
			}
		}
		//fclose(fp);
		
		//mintavetel
		if (mintavetel_gyak && ((ciklus%mintavetel_gyak)==0)) {
//			fprintf(output, "\nciklusszam:;%d\n", ciklus+1);
			replikatornum = atlagadatok(matrix, meret, NOEA, ciklus, output);
			if(!replikatornum) printf("\na rendszer meghalt (%d .ciklus)\n", ciklus);
		}
		if (matrixkiiratas_gyak && ((ciklus%matrixkiiratas_gyak)==0)) {
			fprintf(moutput, "\n#%d\n", cella);
			fajlbaMatrix(matrix, ncol, ncol, moutput);
		}
//		konzolraMatrixD(matrix, ncol, ncol);
//		printf("\n");
	}
	
	//mintavetel - ha nem volt utolso ciklusban
	if (mintavetel_gyak && ((ciklus%mintavetel_gyak) != 0)) {
		replikatornum = atlagadatok(matrix, meret, NOEA, ciklus, output);
		if(!replikatornum) printf("\na rendszer meghalt (%d .ciklus)\n", ciklus);
	}
	if (matrixkiiratas_gyak && ((ciklus%matrixkiiratas_gyak) != 0)) {
		fprintf(moutput, "\n#%d\n", cella);
		fajlbaMatrix(matrix, ncol, ncol, moutput);
	}
	
	//MENTES
	gsl_rng_fwrite (rngSave, r);
		
	time(&timer);
	printf("\nSimulation %s stopped at %s\n", azon, ctime( &timer ));
	
	/******************************
	****A LENYEG VEGE****
	*******************************/
	//matrixok felszabaditasa
	free(matrix);
	free(met_neigh);
	free(repl_neigh);
	free(dif_neigh);
	free(claimek);
	free(inicEA);
	printf("Matrixok felszabaditva\n");

	fclose(output);
	fclose(fp);
	fclose(moutput);
	fclose(rngSave);
	printf("Fajlok lezarva\n");

		
	//randomszam generator lezarasa
	gsl_rng_free(r);
	
	return(0);
}
