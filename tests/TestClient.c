#include "TCP.h"
#include <stdio.h>
#include <string.h>

int main() {
    // Adresse IP du serveur (dans ce cas, localhost)
    char serverIP[] = "127.0.0.1";
    int port = 50406;

    // Création du socket client et connexion au serveur
    int clientSocket = ClientSocket(serverIP, port);

    if (clientSocket == -1) {
        fprintf(stderr, "Erreur lors de la connexion au serveur\n");
        return 1;
    }

    printf("Connecté au serveur sur le port %d\n", port);

    // Réception du message du serveur
    char receivedData[TAILLE_MAX_DATA];
    int bytesRead = Receive(clientSocket, receivedData);

    if (bytesRead == -1) {
        fprintf(stderr, "Erreur lors de la réception des données\n");
    } else {
        printf("Message du serveur : %s\n", receivedData);

        // Ajout pour afficher "Il m'a dit Hello toi meme"
        printf("Il m'a dit Hello toi meme\n");
    }

    // Envoi d'une nouvelle réponse au serveur
    char response[] = "Hello toi meme";
    Send(clientSocket, response, strlen(response));

    // Réception de la deuxième réponse du serveur
    bytesRead = Receive(clientSocket, receivedData);

    if (bytesRead == -1) {
        fprintf(stderr, "Erreur lors de la réception des données\n");
    } else {
        printf("Deuxième message du serveur : %s\n", receivedData);
    }

    // Fermeture du socket client
    close(clientSocket);

    return 0;
}
