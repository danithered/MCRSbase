#include <mainheader.h>
#include <stdlib.h>
#include <randomgen.h>

int diffTM (int *matrix_f, int *diffmatrix_f, int target_f) {
  /* Toffuli-Marghulis szerinti elforgatas: random elforgat egy negyzetet 90 fokkal
   * 
   * matrix_f: alapmatrix
   * diffmatrix_f:  a diffuzios szomszedsagok matrixa
   * numcol_f:az alapmatrix oszlopainak szama 
   * target_f: az elforgatott negyzet bal felso cellajanak szama
   */
	int save_f=0;
		
	
  /* save_f: az elso cella ertekeit menti el
   */
	
	//1. pozicio lementese
	save_f = (*(matrix_f + target_f));

	//Random irany
	if ((gsl_rng_uniform(r) < 0.5)) {
		//poziciok csereje
		*(matrix_f + target_f) = *(matrix_f + *(diffmatrix_f + target_f*4 + 1)); // 0 <- 1 
        *(matrix_f + *(diffmatrix_f + target_f*4 + 1) ) = *(matrix_f + *(diffmatrix_f + target_f*4 + 3)); // 1 <- 3
        *(matrix_f + *(diffmatrix_f + target_f*4 + 3)) = *(matrix_f + *(diffmatrix_f + target_f*4 + 2)); // 3 <- 2
        *(matrix_f + *(diffmatrix_f + target_f*4 + 2)) = save_f; // 2 <- eredeti 0
	} else {
        //poziciok csereje
		*(matrix_f + target_f) = *(matrix_f + *(diffmatrix_f + target_f*4 + 2)); // 0 <- 2 
        *(matrix_f + *(diffmatrix_f + target_f*4 + 2) ) = *(matrix_f + *(diffmatrix_f + target_f*4 + 3)); // 2 <- 3
        *(matrix_f + *(diffmatrix_f + target_f*4 + 3)) = *(matrix_f + *(diffmatrix_f + target_f*4 + 1)); // 3 <- 1
        *(matrix_f + *(diffmatrix_f + target_f*4 + 1)) = save_f; // 1 <- eredeti 0
    }
	return(0);
}
