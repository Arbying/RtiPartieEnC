#ifndef API_H
#define API_H



int VerifClient (char* login, char* mdp, char* data); //Return 1 OK, 2 KO.-> Apres num cli
void ArticleSuivant(int art, char* reponse); //Retourne sous forme de char* les infos de l'article suivant
char* ArticlePrecedent(int art); //Retourne sous forme de char* les infos de l'article suivant
void AchatArticleAPI (int art, int Qu, char* reponse); /*Retroune :
												0 ou 1 : ok pas ok;
												nom article;
												prix;
												quantite;
												idarticle; (pour le serveur);*/

void SupprimeArticle(int idArt, int Qu);

void connection();

int NouvelleFacture(int idClient, float montant);
void NouvelleVente(int idFacture, int idArticle, int quantite);


#endif