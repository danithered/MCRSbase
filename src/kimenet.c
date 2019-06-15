#include <stdlib.h>
#include <stdio.h>

void fajlbaMatrix(int *matrix_f, int ncol_f, int nrow_f, FILE *out_f) {
	int cellaszam_f, col_f, row_f;
	
	for(row_f=0; row_f<nrow_f; row_f++) {
		for(col_f=0; col_f<ncol_f; col_f++) {
            fprintf(out_f, "%d\t", *(matrix_f+(row_f*ncol_f+col_f)));
		}
		fprintf(out_f, "\n");
	}	
		
	return ;
}
