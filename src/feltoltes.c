#include <mainheader.h>
#include <randomgen.h>


void inicM(int *matrix_f, double *inicEA_f, int noEA_f, int meret_f){
    int cellaszam_f=0, ea_f=0;
    double dobott_f, *inic_f;
    
    //matrix lefoglal
    inic_f = (double *) calloc(noEA_f, sizeof(double));
    if(!inic_f) printf("\n\ninicializalasi hiba: nem lehetett lefoglalni matrixot (inicM)\n");
    
    //kiszamit inic
    *(inic_f) = *(inicEA_f); //nulladik marad
    for(ea_f = 1; ea_f < noEA_f; ea_f++){
        *(inic_f + ea_f) = *(inicEA_f + ea_f) + *(inic_f + ea_f - 1); //kovetkezok legyenek kumulaltak
/**/        printf("%f\n", *(inic_f + ea_f));
    }
    if( *(inic_f + noEA_f - 1) > 1 ) printf("\nWARNING: az iniciacios gyakorisagok osszege nagyobb 1-nel!\n");
    
    //feltolt matrixot
    for(cellaszam_f = 0; cellaszam_f < meret_f; cellaszam_f++) {
        dobott_f = gsl_rng_uniform(r);
        for(ea_f=0; ea_f < noEA_f; ea_f++){
            if(dobott_f < *(inic_f + ea_f) ) {
                *(matrix_f + cellaszam_f) = ea_f+1;
/**/                printf("%d pozicioban: random szam = %f, replikator tipusa = %d\n", cellaszam_f, dobott_f, *(matrix_f + cellaszam_f ) );
                break;
            }
        }
        
    }
    
    //felszabadit kumulalt matrixot
    free(inic_f);
}
