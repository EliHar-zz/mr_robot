package tcp;

import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

import java.io.IOException;
import java.net.Socket;

/**
 * Client Connection establishes a TCP connection with a server.
 *
 * An instance of this class will connect to the server on the provided ip address and port number.
 * Three threads are created by this class:
 * - Thread to queue objects and send them in FIFO order
 * - Thread to receive objects
 * - Thread to send a ping object every X seconds to make sure the connection is still working
 */
public class TCPClientConnection implements Runnable {
    private Logger logger = LogManager.getFormatterLogger(getClass());

    private String destIpAddress;
    private int destPort;
    private final TCPSender tcpSender;
    private final TCPReceiver tcpReceiver;
    private final TCPPing tcpPing;
    private static final int CONNECT_WAIT = 5000;
    private boolean closeConnection = false;
    private Socket clientSocket;
    private String name;

    public TCPClientConnection(String name, String destIpAddress, int destPort) {
        this.name = name;
        this.destIpAddress = destIpAddress;
        this.destPort = destPort;
        tcpSender = new TCPSender();
        tcpReceiver = new TCPReceiver();
        tcpPing = new TCPPing(tcpSender);
    }

    /**
     * Get TCP receiver instance
     * @return tcp receiver instance
     */
    public TCPReceiver getTcpReceiver() {
        return tcpReceiver;
    }

    /**
     * Get TCP sender instance
     * @return tcp sender instance
     */
    public TCPSender getTcpSender() {
        return tcpSender;
    }

    /**
     * Close socket connection without try to connect again
     */
    public void closeAndStop() {
        logger.info("Stopping connection from client and will never try to connect again");
        closeConnection = true;
        try {
            if(clientSocket != null && !clientSocket.isClosed()) {
                clientSocket.close();
            }
            logger.info("Connection stopped");
        } catch (IOException e) {
            logger.error(e);
        }
    }

    @Override
    public void run() {

        // Keep trying to connect
        while (!closeConnection) {
            logger.info("Trying to connect to %s at port %d ...", destIpAddress, destPort);
            try {
                clientSocket= new Socket(destIpAddress, destPort);
                logger.info("Connected to the server successfully!");

                // Update sender, receiver and ping
                tcpSender.setSocket(clientSocket);
                tcpReceiver.setSocket(clientSocket);
                tcpPing.setSocket(clientSocket);

                // Start TCP Sender
                Thread sendThread = new Thread(tcpSender);
                sendThread.setName(name + "-Client-Sender");
                sendThread.start();

                // Start TCP Receiver
                Thread receiveThread = new Thread(tcpReceiver);
                receiveThread.setName(name + "-Client-Receiver");
                receiveThread.start();

                // Start pinging the server
                Thread pingThread = new Thread(tcpPing);
                pingThread.setName(name + "-Client-Ping");
                pingThread.start();

                // Join threads before closing the socket
                sendThread.join();
                receiveThread.join();
                pingThread.join();

            } catch (IOException e) {
                logger.error(e);

                try {
                    logger.info("Waiting for %d ms before trying again ...", CONNECT_WAIT);
                    Thread.sleep(CONNECT_WAIT);
                } catch (InterruptedException ee) {
                    logger.error(ee);
                    Thread.currentThread().interrupt();
                }

            } catch (InterruptedException e) {
                logger.error(e);
                Thread.currentThread().interrupt();
            } finally {
                try {
                    if(clientSocket != null && !clientSocket.isClosed()) {
                        clientSocket.close();
                    }
                } catch (IOException e) {
                    logger.error(e);
                }
            }
        }
    }
}
