#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>
#include "protocoles/OVESP.h"
#include <string.h>
#include <cstring>
#include <stdbool.h>
#include "protocoles/TCP.h"
#include <string>
using namespace std;

#define TAILLE_FILE_ATTENTE 20

// Déclarations des fonctions
void* MonThread(void* arg);
void gestionnaireCtrlC(int signum);
void ViderPanier(int* IndiceProchainArtPan, struct panier* panierEnCours);




int indiceEcriture = 0;
int indiceLecture = 0;

struct panier
{
    char NomArticle[50];
    int numArt;
    int qu;
    float prix;
};

int sService;

// Variable globale pour stocker le PID du thread principal
pid_t pidPrincipal;

pthread_mutex_t mutexSocketsAcceptees;
pthread_cond_t condSocketsAcceptees;

int socketsAcceptees[TAILLE_FILE_ATTENTE];

int sEcoute;

int main(int argc, char *argv[]) {
    // Stocke le PID du thread principal
    pidPrincipal = getpid();

char ipClient[16];


    // Initialisation socketsAcceptees
     pthread_mutex_init(&mutexSocketsAcceptees,NULL);
     pthread_cond_init(&condSocketsAcceptees,NULL);
     for (int i=0 ; i<TAILLE_FILE_ATTENTE ; i++)
     socketsAcceptees[i] = -1;




     // Creation de la socket d'écoute
     if ((sEcoute = ServerSocket(50406)) == -1)
     {
     perror("Erreur de ServeurSocket");
     exit(1);
     }




    // Création de 5 threads
    pthread_t threads[5];
    for (int i = 0; i < 5; ++i) {
        if (pthread_create(&threads[i], NULL, MonThread, NULL) != 0) {
            perror("Erreur lors de la création du thread");
            exit(EXIT_FAILURE);
        }
    }






// Reste du code ici
    
    while(1)
    {
         printf("Attente d'une connexion...\n");
         if ((sService = Accept(sEcoute,ipClient)) == -1)
         {
             perror("Erreur de Accept");
             close(sEcoute);
             exit(1);
         }
         printf("Connexion acceptée : IP=%s socket=%d\n",ipClient,sService);
         // Insertion en liste d'attente et réveil d'un thread du pool
         // (Production d'une tâche)
         pthread_mutex_lock(&mutexSocketsAcceptees);
         socketsAcceptees[indiceEcriture] = sService; // !!!
         indiceEcriture++;
         if (indiceEcriture == TAILLE_FILE_ATTENTE)
         {
            indiceEcriture = 0;  
         } 
         pthread_mutex_unlock(&mutexSocketsAcceptees);
         pthread_cond_signal(&condSocketsAcceptees);

    }



    // Attente des threads
    for (int i = 0; i < 5; ++i) {
        if (pthread_join(threads[i], NULL) != 0) {
            perror("Erreur lors de l'attente du thread");
            exit(EXIT_FAILURE);
        }



    }

    return 0;
}

