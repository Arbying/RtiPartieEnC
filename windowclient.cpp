#include "protocoles/OVESP.h"
#include "protocoles/TCP.h"
#include "windowclient.h"
#include "ui_windowclient.h"
#include <QMessageBox>
#include <string>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <cstring>
using namespace std;


int sClient;


extern WindowClient *w;

#define REPERTOIRE_IMAGES "images/"

int articleEnCours = 0;
char nomARticle[50];
float monTotal = 0;

char* reponse = (char*) malloc(1500*sizeof(char));

WindowClient::WindowClient(QWidget *parent) : QMainWindow(parent), ui(new Ui::WindowClient)
{
    ui->setupUi(this);

    // Configuration de la table du panier (ne pas modifer)
    ui->tableWidgetPanier->setColumnCount(3);
    ui->tableWidgetPanier->setRowCount(0);
    QStringList labelsTablePanier;
    labelsTablePanier << "Article" << "Prix à l'unité" << "Quantité";
    ui->tableWidgetPanier->setHorizontalHeaderLabels(labelsTablePanier);
    ui->tableWidgetPanier->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidgetPanier->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidgetPanier->horizontalHeader()->setVisible(true);
    ui->tableWidgetPanier->horizontalHeader()->setDefaultSectionSize(160);
    ui->tableWidgetPanier->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidgetPanier->verticalHeader()->setVisible(false);
    ui->tableWidgetPanier->horizontalHeader()->setStyleSheet("background-color: lightyellow");

    ui->pushButtonPayer->setText("Confirmer achat");
    setPublicite("!!! Magasin test Sebarby V 2.406 !!!");

    // Exemples à supprimer
    /*setArticle("pommes",5.53,18,"pommes.jpg");
    ajouteArticleTablePanier("cerises",8.96,2);*/
}

