#include "OVESP.h"

int estOk = 0;

bool OVESP(char* requete, char* reponse, int socket)
{

	printf("\n\n   <<<< J'ouvre OVESP>>>>>\n");
// ***** Récupération nom de la requete *****************
	 char *savePtr;
	 char *ptr = strtok_r(requete,"#", &savePtr); 
	 printf ("apres le 1er il reste : %s", savePtr);

	 // ***** LOGIN ******************************************
	 if (strcmp(ptr,"LOGIN") == 0)
	 {

	 	printf ("\t\t\t IF LOGIN \n");
	 	printf ("Ma requete : %s", requete);
 		// On récupere le login et mdp (attention 20 caracteres sur la BD)
 	//char user[1050], password[1050], nouveau[1050];
 	char* user = (char*) malloc(1500*sizeof(char*));
 	char* password = (char*) malloc(1500*sizeof(char*));
 	char* nouveau = (char*) malloc(4*sizeof(char*));

 	ptr = strtok_r(NULL,"#", &savePtr);
 	printf ("\nUser AVANT COPIE : %s\n", ptr);
	strcpy(user,ptr);
	printf ("User APRES copie : %s\n", user);
	ptr = strtok_r(NULL,"#", &savePtr);
	strcpy(password, ptr);
	ptr = strtok_r(NULL,"#", &savePtr);
	strcpy(nouveau, ptr);
	//printf("\t[THREAD %p] LOGIN de %s\n",pthread_self(),user);

		// Interroger la BD et préparer réponse pour le CLIENT
		
printf ("Login : %s, password : %s, nouveau %s", user, password, nouveau);

		estOk = VerifClient(user, password, nouveau); //Contient le numéro client ou 0.
		printf ("\nEst ok = %d", estOk);
		sprintf(reponse,"LOGIN2#%d", estOk);

		free(user);
		free(password);
		free(nouveau);
		
		if (estOk == 0)
		{
			return false;
		}
		else
		{
			return true;
		}

	}


		 // ***** LOGIN - REP CLIENT ******************************************
	 if (strcmp(ptr,"LOGIN2") == 0)
	 {
	 	printf ("\t\t\t IF LOGIN2 \n");
	 	char resultat_login[1498];
	 	strcpy(resultat_login,strtok_r(NULL,"#", &savePtr)); //Contient 1 ou 0. -> ModifiER avec num cli apres.
	 	if (strcmp(resultat_login, "0") == 0)
	 	{
	 		return false;
	 	}
	 	else
	 	{
	 		return true;
	 	}
	 }

	 if(strcmp(ptr, "ARTSUIVANT") == 0)
	 {

	 	char* artEncours = strtok_r(NULL,"#", &savePtr);
	 		printf ("Je suis dans article suivant");	
	 		printf ("art EnCours = QQQ%sQQQ", artEncours);		


	 		int a = (atoi(artEncours)+1);
	 		printf ("\n a = %d \n", a);

	 		ArticleSuivant(a, reponse);
	 		return true;
	 		
	}

	 	if ((strcmp(ptr, "ARTSUIVANT2")) == 0)
	 	{

	 		ptr = strtok_r(NULL,"#", &savePtr); // CA corresponde à 0 OU à ID ARTICLES

	 		if((strcmp(ptr, "0")) == 0)
	 		{
	 			return false;
	 		}
	 		else
	 		{
				printf("\nje suis biern dans le else\n");
				char* manouvellereponse = (char*)malloc(1500 * sizeof(char));
				char* champ = strtok_r(NULL, "#", &savePtr); // Je chope l'id qui me sert à rien.
				strncpy(manouvellereponse, champ, 1499);  // Utilise strncpy pour éviter le débordement
				manouvellereponse[1499] = '\0';  // Assure-toi que la chaîne est terminée par '\0'
				strcat(manouvellereponse, "#");
				champ = strtok_r(NULL, "#", &savePtr); // Je chope l'element 1
				strncat(manouvellereponse, champ, 1499 - strlen(manouvellereponse));  // Utilise strncat pour éviter le débordement
				strcat(manouvellereponse, "#");
				champ = strtok_r(NULL, "#", &savePtr); // Je chope l'element 2
				strncat(manouvellereponse, champ, 1499 - strlen(manouvellereponse));  // Utilise strncat pour éviter le débordement
				strcat(manouvellereponse, "#");
				champ = strtok_r(NULL, "#", &savePtr); // Je chope l'element 3
				strncat(manouvellereponse, champ, 1499 - strlen(manouvellereponse));  // Utilise strncat pour éviter le débordement

				printf("\nJe vais retourner : %s", manouvellereponse);
				strcpy(reponse, manouvellereponse);

				// Libération de la mémoire allouée
				free(manouvellereponse);

	 			return true;
	 		}

	 	}




	 if (strcmp(ptr, "ACHAT") == 0)
	 {
	 	char* art = strtok_r(NULL, "#", &savePtr);
	 	char* qu = strtok_r(NULL, "#", &savePtr);
	 	
	 	int art1 = atoi(art);
		int qu1 = atoi(qu);
		printf ("\nOVESP JE VAIS APPELER ACHATARTICLE\n");
		AchatArticleAPI(qu1, art1, reponse);
		printf ("\nOVESP A COMME REPONSE %s\n\n", reponse);

	 	return true;
	 }

	 if (strcmp(ptr, "ACHAT2") == 0)
	 {
	 	printf ("\nCAS ACHAT2 \n");
	 	char estjuste[1500];
	 	//char* jelis;
	 	int i = 0;

	 	printf ("JUSTE AVANT STR\n");
	 	char* art = strtok_r(NULL, "#", &savePtr);
	 	

	 	if (strcmp(art, "KO") == 0)
	 	{
	 		printf ("FAUX");
			i = 0;
			return false;
	 	}
	 	else
	 	{
	 		
	 		strcpy(reponse,savePtr);
	 		printf ("VRAI %s\n", reponse);
			return true;
	 	}

	}

	if(strcmp(ptr, "VIDERPANIER") == 0)
	{
	 	strcpy(reponse, "VIDERPANIER");
	 	return true;
	}
	 

	 else if (strcmp(ptr, "LOGOUT") == 0)
	 {
	 	strcpy(reponse, "LOGOUT2");
	 	return true;
	 }

	 else if(strcmp(ptr, "SUPPRESSION") == 0)
	 {
	 	strcpy(reponse, "SUPPRESSION2#");
	 	strcat(reponse, strtok_r(NULL,"#", &savePtr)); // On ajoute le num article.
	 	return true;
	 }

	else if(strcmp(ptr, "SUPPOK") == 0)
	 {
	 	strcpy(reponse, savePtr);
	 	return true;
	 }

	 else if (strcmp(ptr, "VALIDEPANIER") == 0)
	 {
	 	strcpy(reponse,"VALIDEPANIER2");
	 	return true;
	 }

	 else if (strcmp(ptr, "VALIDEPANIER2") == 0)
	 {
		if (strcmp(strtok_r(NULL, "#", &savePtr), "OK") == 0)
		{
			return true;
		}
		else
		{
			return false;
		}
	 }
	 else
	 {
	 	printf ("\n\n\n\n\nQUELLE MERDE\n\n\n\n\n");
	 }

	 return false;
 }



