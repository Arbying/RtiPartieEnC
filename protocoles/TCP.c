#include "TCP.h"
#include <netdb.h>




int ServerSocket(int port) {
    int serverSocket;
    struct addrinfo hints, *res, *p;
    char portString[10];

    // Convertissez le numéro de port en chaîne de caractères
    snprintf(portString, sizeof(portString), "%d", port);

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;      // IPv4
    hints.ai_socktype = SOCK_STREAM; // TCP
    hints.ai_flags = AI_PASSIVE;    // Utiliser l'adresse IP locale

    // Obtenez les informations d'adresse pour le serveur
    if (getaddrinfo(NULL, portString, &hints, &res) != 0) {
        perror("Erreur lors de l'obtention des informations d'adresse");
        return -1;
    }

    // Parcourez les résultats et essayez de créer un socket
    for (p = res; p != NULL; p = p->ai_next) {
        serverSocket = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (serverSocket == -1) {
            continue; // Échec de la création du socket, essayez le suivant
        }

        // Liez le socket à l'adresse et au port
        if (bind(serverSocket, p->ai_addr, p->ai_addrlen) == 0) {
            break; // Liaison réussie	
        }

        close(serverSocket);
    }

    freeaddrinfo(res);

    if (p == NULL) {
        perror("Erreur lors de la création du socket ou de la liaison");
        return -1;
    }

    // Mettez le socket en mode écoute
    if (listen(serverSocket, 5) == -1) {
        perror("Erreur lors de la mise en écoute du socket");
        close(serverSocket);
        return -1;
    }

    return serverSocket;
}


int Accept(int sEcoute, char *ipClient) {
    struct sockaddr_in clientAddr;
    socklen_t addrLen = sizeof(clientAddr);

    // Mettez le socket en mode écoute
    if (listen(sEcoute, 5) == -1) {
        perror("Erreur lors de la mise en écoute du socket");
        return -1;
    }

    // Acceptez une connexion entrante
    int clientSocket = accept(sEcoute, (struct sockaddr *)&clientAddr, &addrLen);
    if (clientSocket == -1) {
        perror("Erreur lors de l'acceptation de la connexion");
        return -1;
    }

    // Récupérez l'adresse IP du client
    if (ipClient != NULL) {
        char clientIP[INET_ADDRSTRLEN];
        if (inet_ntop(AF_INET, &(clientAddr.sin_addr), clientIP, sizeof(clientIP)) == NULL) {
            perror("Erreur lors de la récupération de l'adresse IP du client");
            close(clientSocket);
            return -1;
        }

        // Copiez l'adresse IP du client dans ipClient
        strncpy(ipClient, clientIP, INET_ADDRSTRLEN);
    }

    return clientSocket;
}


int ClientSocket(char* ipServeur, int portServeur) {
    int clientSocket;
    struct addrinfo hints, *serverInfo, *p;

    // Initialisez la structure hints pour getaddrinfo
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;      // Utilisez IPv4
    hints.ai_socktype = SOCK_STREAM; // Utilisez TCP

    // Obtenez les informations d'adresse du serveur

    char portString[10];
snprintf(portString, sizeof(portString), "%d", portServeur);

int status = getaddrinfo(ipServeur, portString, &hints, &serverInfo);

    if (status != 0) {
        fprintf(stderr, "Erreur lors de la récupération des informations d'adresse: %s\n", gai_strerror(status));
        return -1;
    }

    // Parcourez les résultats pour trouver une adresse utilisable
    for (p = serverInfo; p != NULL; p = p->ai_next) {
        // Créez le socket
        clientSocket = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (clientSocket == -1) {
            perror("Erreur lors de la création du socket");
            continue;
        }

        // Connectez-vous au serveur
        if (connect(clientSocket, p->ai_addr, p->ai_addrlen) == -1) {
            close(clientSocket);
            perror("Erreur lors de la connexion au serveur");
            continue;
        }

        // Si nous sommes ici, la connexion a réussi
        break;
    }

    // Libérez les ressources des informations d'adresse
    freeaddrinfo(serverInfo);

    if (p == NULL) {
        fprintf(stderr, "Impossible de se connecter au serveur\n");
        return -1;
    }

    return clientSocket;
}


int Send (int sSocket, char* data, int taille)
{
	if (taille > TAILLE_MAX_DATA)
		return -1;

	// Preparation de la charge utile
	char trame[TAILLE_MAX_DATA+2];
	memcpy(trame,data,taille);
	trame[taille] = '#';
	trame[taille+1] = ')';
    trame[taille+2] = '\0';

	// Ecriture sur la socket
    printf ("DANS LA FONCTION SEND : %s\n", trame);
	return write(sSocket, trame, taille+2)-2;
}




int Receive (int sSocket, char* data)
{
	bool fini = false;
	int nbLus, i = 0;
	char lu1, lu2;
printf ("DEBUT LECTURE \n");

	while(!fini)
	{
		if ((nbLus = read(sSocket, &lu1, 1)) == -1)
			return -1;
		if (nbLus == 0) return i;  //connexion fermee par le client
        //printf("(175) *%c*- ", lu1);
		if (lu1 == '#')
		{
            
			if ((nbLus = read(sSocket, &lu2, 1)) == -1)
				return -1;
			if (nbLus == 0) return i;  //connexion fermee par le client

			if (lu2 == ')')
            {
                data[i] = '\0';
                printf ("Les données sont : %s", data);
              fini = true;  
            } 
			else
			{
          //      printf("(186) *%c*- ", lu2);
				data[i] = lu1;
				data[i+1] = lu2;
				i += 2;
			}
		}
		else
		{
        //    printf("(194) *%c*- ", lu1);
			data[i] = lu1;
			i++;
		}
	}	
    printf ("\n   ---FIN DE LECTURE");

	return i;
}