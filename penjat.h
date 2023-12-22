#ifndef PENJAT_H
#define PENJAT_H

#define MIDA_PARAULA 10
#define TOTAL_PARAULES 5
#define INTENTS 10

#define JA_PROVADA -1
#define HI_ES 1
#define NO_HI_ES 0

#define PERDUT -1
#define GUANYAT 1
#define JUGANT 0

typedef struct penjat
{
    int client_id;
    char paraula[MIDA_PARAULA];
    char paraula_oculta[MIDA_PARAULA];
    char lletres_provades[25];
    int intents;
    int estat;

} paraula_t;

paraula_t *crear_paraula();

//Comprovar si la lletra es a la paraula
// Return   -1 -> ja s'ha dit
//          1 -> hi es
//          0 -> no hi es
int comprovar_lletra(paraula_t *paraula, char lletra);

// revela la lletra encertada a la paraula oculta
void revelar_lletra (paraula_t *paraula, char lletra);

// comprovar si hem encertat la paraula
int paraula_encertada (paraula_t *paraula);

bool fi_joc(paraula_t *paraula);

#endif