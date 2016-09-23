package board;

import tcp.TCPClientConnection;

/**
 * Connect client on the board machine
 */
public class Board {
    public static void main(String[] args) {
        TCPClientConnection tcpServerConnection = new TCPClientConnection("Board", "127.0.0.1", 5000);
        Thread thread = new Thread(tcpServerConnection);
        thread.start();
    }
}
