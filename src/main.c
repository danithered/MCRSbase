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
	int argnum=4 + NOEA;
	if (argc < argnum) {
		printf("tul keves argomentum: %d szukseges\n", argnum);
		return(3);
	}
	
	int ncol=atoi(argv[1]);
    double met_neigh_meret = atof(argv[2]), repl_neigh_meret = atof(argv[3]);
	char azon[20] = "\0";
    strcpy(azon, argv[4 + NOEA]);
    
    
    /* ncol: alapmatrix oszlopainak szama
     * met_neigh_meret: metabolikus szomszedsag merete
     * repl_neigh_meret: metabolikus szomszedsag merete
     * azon: egyedi azonosito
	 */
	printf("Argomentumok sikeresen beolvasva:\n%d %f %f %s\n", ncol, met_neigh_meret, repl_neigh_meret, azon);
    
    
	//Valtozok deklaralasa
	int meret=ncol*ncol, ciklus=0, iter=0, cella=0, met_neigh_cellaszam=0, repl_neigh_cellaszam=0;
	char mappa[30]="OUT/\0", mentesmappa[30]="save\0", fajlnev[50]="\0", kezdet[30]="\0", csvname[50]={0}, cellafajlnev[50]="\0", savetoR[50]="\0", savetoData[50]="\0", savetoE[50]="\0";
	

	/*
	 * meret= ncol*nrow, az alapmatrix cellaszama
	 * ciklus: hanyadik iteracios ciklusnal jarunk (egy cikluson belul meret szamu iteracios lepes van)
	 * iter: egy cikluson belul hanyadik iteracios lepesnel tartunk
	 * cella: az epp nezett cella szama
	 * met_neigh_cellaszam: hany cellabol all a metabolikus szomszedsag
	 * repl_neigh_cellaszam: hany cellabol all a replikacios szomszedsag
	 * */
	
	//Pointerek deklaralasa
	int *matrix, *met_neigh, *repl_neigh, *dif_neigh;
    double *claimek, *adatok, *inicEA;
	FILE *output, *fp, *cellak, *rngSave, *dataSave, *enzSave;
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
    inicEA = (double *) calloc(NOEA, sizeof(double));
	
	if(!matrix ||!claimek) {
            printf("nem lehet lefoglalni egy matrix-ot(main)\n");
            return(1);
	}

	
	//szovegmuveletek
	strcat(mappa, azon);
	sprintf(mentesmappa, "%s/%s", mappa, "save");
	sprintf(fajlnev, "%s/%s.data\0", mappa, azon);
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
	
	output = fopen(fajlnev, "a");
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
//  konzolraMatrix(matrix, ncol, ncol);
    fprintf(output, "");
    fajlbaMatrix(matrix, ncol, ncol, output);
	

