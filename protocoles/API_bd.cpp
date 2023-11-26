#include <stdio.h>
#include <stdlib.h>
#include <mysql.h>
#include <string.h>
#include "API_bd.h"
#include "OVESP.h"
#include <string.h>
#include <string>
#include <cstring>
#include <time.h>
using namespace std;

MYSQL_RES *resultat;
MYSQL_ROW Tuple;
MYSQL *connexion;
MYSQL_ROW ligne;

char requete[200];
int nbArticles = 0;
int ret = 0;
int idQ;
int qudemande;
int qudispo;
int newqu;
char Prix[20];
char NewPrix[20];
char temp[20];
int senti = 0;
float TestPrix = -1;

void connection()
{
    connexion = mysql_init(NULL);
    if (mysql_real_connect(connexion, "localhost", "Student", "PassStudent1_", "PourStudent", 0, 0, 0) == NULL)
    {
        fprintf(stderr, "(SERVEUR) Erreur de connexion à la base de données...\n");
        exit(1);
    }
}




int VerifClient(char *login, char *mdp, char *data)
{
    printf ("\n Je suis dans VerifClient\n\n");
    printf ("Login : %s, mdp : %s, nouveau : %s", login, mdp, data);

    int nouveau = atoi(data);
    printf ("\n Convertion en int (pour nouveau ou pas) donne %d : \n", nouveau);
    int idClient;
    connection();

    if (nouveau == 0)
     {
        printf ("Je suis dans le traitement d'un client existant. \n");

        // Utiliser des requêtes préparées pour éviter les attaques par injection SQL
        sprintf(requete, "SELECT id FROM CLIENTS WHERE mdp = '%s' AND nom = '%s'", mdp, login);

        printf ("J'envoie la jolie requete suivante toute belle toute propre : \n %s\n", requete);

        if (mysql_query(connexion, requete) != 0)
        {
            printf ("Pas reussi à ernvoyer ma requete... \n");
            fprintf(stderr, "Erreur de Mysql-query: %s\n", mysql_error(connexion));
            mysql_close(connexion);
            printf ("\n Erreur if 1\n");
            return 0;  // Code d'erreur
        }
        else
        {
            printf ("J'ai envoyé cette saloperie de requete\n");
        }

        resultat = mysql_store_result(connexion);

        if (resultat == NULL)
        {
            printf ("\n Erreur if 2 - Client n existe pas dans la BD\n");
            fprintf(stderr, "Erreur lors de la récupération du résultat: %s\n", mysql_error(connexion));
            mysql_free_result(resultat);
            mysql_close(connexion);
            return 0;  // Code d'erreur 
        }
        else
        {
            ligne = mysql_fetch_row(resultat);

            if (ligne != NULL) {
                idClient = atoi(ligne[0]);
                printf ("\nJe l'ai trouvé au numéro %d - %s", idClient, ligne[0]);
            } else {
                printf ("\nErreur: Aucun client trouvé dans la BD\n");
                mysql_free_result(resultat);
                mysql_close(connexion);
                return 0;  // Code d'erreur
            }

            mysql_free_result(resultat);
            mysql_close(connexion);
            return idClient;

        }

        
    }
    
    else        // Insérer un nouveau client
    {
        sprintf(requete, "INSERT INTO CLIENTS (mdp, nom) VALUES ('%s', '%s')", mdp, login);
        if (mysql_query(connexion, requete) != 0)
        {
            fprintf(stderr, "Erreur lors de l'insertion d'un nouveau client: %s\n", mysql_error(connexion));
            mysql_close(connexion);
            return 99;  // Code d'erreur
        }

        // Récupérer le numéro du dernier client inséré
        if (mysql_query(connexion, "SELECT LAST_INSERT_ID()") == 0)
        {
            MYSQL_RES *resultat = mysql_store_result(connexion);
            if (resultat)
            {
                MYSQL_ROW ligne = mysql_fetch_row(resultat);
                if (ligne)
                {
                    int idClient = atoi(ligne[0]);
                    mysql_free_result(resultat);
                    mysql_close(connexion);
                    return idClient; //idClient;  // Retourner le numéro du client inséré
                }
                mysql_free_result(resultat);
            }
        }
    }

    mysql_close(connexion);

    return 0;  // Code d'erreur
}








