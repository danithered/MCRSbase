#include <mainheader.h>
#include <math.h>

void konzolraMatrix(int *matrix_f, int ncol_f, int nrow_f) {
	int cellaszam_f, col_f, row_f;
	
	for(row_f=0; row_f<nrow_f; row_f++) {
		for(col_f=0; col_f<ncol_f; col_f++) {
			printf("%d\t", *(matrix_f+(row_f*ncol_f+col_f)));
		}
		printf("\n");
	}	
		
	return ;
}

void konzolraMatrixD(double *matrix_f, int ncol_f, int nrow_f) {
	int cellaszam_f, col_f, row_f;
	
	for(row_f=0; row_f<nrow_f; row_f++) {
		for(col_f=0; col_f<ncol_f; col_f++) {
			printf("%g\t", *(matrix_f+(row_f*ncol_f+col_f)));
		}
		printf("\n");
	}	
		
	return ;
}
