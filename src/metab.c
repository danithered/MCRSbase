#include <mainheader.h>
#include <stdlib.h>
#include <math.h>
#include <randomgen.h>

double metabolizmus(int *matrix_f, int *met_szomszedsag_f, int method_f, int szomsz_cellaszam_f, int enzimaktszam_f, double reciprocEnzimaktszam_f, int cella_f) {
//	printf("\nenzimaktszam=%d", enzimaktszam_f);
	int szomszed_f = 0, nezett_f=0, enzakt_f = 0;
	double *enzimsum_f, metab_f=1;
	
	/* szomszed_f: a target (cella_f) szomszedjanak sorszama/ hanyadik szomszedot nezzuk eppen
	 * nezett_f: hanyadik cellat nezzuk eppen
	 * enzakt_f: a nezett enzakt szamlaloja
	 * enzimsum_f: az adott enzimaktivitasok osszege
	 * metab_f: M, azaz a metabolizmus erteke az adott cellara
	 */
	
	enzimsum_f=(double*) calloc(enzimaktszam_f, sizeof(double));
	
	//Conunting enzyme activity
//	printf("\ncella: %d (szerk, spec, eakt)\n", cella_f);
	for(szomszed_f=0; szomszed_f < szomsz_cellaszam_f; szomszed_f++) {
		nezett_f = *(met_szomszedsag_f+cella_f*szomsz_cellaszam_f+szomszed_f);
		if ( (*(matrix_f+nezett_f)) > 0 ) {
		  //*(enzimsum_f+(*(matrix_f+nezett_f)).k) = *(enzim_f+nezett_f*enzimaktszam_f+(*(matrix_f+nezett_f)).k);
		  *(enzimsum_f + *(matrix_f+nezett_f) - 1)++ ;
//  		printf("\nnezett=%d\tdominans=%d", nezett_f, dominans_f);
//  		printf(" (%d, %d, %g)", (*(matrix_f+nezett_f)).szerk, (*(matrix_f+nezett_f)).spec, *(enzim_f+nezett_f*enzimaktszam_f+dominans_f));
        }
	}
	
    //Check if there is a 0 amongst activities
    for(enzakt_f=0; enzakt_f<enzimaktszam_f; enzakt_f++){
		if( *(enzimsum_f+enzakt_f) <= 0 ) return(0);
    }
	
	//Metabolic function - there is no zero!
	switch(method_f)
		case 1:
			metab_f =1;
			for(enzakt_f=0; enzakt_f<enzimaktszam_f; enzakt_f++){
				metab_f *= (double) *(enzimsum_f+enzakt_f);
//			printf("\nenzimsum= %g", *(enzimsum_f+enzakt_f));
			}
//			printf("\n\nmetab_f=%g\t1/enzimaktszam_f=%g", metab_f, (1/ (double) enzimaktszam_f));
//			printf("\nmetab_f=%g, reciprocEnzimaktszam_f=%g", metab_f, reciprocEnzimaktszam_f);
	
			metab_f=pow(metab_f, reciprocEnzimaktszam_f);
	
			break;
		case 2:
			metab_f = minimum(enzimsum_f, enzimaktszam_f);
			break;
		case 3:
			metab_f = 0;
			for(enzakt_f=0; enzakt_f<enzimaktszam_f; enzakt_f++){
				metab_f += 1 / ((double) *(enzimsum_f+enzakt_f));
			}
			metab_f = 1 / metab_f;
			break;
	
	free (enzimsum_f);
//	printf("\nvegso metab_f=%g", metab_f);
	return (metab_f);
}

