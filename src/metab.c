#include <mainheader.h>
#include <stdlib.h>
#include <math.h>
#include <randomgen.h>

extern double* fitness_map;

unsigned long long binom(unsigned long long n, unsigned long long k) {
    if (k > n) return 0;

    if (k > n - k) { // symmetry
        k = n - k;
    }

    unsigned long long result = 1;

    for (unsigned long long i = 1; i <= k; i++) {
        result *= ((n - k + i) / i);
    }

    return result;
}

unsigned long long getNo(unsigned int sumea, unsigned int noEA){
  if(sumea == 0) return (1);
  if(noEA == 1) return(1);
  return(binom(sumea + noEA - 1, noEA - 1));
}

unsigned long long getIndex(unsigned long *eas, unsigned long noEA){
  unsigned long long sumea = 0;
  for(int i=0; i<noEA; i++){
    sumea += eas[i];
  }

  if(sumea == 0) return (0); 
  
  unsigned long long index = 0;
  
  // forward by sum number of acts
  if(sumea > 0){
    for(unsigned long i = 0; i < sumea; ++i ){
      index += getNo(i, noEA); 
    }
  }
  
  // calculate index based on position of individual acts
  unsigned long long ea_left = sumea;
  unsigned long long pos_left = noEA;
  for(unsigned long *ea = eas, *ea_end = eas + noEA; ea != ea_end; ++ea){
    --pos_left;
    // add positions of eas before
    if(*ea > 0){
      if(pos_left != 0){
        for(unsigned long long n = 0; n < *ea; ++n){
          index += getNo(ea_left-n, pos_left);
        }
      } 
      ea_left -= *ea;
      if(ea_left == 0) return(index);
    }
  }
  return(index);
}

double randomMet(unsigned long *eas, int noEA){
  unsigned long long sumea = 0;
  for(int i=0; i<noEA; i++){
    sumea += eas[i];
  }

  if(sumea == 0) return(0);
  return (gsl_rng_uniform(r) * 2);
}

void generate(double *map, int pos, int remaining, unsigned long *eas, int X) {
    if (pos == X - 1) {
        eas[pos] = remaining;
        map[getIndex(eas, X)] = randomMet(eas, X);
        return;
    }

    for (int v = 0; v <= remaining; v++) {
        eas[pos] = v;
        generate(map, pos + 1, remaining - v, eas, X);
    }
}

void generate_all(double* map, int N, int X) {
    unsigned long eas[X];
    generate(map, 0, N, eas, X);
}

void createMapping(double **map, int no_ea, int no_neighbours){
	// calculate size
	int size = binom(no_ea + no_neighbours, no_ea);
	// allocate memory for the map
	*map = (double*) calloc(size, sizeof(double));
	if(!*map) {
		fprintf(stderr, "\nError allocating memory for the map in createMapping function!\n");
		exit(1);
	}

	// fill the map based on the function
	for(unsigned int nrep = 0; nrep <= no_neighbours; ++nrep){
		generate_all(*map, nrep, no_ea) ;
	}
	
}


double metabolizmus(int *matrix_f, int *met_szomszedsag_f, int method_f, int szomsz_cellaszam_f, int enzimaktszam_f, double reciprocEnzimaktszam_f, int cella_f) {
//	printf("\nenzimaktszam=%d", enzimaktszam_f);
	int szomszed_f = 0, nezett_f=0, enzakt_f = 0;
	unsigned long *enzimsum_f, metab_f=1;
	
	/* szomszed_f: a target (cella_f) szomszedjanak sorszama/ hanyadik szomszedot nezzuk eppen
	 * nezett_f: hanyadik cellat nezzuk eppen
	 * enzakt_f: a nezett enzakt szamlaloja
	 * enzimsum_f: az adott enzimaktivitasok osszege
	 * metab_f: M, azaz a metabolizmus erteke az adott cellara
	 */
	
	enzimsum_f=(unsigned long*) calloc(enzimaktszam_f, sizeof(unsigned long));
	
	//Conunting enzyme activity
//	printf("\ncella: %d (szerk, spec, eakt)\n", cella_f);
	for(szomszed_f=0; szomszed_f < szomsz_cellaszam_f; szomszed_f++) {
		nezett_f = *(met_szomszedsag_f+cella_f*szomsz_cellaszam_f+szomszed_f);
		if ( (*(matrix_f+nezett_f)) > 0 ) {
		  //*(enzimsum_f+(*(matrix_f+nezett_f)).k) = *(enzim_f+nezett_f*enzimaktszam_f+(*(matrix_f+nezett_f)).k);
		  (*(enzimsum_f + *(matrix_f+nezett_f) - 1))++ ;
//  		printf("\nnezett=%d\tdominans=%d", nezett_f, dominans_f);
//  		printf(" (%d, %d, %g)", (*(matrix_f+nezett_f)).szerk, (*(matrix_f+nezett_f)).spec, *(enzim_f+nezett_f*enzimaktszam_f+dominans_f));
        }
	}
	
    //Check if there is a 0 amongst activities
    for(enzakt_f=0; enzakt_f<enzimaktszam_f; enzakt_f++){
		if( *(enzimsum_f+enzakt_f) <= 0 ) {
			free(enzimsum_f);
			return(0);
		}
    }
	
	//Metabolic function - there is no zero!
	switch(method_f) {
		// 1: M = (e1*e2*...*en)^(1/n)
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
		// 2: M = minimum(e1, e2, ..., en)
		case 2:
			metab_f = minimum(enzimsum_f, enzimaktszam_f);
			break;
		// 3: M = 1 / (1/e1 + 1/e2 + ... + 1/en)
		case 3:
			metab_f = 0;
			for(enzakt_f=0; enzakt_f<enzimaktszam_f; enzakt_f++){
				metab_f += 1 / ((double) *(enzimsum_f+enzakt_f));
			}
			metab_f = 1 / metab_f;
			break;
		// 4: flat (if any is 0, M=0, else M=1)
		case 4:
			metab_f = 1;
			for(enzakt_f=0; enzakt_f<enzimaktszam_f; enzakt_f++){
				if( *(enzimsum_f+enzakt_f) == 0 ) {
					metab_f = 0;
					break;
				}
			}
			break;
		// 5: M = U(0,2)
		case 5:
			metab_f = fitness_map[getIndex(enzimsum_f, enzimaktszam_f)];
			
			break;
		// 6: M = (e1*e2)^n (n=-5.0, -4.5, ... 0.0 ... 5.0)
	}
	
	free (enzimsum_f);
//	printf("\nvegso metab_f=%g", metab_f);
	return (metab_f);
}

