#include <mainheader.h>
#include <stdlib.h>
#include <math.h>
#include <randomgen.h>

double metabolizmusO(int *matrix_f, int *met_szomszedsag_f, int szomsz_cellaszam_f, int enzimaktszam_f, double reciprocEnzimaktszam_f, int cella_f) {
//	printf("\nenzimaktszam=%d", enzimaktszam_f);
	int szomszed_f, nezett_f=0, enzakt_f, dominans_f;
	double *enzimsum_f, metab_f=1;
	
	/* szomszed_f: a target (cella_f) szomszedjanak sorszama/ hanyadik szomszedot nezzuk eppen
	 * nezett_f: hanyadik cellat nezzuk eppen
	 * enzakt_f: a nezett enzakt szamlaloja
	 * enzimsum_f: az adott enzimaktivitasok osszege
	 * metab_f: M, azaz a metabolizmus erteke az adott cellara
	 */
	
	enzimsum_f=(double*) calloc(enzimaktszam_f, sizeof(double));
	
//	printf("\ncella: %d (szerk, spec, eakt)\n", cella_f);
	for(szomszed_f=0; szomszed_f < szomsz_cellaszam_f; szomszed_f++) {
		nezett_f=*(met_szomszedsag_f+cella_f*szomsz_cellaszam_f+szomszed_f);
		if (! (*(matrix_f+nezett_f)) ) continue;
		//*(enzimsum_f+(*(matrix_f+nezett_f)).k) = *(enzim_f+nezett_f*enzimaktszam_f+(*(matrix_f+nezett_f)).k);
		*(enzimsum_f+dominans_f) += *(enzim_f+nezett_f*enzimaktszam_f+dominans_f);
//		printf("\nnezett=%d\tdominans=%d", nezett_f, dominans_f);
//		printf(" (%d, %d, %g)", (*(matrix_f+nezett_f)).szerk, (*(matrix_f+nezett_f)).spec, *(enzim_f+nezett_f*enzimaktszam_f+dominans_f));
	}
	for(enzakt_f=0; enzakt_f<enzimaktszam_f; enzakt_f++){
		metab_f *= *(enzimsum_f+enzakt_f);
//		printf("\nenzimsum= %g", *(enzimsum_f+enzakt_f));
	}
//	printf("\n\nmetab_f=%g\t1/enzimaktszam_f=%g", metab_f, (1/ (double) enzimaktszam_f));
//	printf("\nmetab_f=%g, reciprocEnzimaktszam_f=%g", metab_f, reciprocEnzimaktszam_f);
	
	if (metab_f>0) metab_f=pow(metab_f, reciprocEnzimaktszam_f);
	
	free (enzimsum_f);
//	printf("\nvegso metab_f=%g", metab_f);
	return (metab_f);
}

