#include <stdlib.h>
#include <stdio.h>

void fajlbaMatrix(int *matrix_f, int ncol_f, int nrow_f, FILE *out_f) {
	int col_f, row_f;
	
	for(row_f=0; row_f<nrow_f; row_f++) {
		for(col_f=0; col_f<ncol_f; col_f++) {
            fprintf(out_f, "%d\t", *(matrix_f+(row_f*ncol_f+col_f)));
		}
		fprintf(out_f, "\n");
	}	
		
	return ;
}

int atlagadatok(int *matrix_f, int meret_f, int noEa_f, int ciklus_f, FILE *fajl_f) {
    /* matrix_f: alapmatrix
     * meret_f: az alapmatrix merete
     * noEa_f: hany fajta enzimaktivitas van
     * fajl_f: kimenet fajlba pointer
     * 
     * szamlalo_f: hany replikator van adott tipusbol. Poziciok:
     *  -1: parazita
     *  0: ures cella
     *  1: elso enzimaktivitas
     *  stb.
     * cella_f: ciklusszamlalo, hanyadik cella adatat nezzuk; kesobb: szamlalo_f vegignez
     */
    
    int *szamlalo_f, cella_f = 0, osszesen = 0;
    
    szamlalo_f = (int *) calloc(noEa_f, sizeof(int));
    szamlalo_f++;
    
    //szamlalas
    for(cella_f = 0; cella_f < meret_f; cella_f++) {
        (*(szamlalo_f + *(matrix_f + cella_f)) )++;
    }
    
    //kiiras
    fprintf(fajl_f, "%d", ciklus_f);
    for(ciklus_f = -1; ciklus_f <= noEa_f; ciklus_f++) {
        fprintf(fajl_f, " %d", *(szamlalo_f + ciklus_f) );
        if(ciklus_f) osszesen += *(szamlalo_f + ciklus_f);
    }
    fprintf(fajl_f, "\n");
    
    //felszabadit es visszateres
    free(--szamlalo_f);
    return(osszesen);
}
