#include <stdio.h>
#define MUTACIO mutacioE3
#define TRANSITION transition3

//#define BITNUMFORMAT %(BITNUM)d
extern unsigned int NOEA;
	
//feltoltes.c
void inicM(int *matrix_f, double *inicEA_f, int noEA_f, int meret_f);

//konzolra.c
void konzolraMatrix(int *, int, int);
void konzolraMatrixD(double *, int, int);

//szomszed.c
int* metNeighInic(int, int, double);
int szomsz_meret(double);
int* difNeighInic(int meret_f, int ncol_f);

//torus.c
int torus(int, int);

//eszkozok.c
int torottpalca(double *values, int noChoices, double random);
unsigned long minimum(unsigned long *vector_f, int dbszam_f);
double minimumD(double *vector_f, int dbszam_f);
int betoltesRng(FILE *rngLoad_s);
int mentesRng(char *filename_f);

//bitmuveletek.c
//int olvas (int, int);
//int ir (int, int, int);
//int kiirbit (int, int);

//statisztika.c
//int alatt (int, int);
//int faktorialis (int);
//int hanyfajta (int);
//double szorasD (int, double, double);

//kimenet.c
void fajlbaMatrix(int *matrix_f, int ncol_f, int nrow_f, FILE *out_f);
int atlagadatok(int *matrix_f, int meret_f, int noEa_f, int ciklus_f, FILE *fajl_f);

//save.c

//diffuzio.c
int diffTM (int *matrix_f, int *diffmatrix_f, int target_f);

//metab.c
double metabolizmus(int *matrix_f, int *met_szomszedsag_f, int method_f, int szomsz_cellaszam_f, int enzimaktszam_f, double reciprocEnzimaktszam_f, int cella_f);
unsigned long long getIndex(unsigned long *eas, unsigned long noEA);
void createMapping(double **map, int no_ea, int no_neighbours);