void ArticleSuivant(int art, char* reponse)
{
   
    connection();

    /*AAA
        Faire une requete qui compte le nombre d'articles, si int art > au reour de la requete return 0
        De la sorte on est sur qu'on est pas au bout*/

    sprintf(requete, "select * from UNIX_FINAL where id = %d", art);



    if (mysql_query(connexion, requete) != 0)
    {
        fprintf(stderr, "Erreur de Mysql-query");
        strcpy(reponse, "ARTSUIVANT2#0");
        printf ("ERREUR 1\n");
        fflush(stdout);
    }

    if ((resultat = mysql_store_result(connexion)) == NULL)
    {
        fprintf(stderr, "Erreur de mysql store");
        strcpy(reponse, "ARTSUIVANT2#0");
                printf ("ERREUR 2\n");
        fflush(stdout);
    }
        printf ("AVANT LE 3EME IF\n");
        fflush(stdout);
    if ((Tuple = mysql_fetch_row(resultat)) != NULL)
    {
        printf ("DANS La PREMIERE LIGNE DU IF 3\n");
        fflush(stdout);

        printf ("Tuple[0], %s", Tuple[0]);
        fflush(stdout);

        printf ("Tuple[1], %s ", Tuple[1]);
        fflush(stdout);
        printf ("Tuple[2], %s ", Tuple[2]);
        fflush(stdout);
        printf ("Tuple[3], %s ", Tuple[3]);
        fflush(stdout);
        printf ("Tuple[4], %s ", Tuple[4]);
        fflush(stdout);
        printf ("\nC'est fait\n");
        fflush(stdout);
        
        strcpy(reponse, "ARTSUIVANT2#");
        strcat(reponse, Tuple[0]);
        strcat(reponse, "#");

        strcat(reponse, Tuple[1]);
        strcat(reponse, "#");
        strcat(reponse, Tuple[4]);
        strcat(reponse, "#");
        strcat(reponse, Tuple[3]);
        strcat(reponse, "#");
     
        
        printf ("\n PENDANT ON A REPONSE : %s", reponse);

        strcpy(Prix, Tuple[2]);
                        string tmp4(Prix);
                        size_t x = tmp4.find(",");
                        if (x != string::npos) tmp4.replace(x,1,".");
 

        float monFloat = atof(Prix);
        monFloat = monFloat * 100;
        int monInt = (int) monFloat;
        char monPrix[10];

        sprintf (monPrix, "%d", monInt);

           strcat(reponse, monPrix);
        printf ("\n ==========================D Dans LA BD, monFloat = %f, mon INT = %d\n ET MONPRIXFLOAT = %s", monFloat, monInt, monPrix);


        printf ("\n aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa APRES ON A REPONSE : %s\n", reponse);
        fflush(stdout);

    }

    mysql_close(connexion);
printf ("\nREPONSE : %s\n", reponse);
fflush(stdout);


}


void AchatArticleAPI(int art, int Qu, char* reponse)
{
    connection();

    sprintf(requete, "select * from UNIX_FINAL where id = %d", art);
    if (mysql_query(connexion, requete) != 0)
    {
        fprintf(stderr, "Erreur de Mysql-query");
        printf ("\nErreur 1 \n");
        fflush(stdout);
    }

    if ((resultat = mysql_store_result(connexion)) == NULL)
    {
        fprintf(stderr, "Erreur de mysql store");
        printf ("\nErreur 2 \n");
        fflush(stdout);
    }

    printf ("\nJE SUIS APRES LES 2 1er IF\n");
    fflush(stdout);

    if ((Tuple = mysql_fetch_row(resultat)) != NULL)
    {

        qudispo = atoi(Tuple[3]);
        printf ("\nQu DIspo = %d", qudispo);

        if (qudispo < Qu)
        {
            strcpy(reponse, "ACHAT2#KO");
        }
        else
        {
            sprintf(requete, "UPDATE UNIX_FINAL set stock = %d where id = %d", (qudispo - Qu), art);
            if (mysql_query(connexion, requete) != 0)
            {
                fprintf(stderr, "Erreur de Mysql-query");
                printf ("\nErreur 1 \n");
                fflush(stdout);
            }


            strcpy(reponse, "ACHAT2#");
            strcat(reponse, Tuple[0]); //panierEnCours[IndiceProchainArtPan].numArt = art;
            strcat(reponse, "#");
            strcat(reponse, Tuple[1]); // panierEnCours[IndiceProchainArtPan].NomArticle;
            strcat(reponse, "#");

            sprintf (temp, "%d", Qu); //panierEnCours[IndiceProchainArtPan].qu = qu;
                                      // On récupere du parametre de la fonction (l'User qui la donne)
            strcat(reponse, temp);
            strcat(reponse, "#");
    // Pq c'est pas bien présénté ? Car on essaie de la retenir !
                    strcpy(Prix, Tuple[2]); //panierEnCours[IndiceProchainArtPan].prix = prix;
                        string tmp4(Prix);
                        size_t x = tmp4.find(",");
                        if (x != string::npos) tmp4.replace(x,1,".");
        float monFloat = atof(Prix);
        monFloat = monFloat * 100;
        int monInt = (int) monFloat;
        char monPrix[10];

        sprintf (monPrix, "%d", monInt);

           strcat(reponse, monPrix);
           printf ("\n\n\n\n\n**************************L'API retourne au serveur : %s\n\n\n\n\n", reponse);

        }



    }


    mysql_close(connexion);

}

