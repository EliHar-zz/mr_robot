package host;

import tcp.TCPServerConnection;

/**
 * Create a server on the host machine
 */
public class Host {
    public void startServer() {
        TCPServerConnection tcpServerConnection = new TCPServerConnection("Host", 5000);
        Thread thread = new Thread(tcpServerConnection);
        thread.start();
    }
}
