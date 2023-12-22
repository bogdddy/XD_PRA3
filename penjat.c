#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include "penjat.h"

char paraules[TOTAL_PARAULES][MIDA_PARAULA] = {"first\0", "second\0", "third\0", "fourth\0", "fifth\0"};

paraula_t *crear_paraula(){

    srand(time(NULL));

    paraula_t *newParaula = (paraula_t *)malloc(sizeof(paraula_t));
    if (newParaula == NULL) {
        fprintf(stderr, "Error: Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }

    newParaula->client_id = rand();

    int r = rand() % 5;

    // escollim una paraula aleatoria del diccionari
    strcpy(newParaula->paraula, paraules[r]);

    int i;
    // omplim de _ segons les lletres de la paraula
    for (i=0; i<strlen(newParaula->paraula) ; i++)
        newParaula->paraula_oculta[i] = '_';
    
    newParaula->paraula_oculta[i+1] = '\n';

    newParaula->intents = INTENTS;

    newParaula->estat = JUGANT;

    return newParaula;

}

//Comprovar si la lletra es a la paraula
// Return   -1 -> ja s'ha dit
//          1 -> hi es
//          0 -> no hi es
int comprovar_lletra(paraula_t *paraula, char lletra){

    // lletra provada?
    for (int i=0; i<strlen(paraula->lletres_provades); i++)
        if (paraula->lletres_provades[i] == lletra) return JA_PROVADA;

    strncat(paraula->lletres_provades, &lletra, 1); // afegir lletra a lletres_provades

    // hi es?
    for (int i=0; i<strlen(paraula->paraula); i++)
        if (paraula->paraula[i] == lletra) return HI_ES;

    paraula->intents--;
    
    return NO_HI_ES;


}

// revela la lletra encertada a la paraula oculta
void revelar_lletra (paraula_t *paraula, char lletra){

    for (int i=0; i<strlen(paraula->paraula); i++)
        if (paraula->paraula[i] == lletra) paraula->paraula_oculta[i] = lletra;
    
}

// comprovar si hem encertat la paraula
int paraula_encertada (paraula_t *paraula){

    for (int i=0; i<strlen(paraula->paraula); i++)
        if (paraula->paraula_oculta[i] == '_') return 0;

    return 1;
}

// el joc acaba si sencerta la paraula o s'esgoten els intents
bool fi_joc (paraula_t *paraula){

    if (strcmp(paraula->paraula, paraula->paraula_oculta)==0){
        paraula->estat = GUANYAT;
        return true;
    }else if (paraula->intents == 0){
        paraula->estat = PERDUT;
        return true;
    }
        
    return false;

}

// int main(){

//     paraula_t p;

//     inicialitzar(&p);

//     printf("%s\n%s\n", p.paraula, p.paraula_oculta);

//     char lletra;
//     int fi = 0;

//     while (!fi){

//         scanf(" %c", &lletra);

//         if (comprovar_lletra(&p, lletra) == HI_ES)
//             revelar_lletra(&p, lletra);

//         printf("%s\n", p.paraula_oculta);
//         fi = paraula_encertada(&p);
        

//     }


//     return 0;

// }