#include <stdio.h>
#define MUTACIO mutacioE3
#define TRANSITION transition3

//#define BITNUMFORMAT %(BITNUM)d
	
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

//bitmuveletek.c
int olvas (int, int);
int ir (int, int, int);
int kiirbit (int, int);

//statisztika.c
int alatt (int, int);
int faktorialis (int);
int hanyfajta (int);
double szorasD (int, double, double);

//kimenet.c
void fajlbaMatrix(int *matrix_f, int ncol_f, int nrow_f, FILE *out_f);

//save.c
