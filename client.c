/* Protocol: el client envia un número i el servidor retorna el número multiplicat per 2 */
/* CLIENT */

/* Inclusio de fitxers .h habituals */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

/* Inclusio de fitxers .h per als sockets */
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define MIDA_PAQUET 500

int main(int argc, char **argv)
{

    if (argc == 3)
    {

        int s; /* El socket */
        struct sockaddr_in adr_servidor; /* Adreça i port on hi ha el servidor */
        socklen_t adr_servidor_mida; /* Longitud de les dades adreça i port */
        
        char paquet[MIDA_PAQUET];   /* Per posar les dades a enviar/rebre */

        /* Volem socket d'internet i no orientat a la connexio */
        s = socket(AF_INET, SOCK_DGRAM, 0);

        adr_servidor.sin_family = AF_INET;  /* Socket a Internet */
        adr_servidor.sin_addr.s_addr = inet_addr(argv[1]); /* Adreça on està el servidor */
        adr_servidor.sin_port = htons(atoi(argv[2])); /* Port on escolta el servidor */

        srand(time(NULL));
        int client_id, opcio=0, intents, estat;
        char lletra;
        char paraula[20];

        /* Execucio del protocol */
        
        /* Muntem el buffer */
        sprintf(paquet, "%d\n", 0);

        sendto(s, paquet, MIDA_PAQUET, 0, (struct sockaddr *)&adr_servidor, sizeof(adr_servidor));
        recvfrom(s, paquet, MIDA_PAQUET, 0, NULL, NULL); /* NULL -> No cal saber des d'on ens envien el paquet. */
       
        printf(paquet, "id partida:%d \n");

        /* Llegim les dades que ens envien*/
        sscanf(paquet, "%d %[^0-9] %d\n", &client_id, paraula, &intents);


        while (estat!=1 && estat!=-1)
        {

            printf("Paraula oculta: %s Intents restants: %d\n", paraula, intents);
            printf("1-lletra; 2-resoldre 3-sortir \n");
            scanf("%d",&opcio);

            getchar();

            // endevinar lletra
            if (opcio==1){
                
                printf("Posa una lletra ");
                scanf("%c",&lletra);
 
                /* Muntem el buffer */
                sprintf(paquet, "%d %d %c\n", client_id, 1, lletra);
                sendto(s, paquet, MIDA_PAQUET, 0, (struct sockaddr *)&adr_servidor, sizeof(adr_servidor));
                
            // endevinar paraula
            }else if(opcio == 2){
                printf("Posa la paraula oculta ");
                scanf("%s",paraula);

                sprintf(paquet, "%d %d %c %s\n", client_id, 2, lletra, paraula);
                sendto(s, paquet, MIDA_PAQUET, 0, (struct sockaddr *)&adr_servidor, sizeof(adr_servidor));
                
            }else estat = 1;

            // reposta srv
            recvfrom(s, paquet, MIDA_PAQUET, 0, NULL, NULL);
            sscanf(paquet, "%d %s %d %d\n", &client_id, paraula, &intents, &estat);

        }

        printf(estat == 1?"Enhorabona has guanyat!!\n":"Mes sort el proxim cop \n");

        /* Tanquem el socket */
        close(s);
    }

    else
    {
        printf("El nombre de paràmetres no és el correcte!\n");
    }

    return 0;
}