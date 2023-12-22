/* Protocol: joc del penjat */
/* SERVIDOR */

/* Inclusio de fitxers .h habituals */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "penjat.h"

/* Inclusio de fitxers .h per als sockets */
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define MIDA_PAQUET 500

typedef struct node {
    paraula_t data;
    struct node *seguent;
} node_t;



// trobar struct per id
paraula_t *buscar_per_id(node_t *punter, int id) {
    node_t *actual = punter;
    while (actual != NULL) {
        if (actual->data.client_id == id) {
            return &(actual->data);
        }
        actual = actual->seguent;
    }
    return NULL;  // no trobat
}

// afegir struct a la llista
node_t *afegir_node(node_t *punter, paraula_t data) {
    node_t *newNode = (node_t *)malloc(sizeof(node_t));
    if (newNode == NULL) {
        return punter; // llista sense canvis
    }

    newNode->data = data;
    newNode->seguent = punter;
    return newNode;  // nou punter de la llista
}

// eliminar struct per id
node_t *eliminar_per_id(node_t *punter, int id) {
    node_t *actual = punter;
    node_t *prev = NULL;

    while (actual != NULL) {
        if (actual->data.client_id == id) {
            if (prev == NULL) {
                // eliminar punter
                punter = actual->seguent;
            } else {
                prev->seguent = actual->seguent;
            }
            free(actual);
            return punter;  // nou punter
        }
        prev = actual;
        actual = actual->seguent;
    }

    // id no trobat
    return punter;
}

// Lliberar la memoria
void alliberar(node_t *punter) {
    node_t *actual = punter;
    while (actual != NULL) {
        node_t *seguent = actual->seguent;
        free(actual);
        actual = seguent;
    }
}

int main(int argc, char **argv)
{

    if (argc == 2)
    {
        int s;                              /* El socket */
        struct sockaddr_in socket_servidor; /* Socket on escolta el servidor */
        struct sockaddr_in adr_client;      /* Adreça i port des d'on el client envia el paquet */
        socklen_t adr_client_mida = sizeof(adr_client);          /* Longitud de les dades adreça i port */

        char paquet[MIDA_PAQUET]; /* Per posar les dades a enviar/rebre */

        /* Volem socket d'internet i no orientat a la connexio */
        s = socket(AF_INET, SOCK_DGRAM, 0);

        /* Posem les dades del socket del servidor */
        socket_servidor.sin_family = AF_INET;            /* Socket a Internet */
        socket_servidor.sin_addr.s_addr = INADDR_ANY;    /* Qualsevol NIC */
        socket_servidor.sin_port = htons(atoi(argv[1])); /* Port on estarà escoltant el servidor */

        /* Enllacem el socket */
        int resultat_bind;
        resultat_bind = bind(s, (struct sockaddr *)&socket_servidor, sizeof(socket_servidor));

        if (resultat_bind < 0)
        {
            printf("No s'ha pogut enllacar el socket\n");
        }
        else
        {
            printf("Servidor operatiu al port %d!\n", atoi(argv[1]));

            char lletra, p_client[20];
            int client_id, opcio;
            bool fi = false;
            node_t *connexions = NULL;
            paraula_t *p = NULL;

            while (1)
            {
                printf("Esperant petició d'algun client...\n");
                /* Quan es rep un paquet, a adr_client s'hi anota la IP i port des d'on s'envia */
                recvfrom(s, paquet, MIDA_PAQUET, 0, (struct sockaddr *)&adr_client, &adr_client_mida);

        
                /* Llegim les dades que ens envien*/
                sscanf(paquet, "%d %d %c %s", &client_id, &opcio, &lletra, p_client);

                // protocol

                // nou joc
                if (client_id==0){

                    p = crear_paraula(client_id);
                    connexions = afegir_node(connexions, *p);
                    printf("new connection\n");

                // comprovar lletra
                }else if (opcio==1){

                    if (comprovar_lletra(p, lletra)) revelar_lletra(p, lletra);

                // provar paraula
                }else if (opcio==2){

                    if (paraula_encertada(p) || strcmp(p_client, p->paraula)==0) p->estat = GUANYAT;
                    else p->intents--;

                }

                fi = fi_joc(p);

                sprintf(paquet, "%d %s %d %d\n", p->client_id, !fi?p->paraula_oculta:p->paraula , p->intents, p->estat);      

                /* Enviem el paquet a l'adreça i port on està esperant el client */
                sendto(s, paquet, MIDA_PAQUET, 0, (struct sockaddr *)&adr_client, adr_client_mida);
            }
        }

        /* Tanquem el socket */
        close(s);
    }

    else
    {
        printf("El nombre de paràmetres no és el correcte!\n");
    }
    return 0;
}