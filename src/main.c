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
	int argnum = 11 + 2*(NOEA+1), argszamlalo;
	char *token, *read2, *readfree, read[512]="\0";
	FILE *infile, *rngSeed;
	if (argc < argnum) {
		printf("tul keves argomentum: %d szukseges\n", argnum);
		return(3);
	}
	
	if(argc == (argnum+2) ) {
		// rng seed beolvasasa fajlbol
		printf("RNG seed beolvasasa %s fajlbol\n", argv[argnum + 1]);
		rngSeed = fopen(argv[argnum + 1], "rb");
		if(!infile) printf("%s fajl beolvasasa nem sikerult!\n", argv[argnum + 1]);
		if( !betoltesRng(rngSeed) ) {
			printf("RNG Seed betoltese sikerult\n");
		}
		fclose(rngSeed);
	}	
	
	if(argc > (argnum+2) ) {
		//argomentumok atirasa
		printf("megkezdett szimulacio folytatasa\nArgomentumok beolvasasa fajlbol: %s\nGSL random seed beolvasasa fajlbol: %s", argv[argnum + 1], argv[argnum + 2]);
		infile = fopen(argv[argnum + 1], "r");
		if(!infile) printf("%s fajl beolvasasa nem sikerult!\n", argv[argnum + 1]);
		fgets(read, 256, infile);
		fgets(read, 256, infile);
		readfree = read2 = strdup(read);
		for(argszamlalo=1; ((token = strsep(&read2, " "))); argszamlalo++ ) {
			strcpy(argv[argszamlalo], token);
		}
		free(readfree);
		printf("Argomentumok beolvasasa sikerult.\nRNG Seed beolvasasa folyamatban...\n");
		
		//rng seed
		rngSeed = fopen(argv[argnum + 2], "rb");
		if(!infile) printf("%s fajl beolvasasa nem sikerult!\n", argv[argnum + 2]);
		if( !betoltesRng(rngSeed) ) {
			printf("RNG Seed betoltese sikerult\n");
		}
		fclose(rngSeed);
	}
	
	int ncol=atoi(argv[1]), ciklusszam = atoi(argv[2]), mintavetel_gyak = atoi(argv[8]), matrixkiiratas_gyak = atoi(argv[9]), modszer = atoi(argv[10]);
	double met_neigh_meret = atof(argv[3]), repl_neigh_meret = atof(argv[4]), phalal = atof(argv[5]), claimEmpty = atof(argv[6]), diffuzioGyak = atof(argv[7]);
	char azon[20] = "\0";
	strcpy(azon, argv[11 + 2*(NOEA+1)]);
    
    
	/* 1: ncol: alapmatrix oszlopainak szama
	* 2: ciklusszam: milyen sokaig fut a program
	* 3: met_neigh_meret: metabolikus szomszedsag merete
	* 4: repl_neigh_meret: metabolikus szomszedsag merete
	* 5: phalal: extinkcio valsege
	* 6: claimEmpty: az uresen maradas claim-ja
	* 7: diffuzioGyak: milyen gyakran kovetekzik be diff esemeny
	* 8: mintavetel_gyak: milyen gyakran irjon ki atlagadatokat: 0 soha, 1 minden generacioban, 2 minden 2. generacioban
	* 9: matrixkiiratas_gyak: milyen gyakran irja ki a teljes matrixot
	* 10: modszer: melyik fuggvennyel szamitsa ki a metabolizmus hatekonysagat
	* 	1: klasszikus, mertani atlag
	* 	2: minimum (by Gergo)
	* 	3: reciproc osszegek reciproca (by Sz Andras)
	* 
	* ... EA adatok ...
	* 	iniciacios gyakorisag
	* 	k
	* 
	* azon: egyedi azonosito
	*/
	
	printf("Argomentumok sikeresen beolvasva:\n%d %d %f %f %f %f %f %d %d %d %s\n", ncol, ciklusszam, met_neigh_meret, repl_neigh_meret, phalal, claimEmpty, diffuzioGyak, mintavetel_gyak, matrixkiiratas_gyak, modszer, azon);
    
    
	//Valtozok deklaralasa
	int meret=ncol*ncol, ciklus=0, iter=0, cella=0, met_neigh_cellaszam=0, repl_neigh_cellaszam=0, replikatornum=1, num_repl_neigh=0, nezett=0, sikeres=0;
	double reciprocEnzakt_num = (double) 1/(double)NOEA , diffuzio_szam=0.0;
	char mappa[30]="OUT/\0", mentesmappa[30]="save\0", fajlnev[50]="\0", mfajlnev[50]="\0", kezdet[30]="\0", csvname[50]={0}, cellafajlnev[50]="\0", savetoR[50]="\0", savetoData[50]="\0", savetoE[50]="\0";
	

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
	double *claimek, *adatok, *inicEA, *kvalues;
	FILE *output, *moutput;
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
/**/	printf("met_neigh_cellaszam: %d, repl_neigh_cellaszam: %d\n", met_neigh_cellaszam, repl_neigh_cellaszam);
	
	//matrixok lefoglalasa
	matrix = (int *) calloc(meret, sizeof(int));
	claimek=(double *) calloc((repl_neigh_cellaszam+1), sizeof(double));
	*(claimek) = claimEmpty;
	inicEA = (double *) calloc(NOEA+1, sizeof(double));
	kvalues = (double *) calloc(NOEA+1, sizeof(double));
	
	if(!matrix ||!claimek) {
            printf("nem lehet lefoglalni egy matrix-ot(main)\n");
            return(1);
	}

	
	//szovegmuveletek
	strcat(mappa, azon);
	sprintf(mentesmappa, "%s/%s", mappa, "save");
	sprintf(fajlnev, "%s/%s.data\0", mappa, azon);
	sprintf(mfajlnev, "%s/%s_matrix.data\0", mappa, azon);
	
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
	
	
	/******************************
	******** A LENYEG *************
	*******************************/
	
	//Matrixok feltoltese
	printf("Starting to inicialise matrix with probabilities (and k values):\n");
	for(ciklus=0; ciklus <= NOEA; ciklus++){
		*(inicEA + ciklus) = atof(argv[argnum - 2*(NOEA+1) + ciklus]) ;
		printf(" %f", *(inicEA + ciklus));
		*(kvalues + ciklus) = atof(argv[argnum - NOEA - 1 + ciklus]) ;
		printf(" (%f)", *(kvalues + ciklus));
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
	for(iter=0; iter <= NOEA; iter++) {
		fprintf(output, " inicEA%d", iter?iter:-1 );
		fprintf(moutput, " inicEA%d", iter?iter:-1 );
	}
	for(iter=0; iter <= NOEA; iter++) {
		fprintf(output, " kvalues%d", iter?iter:-1 );
		fprintf(moutput, " kvalues%d", iter?iter:-1 );
	}
	fprintf(output, " azon\n%d %d %g %g %g %g %g %d %d %d", ncol, ciklusszam, met_neigh_meret, repl_neigh_meret, phalal, claimEmpty, diffuzioGyak, mintavetel_gyak, matrixkiiratas_gyak, modszer);
	fprintf(moutput, " azon\n%d %d %g %g %g %g %g %d %d %d", ncol, ciklusszam, met_neigh_meret, repl_neigh_meret, phalal, claimEmpty, diffuzioGyak, mintavetel_gyak, matrixkiiratas_gyak, modszer);
	for(iter=0; iter <= NOEA; iter++) {
		fprintf(output, " %g", *(inicEA+iter) );
		fprintf(moutput, " %g", *(inicEA+iter) );
	}
	for(iter=0; iter <= NOEA; iter++) {
		fprintf(output, " %g", *(kvalues+iter) );
		fprintf(moutput, " %g", *(kvalues+iter) );
	}
	
	fprintf(output, " %s\n", azon);
	fprintf(moutput, " %s\n#0\n", azon);
	
	replikatornum = atlagadatok(matrix, meret, NOEA, 0, output);
	
	fajlbaMatrix(matrix, ncol, ncol, moutput); //elso allapot elmentese
	
	//Rng seed mentese
	sprintf(savetoR, "%s/saveR_%s_%d.bin\0", mentesmappa, azon, 0);
	mentesRng(savetoR);
	
	
	//egyeb kezdes elotti teendok
	//kvalues--; //elobbre leptettem (nulladik pozicio innentol az elso), hogy kesobb a kvalues+matrix ertekeknel ne kelljen mindig "-1"-et beirni
	
	//Iteracio
	for(ciklus=1; ciklus <= ciklusszam && replikatornum; ciklus++) {
//		printf("\nROUND %d\n", ciklus);
		diffuzio_szam=0;
		//REPLIKACIOS LEPES
		for(iter=0; iter < meret; iter++) {
			cella=gsl_rng_uniform_int(r, meret);
			if (*(matrix+cella) != 0) { //ha van benne vmi
				if(gsl_rng_uniform(r) < phalal) {
					*(matrix+cella) = 0; //meghal
//					printf("Cell death: %d\n", cella);
				}
//				else {printf("NO cell death: %d\n", cella);}
			}
			else { //ha nincs benne semmi
//				printf("Replication to position %d (%d):\n", cella, *(matrix + cella));
				for (num_repl_neigh=1; num_repl_neigh<repl_neigh_cellaszam; num_repl_neigh++) {
					nezett= *(repl_neigh+cella*repl_neigh_cellaszam+num_repl_neigh);
					if (*(matrix+nezett) ) { //ha van benne replikator
//						printf("");
						*(claimek+num_repl_neigh) = (metabolizmus(matrix, met_neigh, modszer, met_neigh_cellaszam, NOEA, reciprocEnzakt_num, nezett) * ((*(matrix+nezett) > 0)?(*(kvalues + *(matrix+nezett))):(*kvalues) )) + *(claimek + num_repl_neigh - 1); // A kumulalt ertekeket szamitja ki!
						
//						printf("\tcell %d Claim(%d)= %g, k= %g, M= %g\n", nezett, num_repl_neigh, *(claimek+num_repl_neigh), (*(matrix+nezett) > 0)?(*(kvalues + *(matrix+nezett))):(*kvalues), metabolizmus(matrix, met_neigh, modszer, met_neigh_cellaszam, NOEA, reciprocEnzakt_num, nezett));
//						printf("\n%g", *(claimek+num_repl_neigh));
//						printf("\nreciprocenzakt_num=%g", reciprocEnzakt_num);
					}
					else {
						*(claimek+num_repl_neigh) = *(claimek + num_repl_neigh - 1);
//						printf("\tcell %d: no replicator here, claim: %g\n", nezett, *(claimek+num_repl_neigh));
					}
				}
//				printf("\nClaimSum= %g", claimSum);
//				printf("\nnezett cella: %d", cella);
				//valaszto= gsl_rng_uniform_pos(r);
				
//				printf("repl_neigh_cellaszam: %d\n", repl_neigh_cellaszam);
				sikeres = torottpalca(claimek, repl_neigh_cellaszam, gsl_rng_uniform_pos(r));
				if( sikeres ) {
					*(matrix + cella) = *(matrix + *(repl_neigh + cella*repl_neigh_cellaszam + sikeres));
//					printf("succesful replication of replicator type %d from position %d to %d\n", *(matrix + *(repl_neigh + cella*repl_neigh_cellaszam + sikeres)), *(repl_neigh + cella*repl_neigh_cellaszam + sikeres), cella);
				}
				
//				printf("\ntemplat: %d\ttipusa: %d, random= %g", *(repl_neigh + cella*repl_neigh_cellaszam + sikeres), *(matrix + *(repl_neigh + cella*repl_neigh_cellaszam + sikeres)), valaszto);
				
			}
			//DIFFUZIO
//			printf("ciklus %d, iter %d, diffuzio_szam: %g\n", ciklus, iter, diffuzio_szam);
			for(diffuzio_szam += diffuzioGyak; diffuzio_szam>=1; diffuzio_szam--) {
//				printf("diffuzio in ciklus %d, iter %d\n", ciklus, iter);
//				konzolraMatrix(matrix, ncol, ncol);
				diffTM (matrix, dif_neigh, gsl_rng_uniform_int(r, meret));
//				konzolraMatrix(matrix, ncol, ncol);
			}
		} //iteracios lepes bezar
		//fclose(fp);
		
		//mintavetel
		if (mintavetel_gyak && ((ciklus%mintavetel_gyak)==0)) {
//			fprintf(output, "\nciklusszam:;%d\n", ciklus+1);
			replikatornum = atlagadatok(matrix, meret, NOEA, ciklus, output);
			if(!replikatornum) printf("\na rendszer meghalt (%d .ciklus)\n", ciklus);
			fflush(output);
		}
		if (matrixkiiratas_gyak && ((ciklus%matrixkiiratas_gyak)==0)) {
			fprintf(moutput, "\n#%d\n", ciklus);
			fajlbaMatrix(matrix, ncol, ncol, moutput);
			sprintf(savetoR, "%s/saveR%s_%d.bin\0", mentesmappa, azon, ciklus);
			mentesRng(savetoR);
			fflush(moutput);
		}
//		konzolraMatrixD(matrix, ncol, ncol);
//		printf("\n");
	} //ciklus bezar
	
	//mintavetel - ha nem volt utolso ciklusban
	if (mintavetel_gyak && ((ciklus%mintavetel_gyak) != 0)) {
		replikatornum = atlagadatok(matrix, meret, NOEA, ciklus, output);
		if(!replikatornum) printf("\na rendszer meghalt (%d .ciklus)\n", ciklus);
	}
	if (matrixkiiratas_gyak && ((ciklus%matrixkiiratas_gyak) != 0)) {
		fprintf(moutput, "\n#%d\n", ciklus);
		fajlbaMatrix(matrix, ncol, ncol, moutput);
		sprintf(savetoR, "%s/saveR%s_%d.bin\0", mentesmappa, azon, ciklus);
		mentesRng(savetoR);
	}
	
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
	free(kvalues);
	printf("Matrixok felszabaditva\n");

	fclose(output);
	//fclose(fp);
	fclose(moutput);
	printf("Fajlok lezarva\n");

		
	//randomszam generator lezarasa
	gsl_rng_free(r);
	
	return(0);
}
