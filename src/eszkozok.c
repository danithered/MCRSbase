#include <randomgen.h>
#include <sys/stat.h>
#include <sys/types.h>

int torottpalcaProb(double *props, int noChoices, double random) {
	/* torott palca kumulalt bemeneti ertekekkel
	 * visszateresi ertekek:
	 * 	0-(noChoices-1): melyiket valassztottuk
	 * 	-1: egyik sem
	 * props: a pointer a kumulalt ertekekhez
	 * noChoices: hany tagja van a props-nak
	 * random: a random szam
	 * 
	 * choice: szamlalo, vegigmegy a props-on
	 */
	
	int choice=0;
	for (choice = 0; choice < noChoices; choice++) {
		if (random < *(props + choice) ) {
			return(choice);
		}
	}
	return(-1);
}

int torottpalca(double *values, int noChoices, double random) {
	/* torott palca kumulalt bemeneti ertekekkel
	 * visszateresi ertekek:
	 * 	0-(noChoices-1): melyiket valassztottuk
	 * props: a pointer a kumulalt ertekekhez
	 * noChoices: hany tagja van a props-nak
	 * random: a random szam
	 * 
	 * choice: szamlalo, vegigmegy a props-on
	 * osszeg: az utolso ertek
	 */

	int choice=0; 
	double osszeg=0;
	
//	printf("noChoices= %d\n", noChoices);
	osszeg = *(values + noChoices - 1);
//	printf("torottpalca osszeg= %g, noChoices= %d\n", osszeg, noChoices);
	
	for (choice = 0; choice < noChoices; choice++) {
//		printf("torottpalca (%d): %g / %g = %g\n", choice, *(values + choice), osszeg, *(values + choice)/osszeg);
		if (random < (*(values + choice)/osszeg ) ) {
//			printf("torottpalca choice: %d; random value= %g, limit: %g\n", choice, random, *(values + choice)/osszeg);
			return(choice);
		}
	}
}

double minimumD(double *vector_f, int dbszam_f) {
	int szamlalo_f = 0; 
	double lokalismin_f = *vector_f;
	
	for(szamlalo_f = 1; szamlalo_f < dbszam_f; szamlalo_f++) {
		if( *(vector_f + szamlalo_f) < lokalismin_f ) lokalismin_f = *(vector_f + szamlalo_f);
	}
	return(lokalismin_f);
}

unsigned long minimum(unsigned long *vector_f, int dbszam_f) {
	int szamlalo_f = 0; 
	unsigned long lokalismin_f = *vector_f;
	
	for(szamlalo_f = 1; szamlalo_f < dbszam_f; szamlalo_f++) {
		if( *(vector_f + szamlalo_f) < lokalismin_f ) lokalismin_f = *(vector_f + szamlalo_f);
	}
	return(lokalismin_f);
}

int betoltesRng(FILE *rngLoad_s) {
	r = (gsl_rng *) gsl_rng_alloc (gsl_rng_mt19937);
	gsl_rng_fread (rngLoad_s, r);
  
	return(0);
}

int mentesRng(char *filename_f) {
	FILE *rngSave_f;
	struct stat st = {0}, stCsv = {0};
	
	if (stat(filename_f, &stCsv) != -1) {
		printf("\n%s ilyen nevu fajl mar van!\n", filename_f);
		return(4);
	}
	rngSave_f = fopen(filename_f, "a");
	
	gsl_rng_fwrite (rngSave_f, r);
	
	fclose(rngSave_f);
	
	return(0);

}

