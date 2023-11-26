#include "TCP.h"
#include <stdio.h>
#include <string.h>

int main() {
    int serverSocket = ServerSocket(50406);

    if (serverSocket == -1) {
        fprintf(stderr, "Erreur lors de la création du serveur\n");
        return 1;
    }

    printf("Attente de la connexion du client...\n");

    char clientIP[INET_ADDRSTRLEN];
    int clientSocket = Accept(serverSocket, clientIP);

    if (clientSocket == -1) {
        fprintf(stderr, "Erreur lors de l'acceptation de la connexion\n");
        return 1;
    }

    printf("Client connecté depuis l'adresse IP : %s\n", clientIP);

    // Réception du message du client
    char receivedData[TAILLE_MAX_DATA];
    int bytesRead = Receive(clientSocket, receivedData);

    if (bytesRead == -1) {
        fprintf(stderr, "Erreur lors de la réception des données\n");
    } else {
        printf("Message du client : %s\n", receivedData);

        // Ajout pour afficher "Je dis hello toi meme"
        printf("Je dis hello toi meme\n");
    }

    // Envoi de la réponse au client
    char response[] = "J'ai recu Hello";
    Send(clientSocket, response, sizeof(response) - 1);  // Utiliser sizeof pour inclure le caractère null

    // Fermeture des sockets
    close(clientSocket);
    close(serverSocket);

    return 0;
}
