#include <mainheader.h>
#include <stdlib.h>
#include <math.h>

int Rmod(int a, int b) {
	int r = a%b; 
	return r >= 0 ? r : r + b;
}

unsigned int countNoNeigh(const double neigh_tipus){
	if(neigh_tipus < 0.0) {
		fprintf(stderr, "countNoNeigh: a szomszedsagi tavolsag nem lehet negativ!\n");
		return (0);
	}
	if(neigh_tipus == 0.0) {
		return (0);
	}
	unsigned int no=0; 
    
	//other cells
	int maxDist = (int) sqrt(neigh_tipus)+1;
	for(int x = -maxDist; x <= maxDist; x++) for(int y = -maxDist; y <= maxDist; y++){ 
		if( x*x + y*y <= neigh_tipus ) {
			no++;
		}
	} //end for x and y
	return(no);
}

void blueprintNeigh(const double neigh_tipus, int* n_inic_x, int* n_inic_y){
	int index=0;
	//self
	n_inic_x[index] = 0;
	n_inic_y[index++] = 0;
    
	//other cells
	int maxDist = (int) sqrt(neigh_tipus)+1;
	for(int x = -maxDist; x <= maxDist; x++) for(int y = -maxDist; y <= maxDist; y++){ 
		if( (x || y) && (x*x + y*y <= neigh_tipus ) ) {
			n_inic_x[index] = x;
			n_inic_y[index++] = y;
		}
	} //end for x and y
}

int calcNeigh(const int i, const unsigned int n, int* n_inic_x, int* n_inic_y, const int ncol) {
	return( Rmod( ((int)i/ncol + n_inic_y[n]) , ncol) * ncol + 
		Rmod( Rmod(i , ncol) + n_inic_x[n] , ncol));
}

/** initialises the neighbour matrix for each cell, according to the given neighbourhood distance
 * @param neigh_vec: the neighbour matrix to be filled (already allocated)
 * @param size: the number of cells in the grid
 * @param neigh_tipus: the neighbourhood distance
 * */
void neighInic(int* neigh_vec, const int size, const double neigh_tipus, const int ncol) {
	int no_neigh = countNoNeigh(neigh_tipus);
	int* n_inic_x = (int*) calloc(no_neigh, sizeof(int));
	int* n_inic_y = (int*) calloc(no_neigh, sizeof(int));
	if(!n_inic_x || !n_inic_y) {
		fprintf(stderr, "neighInic: nem sikerult lefoglalni a szomszedsag matrixot!\n");
		return;
	}

	blueprintNeigh(neigh_tipus, n_inic_x, n_inic_y);

	for(int i=0; i < size; i++){ //iterate throught grid
		//assign neighbours
		for(unsigned int n = 0; n < no_neigh; ++n) {	
			int myneigh = calcNeigh(i, n, n_inic_x, n_inic_y, ncol);
			if( myneigh > -1 ) {
				neigh_vec[i*no_neigh + n] = myneigh;
			}
		}
	}
} //end neighInic


/// Original Functions

