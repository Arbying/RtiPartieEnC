
package Protocoles;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.InetAddress;
import java.net.ServerSocket;
import java.net.Socket;

public class TCP {

        // Ne nous sert à rien mais on l'a préparée pour le jour ou on fait un serveur en Java
    public static ServerSocket createServerSocket(int port) {
        try {
            return new ServerSocket(port);
        } catch (IOException e) {
            e.printStackTrace();
            return null;
        }
    }

    public static Socket acceptConnection(ServerSocket serverSocket) {
        try {
            return serverSocket.accept();
        } catch (IOException e) {
            e.printStackTrace();
            return null;
        }
    }

    public static Socket createClientSocket(String serverIP, int serverPort) {
        try {
            InetAddress serverAddress = InetAddress.getByName(serverIP);
            return new Socket(serverAddress, serverPort);
        } catch (IOException e) {
            e.printStackTrace();
            return null;
        }
    }

    public static int send(Socket socket, byte[] data, int size) {
        try {
            OutputStream outputStream = socket.getOutputStream();
            outputStream.write(data, 0, size);
            outputStream.flush();
            return size;
        } catch (IOException e) {
            e.printStackTrace();
            return -1;
        }
    }

    public static int receive(Socket socket, byte[] data) {
        try {
            InputStream inputStream = socket.getInputStream();
            int bytesRead;
            int totalBytesRead = 0;

            while (totalBytesRead < data.length) {
                bytesRead = inputStream.read(data, totalBytesRead, data.length - totalBytesRead);
                if (bytesRead == -1) {
                    return totalBytesRead;
                }
                totalBytesRead += bytesRead;
            }

            return totalBytesRead;
        } catch (IOException e) {
            e.printStackTrace();
            return -1;
        }
    }
}