// Implémentation de la fonction MonThread
void* MonThread(void* arg) {
    int sService;
    pid_t tid = getpid();

    int idAchteur; //Numéro de client depuis la BD

    panier panierEnCours[10];

    size_t size = sizeof(struct panier) * 10;
    memset(panierEnCours, 0, size);

    int IndiceProchainArtPan = 0;
    //int* ptrIPAP = &IndiceProchainArtPan;

    char reponse[1500];
    char requete[1500];
    char travail[1500];
    char donnees[1500];
    char* pourrien;
    char* pourrien2;
    char* savePtr;
    bool toutOK;

    int art;
    int qu;
    int numArt;
    float prix;



    pthread_mutex_lock(&mutexSocketsAcceptees);

    while (indiceEcriture == indiceLecture)
    pthread_cond_wait(&condSocketsAcceptees,&mutexSocketsAcceptees);

    sService = socketsAcceptees[indiceLecture];
    socketsAcceptees[indiceLecture] = -1;
    indiceLecture++;
    if (indiceLecture == TAILLE_FILE_ATTENTE) indiceLecture = 0;
    pthread_mutex_unlock(&mutexSocketsAcceptees);


    while(1)
    {
        printf ("\n\t J ENTRE DANS LE WHILE J ATTEND...\n");

            Receive(sService, requete);
        

            // Lors de la réception des trames
        printf("\nREQUETE :  %s\n", requete);

        toutOK = OVESP(requete, reponse, sService);
        strcpy(travail, reponse);
        pourrien2 = strtok_r(travail, "#", &savePtr);
        strcpy(donnees,pourrien2);

        if (strcmp(donnees, "LOGIN2") == 0)
        {
            if(toutOK == true)
            {
                idAchteur = atoi(strtok_r(NULL,"#", &savePtr));
                printf ("\n\t Connection client réussie. ID client : %d\n", idAchteur);
            }
            else
            {
                printf ("\nLa connection a echoue. L'id client rest à %d (Logiquement 0)\n.", idAchteur);
            }


        }

        // If FLECHES GAUCHES - DROITES -> Serveur n'en a rien à faire.
        // Ceci est gèré dans le protocole.


        if (strcmp(donnees, "ACHAT2") == 0)
        {
            char* temp = (char*)malloc(1500 * sizeof(char));
            char* temp2 = (char*)malloc(1500 * sizeof(char));

            printf ("Je suis le serveur et ACHAT2 = %s", reponse);
                    //On récupere 
                pourrien = strtok_r(NULL,"#", &savePtr);
                if (strcmp(pourrien, "KO") == 0)
                {
                    strcpy(reponse, "ACHAT2#KO");
                }
                else
                {
                    printf ("\nDANS LE ELSE et pourrien = %s\n", pourrien);
                    int art = atoi(pourrien);
                    printf ("Atoi OK et art : %d\n", art);
                    panierEnCours[IndiceProchainArtPan].numArt = art;   // Num article on le prend on envoie pas
                    printf ("panierEnCours[IndiceProchainArtPan].numArt %d \n", panierEnCours[IndiceProchainArtPan].numArt);

                    strcpy(reponse, "ACHAT2#OK#");

printf ("\n\n\n\t----------\n--- Notre trame COMMENCE : %s\n", reponse);
                    pourrien = (strtok_r(NULL,"#", &savePtr)); // Nom, on le prend on l'envoi
                    strcpy (temp2, pourrien);
                    strcat(reponse, temp2); 
                    strcat(reponse, "#");
                    strcpy(panierEnCours[IndiceProchainArtPan].NomArticle, temp2);

                    printf ("\n\n\n\t----------\n--- Notre trame a déjà le nom : %s\n", reponse);



                    pourrien = (strtok_r(NULL,"#", &savePtr));
                    strcpy(temp2, pourrien);
                    strcat(reponse, temp2);
                    strcat(reponse, "#");
                    art = atoi(temp2);
                    panierEnCours[IndiceProchainArtPan].qu = art;
                    printf ("241 OK\n");

                    pourrien = (strtok_r(NULL,"#", &savePtr));
                    strcpy(temp2, pourrien);
                    strcat(reponse, temp2); //Pas de carré c'est le dernier
                    art = atoi(temp2);
                    float monPrix = (float)art;
                    monPrix = monPrix / (float) 100;
                    panierEnCours[IndiceProchainArtPan].prix = monPrix;
                    
                    IndiceProchainArtPan = IndiceProchainArtPan + 1;
                  
                }
                    free(temp);
                    printf ("OK 267\n");
                    free(temp2);
                    printf ("OK 269\n");
            }
            


            if (strcmp(donnees, "VIDERPANIER") == 0)
            {
              
              for (int i = 0; i < IndiceProchainArtPan; i++)
              {
                SupprimeArticle(panierEnCours[i].numArt, panierEnCours[i].qu);
              }
              IndiceProchainArtPan = 0;







            }

            if (strcmp(donnees, "LOGOUT2") == 0)
            {
              for (int i = 0; i < IndiceProchainArtPan; i++)
              {
                SupprimeArticle(panierEnCours[i].numArt, panierEnCours[i].qu);
              }
              IndiceProchainArtPan = 0;
                //Envoi réponse au client
                Send(sService, reponse, strlen(reponse));
                pthread_exit(0);
            }

            if (strcmp(donnees, "SUPPRESSION2") == 0)
            {
                int ArtaSuppr = atoi(strtok_r(NULL, "#", &savePtr)); // On a l'ID
            char data[10];
            float monFloat;
            int monInt;
            char monPrix[10];
            char prix[10];
                printf ("Article à suppremer ligne no : %d", ArtaSuppr);


                SupprimeArticle(panierEnCours[ArtaSuppr].numArt, panierEnCours[ArtaSuppr].qu);


                for (ArtaSuppr; ArtaSuppr < IndiceProchainArtPan; ArtaSuppr++)
                {
                    panierEnCours[ArtaSuppr].numArt = panierEnCours[ArtaSuppr+1].numArt;
                    panierEnCours[ArtaSuppr].qu = panierEnCours[ArtaSuppr+1].qu;
                    panierEnCours[ArtaSuppr].prix = panierEnCours[ArtaSuppr+1].prix;
                    strcpy(panierEnCours[ArtaSuppr].NomArticle, panierEnCours[ArtaSuppr+1].NomArticle);
                }

                IndiceProchainArtPan --;
                    //préparer une réponse pour le client
                strcpy(reponse, "");

                sprintf(reponse, "SUPPOK#%d#", IndiceProchainArtPan);
                for (int i = 0; i < IndiceProchainArtPan-1; i++)
                {
                    strcat(reponse, panierEnCours[i].NomArticle);
                    strcat(reponse, "#");
                    sprintf(travail, "%f", panierEnCours[i].prix);
                        //strcpy(data, panierEnCours[i].prix)
                    sprintf (data, "%f", panierEnCours[i].prix);
        strcpy(prix, data);
                        string tmp4(prix);
                        size_t x = tmp4.find(",");
                        if (x != string::npos) tmp4.replace(x,1,".");
 

        monFloat = atof(prix);
        monFloat = monFloat * 100;
        monInt = (int) monFloat;
        

        sprintf (monPrix, "%d", monInt);

           strcat(reponse, monPrix);




                    strcat(reponse, "#");
                    sprintf(travail, "%d", panierEnCours[i].qu);

                    strcat(reponse, travail);
                    strcat (reponse, "#");
                }
                    int i = IndiceProchainArtPan -1;
                    strcat(reponse, panierEnCours[i].NomArticle);
                    strcat(reponse, "#");
                    sprintf(travail, "%f", panierEnCours[i].prix);

    //strcpy(data, panierEnCours[i].prix)
                    sprintf (data, "%f", panierEnCours[i].prix);
        strcpy(prix, data);
                        string tmp4(prix);
                        size_t x = tmp4.find(",");
                        if (x != string::npos) tmp4.replace(x,1,".");
 

        monFloat = atof(prix);
        monFloat = monFloat * 100;
        monInt = (int) monFloat;
        

        sprintf (monPrix, "%d", monInt);

           strcat(reponse, monPrix);






                //    strcat(reponse, travail);
                    strcat(reponse, "#");
                    sprintf(travail, "%d", panierEnCours[i].qu);
                    strcat(reponse, travail);

printf ("******************************************\n******************************************\n******************************************\n******************************************\n******************************************\n");


                    for (i = 0; i < IndiceProchainArtPan; i++)
                    {
                        printf ("%d   - ", i);
                        printf ("%s  - ", panierEnCours[i].NomArticle);
                        printf ("%d   - ", panierEnCours[i].numArt);
                        printf ("Qu : %d   - ", panierEnCours[i].qu);
                        printf ("Px : %f   *\n", panierEnCours[i].prix);

                    }
printf ("******************************\n");

            }


            if (strcmp(reponse, "VALIDEPANIER2") == 0 && IndiceProchainArtPan > 0)
            {
                printf ("HAHAHAH\n");
                int resultat;
                float totalFacture = 0;
                char* NumVsChar;
                int numFacture;

                for (int i = 0; i < IndiceProchainArtPan; i++)
                {
                    totalFacture = totalFacture + panierEnCours[i].prix *(float)(panierEnCours[i].qu);
                }


                numFacture = NouvelleFacture(idAchteur,totalFacture);

                printf ("\nMA facture porte le numero : %d\n", numFacture);

                for (int i = 0; i < IndiceProchainArtPan; i++)
                {
                    NouvelleVente(numFacture, panierEnCours[i].numArt, panierEnCours[i].qu);
                }
                IndiceProchainArtPan = 0;





                      
            }
printf ("apres VALIDEPANIER2\n");
        printf ("BLABLABLABLABLABLABLAL\n");

                //Envoi REPONSE vers client
        printf("\n SERVEUR ENVOIE : %s\n", reponse);
        Send(sService, reponse, strlen(reponse));



        
        printf ("\n\nFIN DE WHILE\n");
}

    printf("Je suis le thread %d\n", tid);
    
    // Installation du gestionnaire de signal pour Ctrl + C dans chaque thread
    signal(SIGINT, gestionnaireCtrlC);


    return NULL;
}

// Implémentation du gestionnaire de signal pour Ctrl + C
void gestionnaireCtrlC(int signum) 
{
    close(sEcoute);
    pthread_mutex_lock(&mutexSocketsAcceptees);
    for (int i=0 ; i<TAILLE_FILE_ATTENTE ; i++)
    if (socketsAcceptees[i] != -1) close(socketsAcceptees[i]);
    pthread_mutex_unlock(&mutexSocketsAcceptees);
    //SMOP_Close();
    exit(0);
}

