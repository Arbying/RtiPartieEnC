#include <stdio.h>
#include <stdlib.h>
#include <mysql.h>
#include <time.h>
#include <string.h>
#include "protocole.h"

ARTICLE Elm[] = 
{
  {-1,"carottes",2.16f,9,"carottes.jpg"},
  {-1,"cerises",9.75f,8,"cerises.jpg"},
  {-1,"artichaut",1.62f,15,"artichaut.jpg"},
  {-1,"bananes",2.6f,8,"bananes.jpg"},
  {-1,"champignons",10.25f,4,"champignons.jpg"},
  {-1,"concombre",1.17f,5,"concombre.jpg"},
  {-1,"courgette",1.17f,14,"courgette.jpg"},
  {-1,"haricots",10.82f,7,"haricots.jpg"},
  {-1,"laitue",1.62f,10,"laitue.jpg"},
  {-1,"oranges",3.78f,23,"oranges.jpg"},
  {-1,"oignons",2.12f,4,"oignons.jpg"},
  {-1,"nectarines",10.38f,6,"nectarines.jpg"},
  {-1,"peches",8.48f,11,"peches.jpg"},
  {-1,"poivron",1.29f,13,"poivron.jpg"},
  {-1,"pommes de terre",2.17f,25,"pommesDeTerre.jpg"},
  {-1,"pommes",4.00f,26,"pommes.jpg"},
  {-1,"citrons",4.44f,11,"citrons.jpg"},
  {-1,"ail",1.08f,14,"ail.jpg"},
  {-1,"aubergine",1.62f,17,"aubergine.jpg"},
  {-1,"echalotes",6.48f,13,"echalotes.jpg"},
  {-1,"tomates",5.49f,22,"tomates.jpg"}
};

int main(int argc,char *argv[])
{
  // Connection a MySql
  printf("Connection a la BD...\n");
  MYSQL* connexion = mysql_init(NULL);
  mysql_real_connect(connexion,"localhost","Student","PassStudent1_","PourStudent",0,0,0);

  // Creation d'une table UNIX_FINAL
  printf("Creation de la table UNIX_FINAL...\n");
  mysql_query(connexion,"drop table UNIX_FINAL;"); // au cas ou elle existerait deja
  mysql_query(connexion,"create table UNIX_FINAL (id INT(4) auto_increment primary key, intitule varchar(20),prix FLOAT(4),stock INT(4),image varchar(20));");


  // Creation de la table CLIENTS
  printf("Creation de la table CLIENTS...\n");
  mysql_query(connexion,"drop table CLIENTS;"); // au cas ou elle existerait deja
  mysql_query(connexion,"create table CLIENTS (id INT(4) auto_increment primary key, nom varchar(20), mdp varchar(20));");



    // Création de la table FACTURE
    printf("Création de la table FACTURES avec le champ Date en VARCHAR...\n");
    mysql_query(connexion, "drop table FACTURES;");
    mysql_query(connexion, "create table FACTURE (id INT(4) auto_increment primary key, IdClient INT(4), Montant FLOAT(4), Paye TINYINT(1) default 0, Date VARCHAR(20));");

    // Création de la table VENTES
    printf("Création de la table VENTES...\n");
    mysql_query(connexion, "drop table VENTES;");
    mysql_query(connexion, "create table VENTES (idFacture INT(4), idArticle INT(4), quantite INT(4));");


  // Ajout de tuples dans la table UNIX_FINAL
  printf("Ajout de 21 articles la table UNIX_FINAL...\n");
  char requete[256];
  for (int i=0 ; i<21 ; i++)
  {
    sprintf(requete,"insert into UNIX_FINAL values (NULL,'%s',%f,%d,'%s');",Elm[i].intitule,Elm[i].prix,Elm[i].stock,Elm[i].image);
    mysql_query(connexion,requete);
  }

 printf("Liste des tables :\n");
  MYSQL_RES *result = mysql_list_tables(connexion, NULL);
  MYSQL_ROW row;
  while ((row = mysql_fetch_row(result)) != NULL)
  {
    printf("%s\n", row[0]);
  }

  // RTI PARTIE 2 : 
  // Création de la table FonctionsTrav
printf("Création de la table FonctionsTrav...\n");
mysql_query(connexion, "drop table if exists FonctionsTrav;"); // au cas où elle existerait déjà
mysql_query(connexion, "create table FonctionsTrav (Type INT primary key, Fonction varchar(50));");

// Ajout des fonctions par défaut dans la table FonctionsTrav
printf("Ajout des fonctions par défaut dans la table FonctionsTrav...\n");
mysql_query(connexion, "insert into FonctionsTrav values (1, 'Gerant');");
mysql_query(connexion, "insert into FonctionsTrav values (2, 'Caissier');");
mysql_query(connexion, "insert into FonctionsTrav values (3, 'Reassortisseur');");

// Création de la table Employes
printf("Création de la table Employes...\n");
mysql_query(connexion, "drop table if exists Employes;"); // au cas où elle existerait déjà
mysql_query(connexion, "create table Employes (IdEmploye INT primary key, Nom varchar(50), MDP varchar(50), Type INT, Actif varchar(10), foreign key (Type) references FonctionsTrav(Type));");

// Ajout des employés dans la table Employes
printf("Ajout des employés dans la table Employes...\n");
mysql_query(connexion, "insert into Employes values (1, 'Wagner', '1111', 1, 'oui');");
mysql_query(connexion, "insert into Employes values (2, 'Caprasse', '1111', 1, 'oui');");
mysql_query(connexion, "insert into Employes values (3, 'Arber', '1111', 2, 'oui');");
mysql_query(connexion, "insert into Employes values (4, 'Seb', '11111', 2, 'oui');");
mysql_query(connexion, "insert into Employes values (5, 'Pimousse', '1111', 2, 'conge');");
mysql_query(connexion, "insert into Employes values (6, 'Marie', '1111', 2, 'non');");



  // Deconnection de la BD
  mysql_close(connexion);
  exit(0);
}