//feltolti a szomszedsagmatrixot: sajat pozicio, N, W, E, S
int * metNeighInic(int cellaszam_f, int ncol_f, double neigh_tipus_f) {
	/*
	 * cellaszam_f: az alapmatrix cellaszama
	 * neigh_tipus_f: a szomsz. merete: ha 0 -> von Neumann; ha több -> Moore szomsz
	 * */
	
	//Valtozok deklaralasa
	int metNeigh_matrixmeret_f=0, alapMCellaszamlalo_f=0, colszamlalo_f=0, rowszamlalo_f=0, vonN_f = 0, szomsz_f=0, szomszedszam_f=0;
	static int *matrix_f;
	/*
	 * metNeigh_matrixmeret_f: a szomsz. matrix cellaszama
	 * alapMCellaszamlalo_f: szamlalo: hanyadik alapmatrix cellanal jarunk a feltoltesben
	 * ncol_f: alapmatrix oszlopainak szama =sqrt(cellaszam_f)
	 * colszamlalo_f: hanyadik oszlopban tartunk
	 * rowszamlalo_f: hanyadik sorban tartunk
	 * szomsz_f: egy cella szomszedsagaba hany cella tartozik pl.: 2es szomsz eseten (2*2+1)^2=25
	 * *matrix_f: a szomsz. matrix
	 * */
	
	
	szomsz_f = szomsz_meret(neigh_tipus_f);
//	printf("neigh_tipus_f: %g -> szomsz_f: %d\n", neigh_tipus_f, szomsz_f);
	if(neigh_tipus_f==0 || ( neigh_tipus_f - (int)neigh_tipus_f )){
		vonN_f = 1;
		neigh_tipus_f = (double)(int)neigh_tipus_f;
	}
		
	metNeigh_matrixmeret_f=cellaszam_f* szomsz_f; //szomsz. matrix cellameretenek meghatarozasa
//	printf("cellaszam: %d, szomsz: %d, metNeigh_matrixmeret: %d\n", cellaszam_f, szomsz_f, metNeigh_matrixmeret_f);
	matrix_f=(int *) calloc(metNeigh_matrixmeret_f, sizeof(int)); //szomsz. matrix lefoglalasa
	if(!matrix_f) printf("metNeighInic: nem sikerult lefoglalni a matrixot!\n");
	for(alapMCellaszamlalo_f=0; alapMCellaszamlalo_f<cellaszam_f; alapMCellaszamlalo_f++) {
		szomszedszam_f=0;
		//*(matrix_f+ alapMCellaszamlalo_f*szomsz_f+szomszedszam_f)=alapMCellaszamlalo_f;
		
		for(rowszamlalo_f = 0 - neigh_tipus_f; rowszamlalo_f <= 0 + neigh_tipus_f; rowszamlalo_f++) {
			for(colszamlalo_f = 0 - neigh_tipus_f; colszamlalo_f <= 0 + neigh_tipus_f; colszamlalo_f++) {
				//szomszedszam_f++;
				if(!colszamlalo_f && !rowszamlalo_f) *(matrix_f+ alapMCellaszamlalo_f*szomsz_f)=torus(cellaszam_f, alapMCellaszamlalo_f+rowszamlalo_f*ncol_f+colszamlalo_f);
				else {
					*(matrix_f+ alapMCellaszamlalo_f*szomsz_f+szomszedszam_f+1)=torus(cellaszam_f, alapMCellaszamlalo_f+rowszamlalo_f*ncol_f+colszamlalo_f);
//					printf("%d ", *(matrix_f+ alapMCellaszamlalo_f*szomsz_f+szomszedszam_f+1));
					szomszedszam_f++;
				}
			}
		}
		if(vonN_f) {
			*(matrix_f + alapMCellaszamlalo_f*szomsz_f + szomszedszam_f + 1) = torus(cellaszam_f, alapMCellaszamlalo_f - (1 + neigh_tipus_f)*ncol_f); //1: N
			*(matrix_f + alapMCellaszamlalo_f*szomsz_f + szomszedszam_f + 2) = torus(cellaszam_f, alapMCellaszamlalo_f - (1 + neigh_tipus_f)); //2: W
			*(matrix_f + alapMCellaszamlalo_f*szomsz_f + szomszedszam_f + 3) = torus(cellaszam_f, alapMCellaszamlalo_f + (1 + neigh_tipus_f)); //3: E
			*(matrix_f + alapMCellaszamlalo_f*szomsz_f + szomszedszam_f + 4) = torus(cellaszam_f, alapMCellaszamlalo_f + (1 + neigh_tipus_f)*ncol_f); //4: S
		}
	}
	
//	konzolraMatrix(matrix_f, 25, 90000);
	
	return (matrix_f);
}

int szomsz_meret(double tipus_f) {
	if(tipus_f < 1) return(5); //vonNeumann
	else {
		if(tipus_f - (int)tipus_f ) return(szomsz_meret((double)(int)tipus_f) + 4);
		else return (pow(((int)tipus_f*2+1), 2)); //Moore szomszedsag: pl.: 2es szomsz eseten (2*2+1)^2=25 
	}
}

int* difNeighInic(int meret_f, int ncol_f){
    int *dif_f, focal_f = 0;
    
    dif_f = (int*) calloc(meret_f*4, sizeof(int));
    if(dif_f) {
        for(focal_f = 0; focal_f < meret_f; focal_f++) {
            *(dif_f + focal_f*4) = focal_f;
            *(dif_f + focal_f*4 + 1) = torus(meret_f, focal_f + 1); //jobbra
            *(dif_f + focal_f*4 + 2) = torus(meret_f, focal_f + ncol_f); //lent
            *(dif_f + focal_f*4 + 3) = torus(meret_f, focal_f + ncol_f + 1); //jobbra le
        }
    }
    else {
        printf("\n\nNEM sikerult feltolteni diffuzio matrixot!\n");
    }
    
    return(dif_f);
}


