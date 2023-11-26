package Tests;

import Protocoles.*;

import java.net.Socket;

public class TestTCP {

    public static void main(String[] args) {
        String serverIP = "192.168.150.129";
        int serverPort = 50406;

        // Créer le socket client
        Socket clientSocket = TCP.createClientSocket(serverIP, serverPort);

        if (clientSocket != null) {
            System.out.println("Connexion au serveur réussie.");

            // Fermer le socket client
            try {
                clientSocket.close();
            } catch (Exception e) {
                e.printStackTrace();
            }
        } else {
            System.out.println("Échec de la connexion au serveur.");
        }
    }
}