/*	
	//Iteracio
	for(ciklus=0; ciklus<ciklusszam && replikator_num; ciklus++) {
		diffuzio_szam=0;
		//REPLIKACIOS LEPES
		for(iter=0; iter<meret; iter++) {
			cella=gsl_rng_uniform_int(r, meret);
			if ((*(matrix+cella)).k != 0) { //ha van benne vmi
				if(gsl_rng_uniform(r) < phalal) extinkcio(matrix, enzim, enzakt_num, cella); //meghal
			}
			else { //ha nincs benne semmi
				*(claimek+0)=claimEmpty;
				claimSum=claimEmpty;
				for (num_repl_neigh=1; num_repl_neigh<repl_neigh_cellaszam; num_repl_neigh++) {
					nezett= *(repl_neigh+cella*repl_neigh_cellaszam+num_repl_neigh);
					knezett=((*(matrix+nezett)).k);
					if (knezett) {
						*(claimek+num_repl_neigh) = (metabolizmus(matrix, enzim, met_neigh, met_neigh_cellaszam, enzakt_num, reciprocEnzakt_num, nezett) * knezett);
						claimSum+= *(claimek+num_repl_neigh);
						//*(claimek+num_repl_neigh);
						//metabolizmus(matrix, enzim, met_neigh, met_neigh_cellaszam, enzakt_num, 13);
						//double xx= metabolizmus(matrix, enzim, met_neigh, met_neigh_cellaszam, enzakt_num, 13);
//						printf("\n%g", *(claimek+num_repl_neigh));
//						printf("\nreciprocenzakt_num=%g", reciprocEnzakt_num);
					}
					else *(claimek+num_repl_neigh) = 0;
				}
//				printf("\nClaimSum= %g", claimSum);
//				printf("\nnezett cella: %d", cella);
				valaszto= gsl_rng_uniform_pos(r);
				templat=-2; //ha ilyet latsz, akkor nagy baj van...
				claimJelolt=0;
				for (num_repl_neigh=0; num_repl_neigh<repl_neigh_cellaszam; num_repl_neigh++) {
					if(*(claimek+num_repl_neigh)) {claimJelolt += *(claimek+num_repl_neigh)/claimSum;}
//					printf("\nclaim jelolt (%d): %g", (*(matrix+*(repl_neigh+cella*repl_neigh_cellaszam+num_repl_neigh))).szerk, claimJelolt);
					if (claimJelolt > valaszto) {
					  if (num_repl_neigh==0) templat=-1; //ha ures marad
					  else templat=*(repl_neigh+cella*repl_neigh_cellaszam+num_repl_neigh);
					  break;
					}
				}
//				printf("\ntemplat: %d\tclaimJelolt=%g\tvalaszto=%g", templat, claimJelolt, valaszto);
				
				//REPLIKACIO!!!!//
				if(templat > -1) { //ha van masolando templat
					if(replikacio(matrix, enzim, enzakt_num, spec_limit, templat, cella)) {
						printf("gaz van a replikacional");
						return (2); 
					}
//					printf("\n\nreplikacio: %d -> %d", templat, cella);
					//MUTACIO!!!//
					if (pMutacio > gsl_rng_uniform_pos(r)) {
						MUTACIO (matrix, enzim, enzakt_num, sd, cella, emax, tradeoffEE, reciprocTradeoffEE, tradeoffEK, reciprocTradeoffEK, kmax, kmin, spec_limit);
					}
				}
			}
			//DIFFUZIO
			for(diffuzio_szam+=diffuzioGyak; diffuzio_szam>=1; diffuzio_szam--) {
				cella=gsl_rng_uniform_int(r, meret);
//				konzolraMatrixStruct(matrix, meret);
				diffTM (matrix, enzim, enzakt_num, ncol, cella);
//				printf("\n\n");
//				konzolraMatrixStruct(matrix, meret);
			}
		}
//		fclose(fp);
		//tranzicio
// 		//for(cella= 0; cella<meret; cella++) transition1(matrix, enzim, enzakt_num, cella);
		
		//mintavetel
		if (mintavetel_gyak) {
			if ((ciklus%mintavetel_gyak)==0) {
				//fprintf(output, "\nciklusszam:;%d\n", ciklus+1);
				tipusadatok(adatok, matrix, enzim, enzakt_num, meret, enztipus_num, sorhossz);
				replikator_num=0;
				for (enzakt=0; enzakt<enzakt_num; enzakt++) replikator_num= *(adatok+ enzakt*sorhossz);
				if(!replikator_num) printf("\na rendszer meghalt (%d .ciklus)\n", ciklus);
				kimenetTipusadat (output, matrix, adatok, enztipus_num, sorhossz, ciklus+1);
			}
		}
		if (cellamintavetel_gyak && ((ciklus%cellamintavetel_gyak)==0)) {
			if(ment==4) writetoBin(matrix, enzim, save, meret, ciklus+1, dataSave, dataSave);
			else fajlbaCella(cellak, matrix, enzim, enzakt_num, meret, ciklus+1);
		}
//		konzolraCella(matrix, enzim, enzakt_num, meret);
//		konzolraMatrixD(enzim, enzakt_num, meret);
//		printf("\n");
	}
*/	
	
//	konzolraMatrixStruct(matrix, meret);
// 	printf("\nenzimakt:\n");
//	konzolraMatrixD(enzim, enzakt_num, meret);
		
//  	double xxx=metabolizmus(matrix, enzim, met_neigh, met_neigh_cellaszam, enzakt_num, 13);
// 	printf("\n\n\nM=%g\n", xxx);
	
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
	//free(adatok);
    free(inicEA);
	printf("Matrixok felszabaditva\n");

	fclose(output);
	fclose(fp);
	fclose(cellak);
	fclose(rngSave);
	fclose(dataSave);
    printf("Fajlok lezarva\n");

		
	//randomszam generator lezarasa
	gsl_rng_free(r);
	
	return(0);
}
