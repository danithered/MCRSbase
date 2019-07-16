#include <mainheader.h>
#include <stdlib.h>
#include <stdio.h>
#include <randomgen.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>

int main(int argc, char *argv[]) { 
	/*char *token, *read2, from[50] = "\0", read[256]="\0", argv2[256];
	FILE *file;
	int i;
	
	strcpy(from, argv[1]);
	file = fopen(from, "r");
	fgets(read, 256, file);
	fgets(read, 256, file);
	printf("\n%s\n", read);
	
	
	//argv2 = (char *) calloc(10, sizeof(char[256]));
	read2 = strdup(read);
	for(i=1; ((token = strsep(&read2, " "))); i++ ) strcpy(argv[i], token);
	
	
	
	
	for(i=1; i< argc; i++){
		printf("\n%s", argv[i]);
	}
	
	//free(argv2);
	*/
	
	
	int i=0;
	for(i=1; i<argc; i++) {
		printf("%s - %s\n", argv[0], argv[i]);
	}
	
	
	return(0);
}