void SupprimeArticle(int art, int Qu)
{
    connection();

    sprintf(requete, "select * from UNIX_FINAL where id = %d", art);
    if (mysql_query(connexion, requete) != 0)
    {
        fprintf(stderr, "Erreur de Mysql-query");
        printf ("\nErreur 1 \n");
        fflush(stdout);
    }

    if ((resultat = mysql_store_result(connexion)) == NULL)
    {
        fprintf(stderr, "Erreur de mysql store");
        printf ("\nErreur 2 \n");
        fflush(stdout);
    }

    fflush(stdout);

    if ((Tuple = mysql_fetch_row(resultat)) != NULL)
    {

        qudispo = atoi(Tuple[3]);
        printf ("\nQu DIspo = %d", qudispo);


            sprintf(requete, "UPDATE UNIX_FINAL set stock = %d where id = %d", (qudispo + Qu), art);
            if (mysql_query(connexion, requete) != 0)
            {
                fprintf(stderr, "Erreur de Mysql-query");
                printf ("\nErreur 1 \n");
                fflush(stdout);
            }

    }


    mysql_close(connexion);
}


int NouvelleFacture(int idClient, float montant) {
    // Connection à MySQL
    connection();

    // Obtention de la date du jour
    time_t t;
    struct tm* tm_info;
    time(&t);
    tm_info = localtime(&t);

    char dateDuJour[20];  // Utilisez une taille suffisante pour stocker la date
    strftime(dateDuJour, sizeof(dateDuJour), "%Y-%m-%d", tm_info);

    // Insertion de la nouvelle facture dans la table FACTURE
    char requete[512];
    sprintf(requete, "INSERT INTO FACTURE (IdClient, Montant, Date) VALUES (%d, %f, '%s');", idClient, montant, dateDuJour);

    // Affichage de la requête (à des fins de débogage)
    printf("Requête SQL : %s\n", requete);

    // Exécution de la requête
    if (mysql_query(connexion, requete) != 0) {
        fprintf(stderr, "Erreur lors de l'insertion : %s\n", mysql_error(connexion));
        // Gérer l'erreur ici si nécessaire
    } else {
        printf("Insertion réussie.\n");
    }

    // Récupération de l'ID de la nouvelle facture
    int idNouvelleFacture = mysql_insert_id(connexion);

    // Déconnexion de la BD
    mysql_close(connexion);

    return idNouvelleFacture;
}


void NouvelleVente(int idFacture, int idArticle, int quantite)
{
    connection();

    // Insérer la nouvelle vente dans la table VENTES
    char requete[512];
    sprintf(requete, "INSERT INTO VENTES (idFacture, idArticle, quantite) VALUES (%d, %d, %d)", idFacture, idArticle, quantite);

    if (mysql_query(connexion, requete) != 0)
    {
        fprintf(stderr, "Erreur lors de l'insertion de la vente : %s\n", mysql_error(connexion));
        fprintf(stderr, "Requête d'insertion : %s\n", requete);
        mysql_close(connexion);
        return;  // Gérer l'erreur ici si nécessaire
    }
    else
    {
        printf("Insertion de la vente réussie.\n");
    }

    // Afficher le contenu de la table VENTES
    printf("Contenu de la table VENTES après l'insertion :\n");

    sprintf(requete, "SELECT * FROM VENTES");
    if (mysql_query(connexion, requete) != 0)
    {
        fprintf(stderr, "Erreur de Mysql-query : %s\n", mysql_error(connexion));
        mysql_close(connexion);
        return;  // Gérer l'erreur ici si nécessaire
    }

    MYSQL_RES *resultat = mysql_store_result(connexion);
    if (resultat == NULL)
    {
        fprintf(stderr, "Erreur lors de la récupération du résultat : %s\n", mysql_error(connexion));
        mysql_close(connexion);
        return;  // Gérer l'erreur ici si nécessaire
    }

    MYSQL_ROW ligne;
    while ((ligne = mysql_fetch_row(resultat)) != NULL)
    {
        printf("idFacture: %s, idArticle: %s, quantite: %s\n", ligne[0], ligne[1], ligne[2]);
    }

    mysql_free_result(resultat);

    // Déconnexion de la BD
    mysql_close(connexion);
}