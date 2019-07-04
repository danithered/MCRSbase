#include <randomgen.h>

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
	
	int choice=0, osszeg=*(values+noChoices);
	
	for (choice = 0; choice < noChoices; choice++) {
		if (random < (*(values + choice)/osszeg ) ) {
			return(choice);
		}
	}
}

double minimum(double *vector_f, int dbszam_f) {
	int szamlalo_f = 0; 
	double lokalismin_f = *vector_f;
	
	for(szamlalo_f = 1; szamlalo_f < dbszam_f; szamlalo_f++) {
		if( *(vector_f + szamlalo_f) < lokalismin_f ) lokalismin_f = *(vector_f + szamlalo_f);
	}
	return(lokalismin_f);
}