WindowClient::~WindowClient()
{
    delete ui;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///// Fonctions utiles : ne pas modifier /////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void WindowClient::setNom(const char* Text)
{
  if (strlen(Text) == 0 )
  {
    ui->lineEditNom->clear();
    return;
  }
  ui->lineEditNom->setText(Text);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
const char* WindowClient::getNom()
{
  strcpy(nom,ui->lineEditNom->text().toStdString().c_str());
  return nom;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void WindowClient::setMotDePasse(const char* Text)
{
  if (strlen(Text) == 0 )
  {
    ui->lineEditMotDePasse->clear();
    return;
  }
  ui->lineEditMotDePasse->setText(Text);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
const char* WindowClient::getMotDePasse()
{
  strcpy(motDePasse,ui->lineEditMotDePasse->text().toStdString().c_str());
  return motDePasse;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void WindowClient::setPublicite(const char* Text)
{
  if (strlen(Text) == 0 )
  {
    ui->lineEditPublicite->clear();
    return;
  }
  ui->lineEditPublicite->setText(Text);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void WindowClient::setImage(const char* image)
{
  // Met à jour l'image
  char cheminComplet[80];
  sprintf(cheminComplet,"%s%s",REPERTOIRE_IMAGES,image);
  QLabel* label = new QLabel();
  label->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
  label->setScaledContents(true);
  QPixmap *pixmap_img = new QPixmap(cheminComplet);
  label->setPixmap(*pixmap_img);
  label->resize(label->pixmap()->size());
  ui->scrollArea->setWidget(label);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
int WindowClient::isNouveauClientChecked()
{
  if (ui->checkBoxNouveauClient->isChecked()) return 1;
  return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void WindowClient::setArticle(const char* intitule,float prix,int stock,const char* image)
{
  ui->lineEditArticle->setText(intitule);
  if (prix >= 0.0)
  {
    char Prix[20];
    sprintf(Prix,"%.2f",prix);
    ui->lineEditPrixUnitaire->setText(Prix);
  }
  else ui->lineEditPrixUnitaire->clear();
  if (stock >= 0)
  {
    char Stock[20];
    sprintf(Stock,"%d",stock);
    ui->lineEditStock->setText(Stock);
  }
  else ui->lineEditStock->clear();
  setImage(image);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
int WindowClient::getQuantite()
{
  return ui->spinBoxQuantite->value();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void WindowClient::setTotal(float total)
{
  if (total >= 0.0)
  {
    char Total[20];
    sprintf(Total,"%.2f",total);
    ui->lineEditTotal->setText(Total);
  }
  else ui->lineEditTotal->clear();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void WindowClient::loginOK()
{
  ui->pushButtonLogin->setEnabled(false);
  ui->pushButtonLogout->setEnabled(true);
  ui->lineEditNom->setReadOnly(true);
  ui->lineEditMotDePasse->setReadOnly(true);
  ui->checkBoxNouveauClient->setEnabled(false);

  ui->spinBoxQuantite->setEnabled(true);
  ui->pushButtonPrecedent->setEnabled(true);
  ui->pushButtonSuivant->setEnabled(true);
  ui->pushButtonAcheter->setEnabled(true);
  ui->pushButtonSupprimer->setEnabled(true);
  ui->pushButtonViderPanier->setEnabled(true);
  ui->pushButtonPayer->setEnabled(true);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void WindowClient::logoutOK()
{
  ui->pushButtonLogin->setEnabled(true);
  ui->pushButtonLogout->setEnabled(false);
  ui->lineEditNom->setReadOnly(false);
  ui->lineEditMotDePasse->setReadOnly(false);
  ui->checkBoxNouveauClient->setEnabled(true);

  ui->spinBoxQuantite->setEnabled(false);
  ui->pushButtonPrecedent->setEnabled(false);
  ui->pushButtonSuivant->setEnabled(false);
  ui->pushButtonAcheter->setEnabled(false);
  ui->pushButtonSupprimer->setEnabled(false);
  ui->pushButtonViderPanier->setEnabled(false);
  ui->pushButtonPayer->setEnabled(false);

  setNom("");
  setMotDePasse("");
  ui->checkBoxNouveauClient->setCheckState(Qt::CheckState::Unchecked);

  setArticle("",-1.0,-1,"");

  w->videTablePanier();
  w->setTotal(-1.0);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///// Fonctions utiles Table du panier (ne pas modifier) /////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void WindowClient::ajouteArticleTablePanier(const char* article,float prix,int quantite)
{
    char Prix[20],Quantite[20];

    sprintf(Prix,"%.2f",prix);
    sprintf(Quantite,"%d",quantite);

    // Ajout possible
    int nbLignes = ui->tableWidgetPanier->rowCount();
    nbLignes++;
    ui->tableWidgetPanier->setRowCount(nbLignes);
    ui->tableWidgetPanier->setRowHeight(nbLignes-1,10);

    QTableWidgetItem *item = new QTableWidgetItem;
    item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
    item->setTextAlignment(Qt::AlignCenter);
    item->setText(article);
    ui->tableWidgetPanier->setItem(nbLignes-1,0,item);

    item = new QTableWidgetItem;
    item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
    item->setTextAlignment(Qt::AlignCenter);
    item->setText(Prix);
    ui->tableWidgetPanier->setItem(nbLignes-1,1,item);

    item = new QTableWidgetItem;
    item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
    item->setTextAlignment(Qt::AlignCenter);
    item->setText(Quantite);
    ui->tableWidgetPanier->setItem(nbLignes-1,2,item);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void WindowClient::videTablePanier()
{
    ui->tableWidgetPanier->setRowCount(0);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
int WindowClient::getIndiceArticleSelectionne()
{
    QModelIndexList liste = ui->tableWidgetPanier->selectionModel()->selectedRows();
    if (liste.size() == 0) return -1;
    QModelIndex index = liste.at(0);
    int indice = index.row();
    return indice;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///// Fonctions permettant d'afficher des boites de dialogue (ne pas modifier ////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void WindowClient::dialogueMessage(const char* titre,const char* message)
{
   QMessageBox::information(this,titre,message);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void WindowClient::dialogueErreur(const char* titre,const char* message)
{
   QMessageBox::critical(this,titre,message);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////// CLIC SUR LA CROIX DE LA FENETRE /////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void WindowClient::closeEvent(QCloseEvent *event)
{
  on_pushButtonLogout_clicked();
  exit(0);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///// Fonctions clics sur les boutons ////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void WindowClient::on_pushButtonLogin_clicked()
{
      strcpy(nomARticle, "\0");
  
  //    1. créer un vecteur char
      char maRequete[1498];
      strcpy(maRequete,"LOGIN#");

  //    3. y ajouter l'user + #
      char login[20];
      strcpy (login, getNom());
      strcat(maRequete, login);
      strcat(maRequete, "#");

  //    4. y ajouter mdp + #
      char mdp[20];
      strcpy(mdp, getMotDePasse());
      strcat(maRequete, mdp);
      strcat(maRequete, "#");
  //    5. y ajouter New : 0 ou 1.
      if (isNouveauClientChecked() == 1)
      {
        strcat(maRequete, "1");
      }
      else
      {
        strcat(maRequete, "0");
      }


      // Connexion sur le serveur
      char ip[20];
    FILE *fp = fopen("IpServer.dat", "r");
    if (fp == NULL) {
        printf("Erreur lors de l'ouverture du fichier.\n");
        exit(1);
    }

    if (fscanf(fp, "%19s", ip) != 1) {
        printf("Erreur lors de la lecture du fichier.\n");
        fclose(fp); // Ferme le fichier avant de quitter
        exit(1);
    }

    fclose(fp); // Ferme le fichier après lecture


       sClient = ClientSocket(ip, 50406);
       if (sClient == -1)
       {
       perror("Erreur de ClientSocket");
       exit(1);
       }



  //    6. Envoyer sur le reseau vers le serveur (Send);

       Send(sClient, maRequete, strlen(maRequete));
       maRequete[0] = '\0';


  //    7. -> Se met en attente d'un paquet BLOQUANT

       Receive(sClient, maRequete);


  //    8. Recoit la réponse du serveur et appel OVESP*/
      if (OVESP(maRequete, reponse, sClient))
      {
        printf("J'appel LOGIN OK\n");
        loginOK();
        setTotal(monTotal);
        on_pushButtonSuivant_clicked();
      }
      else
      {
       
        dialogueMessage("Erreur de login", "Un peu de sérieux, met ton login comme il faut!");

      }

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void WindowClient::on_pushButtonLogout_clicked()
{

  char maRequete[1498]; // Merci le compilateur qui bouffe de la mémoire pour rien 
  strcpy(maRequete, "LOGOUT");

  //.1 Envoyer requete logout
  Send(sClient, maRequete, strlen(maRequete));
  maRequete[0] = '\0';
  articleEnCours = 0;


  videTablePanier(); // Vider le panier
  logoutOK(); // Change l'interface


 if (sClient > 0)
 {
    ::close(sClient);
 }


}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void WindowClient::on_pushButtonSuivant_clicked()
{
    // 1. Envoi d'une requete pour selectionner l'élement suivant (ARTSUIVANT#)
  //  qui contient char* num article en cours
    char maRequeteSuivant[1498];
    strcpy (maRequeteSuivant, "ARTSUIVANT#");
    char art[5]; // Nein besoin de plus, même ca c'est déjà fort large
    sprintf(art, "%d", articleEnCours);
    strcat(maRequeteSuivant, art);

    //envoi requete
    printf ("J'envoie %s", maRequeteSuivant);
    Send(sClient, maRequeteSuivant, strlen(maRequeteSuivant));
    

    //ATTEND REPONSE
    Receive(sClient, maRequeteSuivant);
    printf ("Je suis la et j'ai recu %s", maRequeteSuivant);



    bool resultat = OVESP(maRequeteSuivant, reponse, sClient);
    if (resultat == false)
    {
      dialogueErreur("bout de liste", "Vous êtes au bout du rouleau essayez l'autre fleche.");
    }
    else
    {
      printf ("Je vais decortiquer la chaine chez le client : \n");
      char* nom = strtok(reponse,"#");
      char* img = strtok(NULL,"#");

      char* pxtxt = strtok(NULL,"#");
      printf ("\n ------------- Le prix vaut : %s\n",  pxtxt);
     
      
      char* qutxt = strtok(NULL,"#");
      printf ("\n ------------- Le QU vaut : %s\n",  qutxt);
      int qu = atoi(pxtxt);

      int longeur = strlen(pxtxt);

     
     double prixdouble = atof(qutxt);
     float prix = (float) prixdouble;
     prix = prix / 100;

      printf("\n J'ai mon int qui vaut : %d et mon float qui vaut %f \n", qu, prix);


      setArticle(nom, prix, qu, img);
      articleEnCours = articleEnCours + 1;
      printf ("<<<<<<<<<<<<<<NOm : %s --------------", nom);

      
    if (strcmp(nomARticle, nom) == 0)
    {
      articleEnCours = articleEnCours -1;
      dialogueErreur("bout de liste", "Vous êtes au bout du rouleau essayez l'autre fleche.");
    }

    strcpy(nomARticle, nom);

    printf ("\n\n\n\n\n*************** nomArticle  = %s ********************\n", nomARticle);
    }




}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void WindowClient::on_pushButtonPrecedent_clicked()
{
    if (articleEnCours < 2)
    {
      dialogueErreur("Article", "Vous etes au début de la liste, essayez l'autre fleche :P");
    }
    else
    {
      articleEnCours = articleEnCours - 2;
      on_pushButtonSuivant_clicked();
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void WindowClient::on_pushButtonAcheter_clicked()
{

  int QuAAcheter = getQuantite();
  if (QuAAcheter < 1)
  {
    dialogueErreur("QuErreur", "tu ne sauras pas acheter moins d un. Logique non ? ");
  }
  else
  {
    char maRequeteAchat[1498];
    strcpy(maRequeteAchat, "ACHAT#");
    char qu[5];
    sprintf (qu, "%d", QuAAcheter);
    strcat (maRequeteAchat, qu);
    strcat (maRequeteAchat, "#");
    char monart[5];
    sprintf(monart, "%d", articleEnCours);
    strcat (maRequeteAchat, monart);

    printf ("POUR ACHETER J'envoi : %s", maRequeteAchat);

    // Envoi de la requete
    Send(sClient, maRequeteAchat, strlen(maRequeteAchat));

    //Attente de la réponse
    Receive(sClient, maRequeteAchat);
    printf ("\n---------\n---J'ai recu ceci : %s", maRequeteAchat);

    bool resultat = OVESP(maRequeteAchat, reponse, sClient);
    if (resultat == false)
    {
      dialogueErreur("Erreur achat", "Quantitée non disponible");
    }
    else
    {
      printf ("\n\nJe suis dans le else donc tout va bien.\n");
      printf ("reponse : %s\n", reponse);
      char* savePtr;
      char* nom = strtok_r(reponse, "#", &savePtr);
      char* qutxt = strtok_r(NULL, "#", &savePtr);
      int qu = atoi(qutxt);
      char* pxtxt = strtok_r(NULL, "#", &savePtr);
      int pxInt = atoi(pxtxt);
      float prixfinal = (float) pxInt;
      prixfinal = prixfinal / 100;
      ajouteArticleTablePanier(nom, prixfinal, qu);
      monTotal = monTotal + (float) qu * prixfinal;
      setTotal(monTotal);

    }
  }


}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void WindowClient::on_pushButtonSupprimer_clicked()
{
    if (getIndiceArticleSelectionne() == -1)
    {
      dialogueErreur("Selection", "Selectionner le bon article !!! Non mais grrrr :P");
    }
    else
    {
      //envoi requete SUPPRESSION
      char supp[1498];
      strcpy(supp, "SUPPRESSION#");
      char temp[1498];
      sprintf(temp, "%d", getIndiceArticleSelectionne());
      strcat(supp, temp);

      Send(sClient, supp, strlen(supp));

     printf ("\nJ'ai envoyé ceci au serveur : %s\n", supp);
      //Attente de la réponse
      Receive(sClient, supp);

      
      bool supok = OVESP(supp, reponse, sClient);
      printf ("\n---------------------------------------\n A TRAITER: %s\n", reponse);
      char* savePtr;
      char* ptr = strtok_r(reponse, "#", &savePtr);
      int n;

      if (ptr == NULL) {
          printf("Erreur de tokenization\n");
      }
      else
      {
          printf("Token : %s\n", ptr);

          char* data = (char*) malloc(50*sizeof(char));
         /* char* nom  = (char*) malloc(50*sizeof(char));
          char* qutxu = (char*) malloc(50*sizeof(char));
          char* pxtxt = (char*) malloc(50*sizeof(char));
          int qu;
          int pxInt;
          int prixfinal;*/

          if (data == NULL)
          {
              printf("Erreur d'allocation mémoire\n");
          }
          else
          {
              strcpy(data, ptr);
              n = atoi(data);
              printf("Données copiées dans data : %d\n", n);
              videTablePanier();
              monTotal = 0;

              for (int i = 0; i < n; i++)
              {
                char* nom = strtok_r(NULL, "#", &savePtr);
                char* pxtxt = strtok_r(NULL, "#", &savePtr);
                char* qutxt = strtok_r(NULL, "#", &savePtr);
                int qu = atoi(qutxt);
                int pxInt = atoi(pxtxt);
                float prixfinal = (float) pxInt;
                prixfinal = prixfinal / 100;
                ajouteArticleTablePanier(nom, prixfinal, qu);
                monTotal = monTotal + (float) qu * prixfinal;
                setTotal(monTotal);
              }

           

              free(data); 

          }
}





    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void WindowClient::on_pushButtonViderPanier_clicked()
{

  //. 1 Envoi la requete.
  char requete[1498];
  //char* reponse;

  strcpy(requete, "VIDERPANIER");
  Send(sClient, requete, strlen(requete));
  requete[0] = '\0';
  //. 2 Attend la réponse ??? 
    Receive(sClient, reponse);
    videTablePanier(); //. 3 Vide le panier
    monTotal = 0;
    setTotal(monTotal);



}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void WindowClient::on_pushButtonPayer_clicked()
{
  //. Envoi la requete
  char requete[1498];
  char reponse[1498];

  strcpy(requete, "VALIDEPANIER");
  Send(sClient, requete, strlen(requete));


  //. 2 Attend la réponse
  Receive(sClient, reponse);

  if (OVESP(requete, reponse, sClient))
  {
    videTablePanier();
    monTotal = 0;
    setTotal(monTotal);
    dialogueMessage("Confirmation achat", "Merci pour votre achat, n'oubliez pas de payer en ligne ou en point de vente - en cash c est mieux :p");
  }
  else
  {
    dialogueErreur ("Erreur commande", "Une erreur inconue de type 406 est survenue ! ");
  }
}
