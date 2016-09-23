package tcp;

import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;

/**
 * TCP Server Connection listens on a port on the host machine.
 *
 * An instance of this class will listen on the provided port number on localhost.
 * Three threads are created by this class:
 * - Thread to queue objects and send them in FIFO order
 * - Thread to receive objects
 * - Thread to send a ping object every X seconds to make sure the connection is still working
 */
public class TCPServerConnection implements Runnable {
    private Logger logger = LogManager.getFormatterLogger(getClass());

    private int port;
    private final TCPSender tcpSender;
    private final TCPReceiver tcpReceiver;
    private final TCPPing tcpPing;
    private static final int LISTEN_WAIT = 5000;
    private boolean closeConnection = false;
    private ServerSocket serverSocket;
    private Socket clientSocket;
    private String name;

    public TCPServerConnection(String name, int port) {
        this.port = port;
        this.name = name;
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
     * Close socket connection without listening again
     */
    public void closeAndStop() {
        logger.info("Stopping server forever");
        closeConnection = true;
        try {
            if(clientSocket != null && !clientSocket.isClosed()) {
                clientSocket.close();
            }
        } catch (IOException e) {
            logger.error(e);
        }

        try {
            if(serverSocket != null && !serverSocket.isClosed()) {
                serverSocket.close();
            }
            logger.info("Server stopped");
        } catch (IOException e) {
            logger.error(e);
        }
    }

    /**
     * Accept a client connection
     */
    private void acceptClient() {
        try {
            clientSocket = serverSocket.accept();
            logger.info("Connected to a client successfully!");

            // Update sender, receiver and ping
            tcpSender.setSocket(clientSocket);
            tcpReceiver.setSocket(clientSocket);
            tcpPing.setSocket(clientSocket);

            // Start TCP Sender
            Thread sendThread = new Thread(tcpSender);
            sendThread.setName(name + "-Server-Sender");
            sendThread.start();

            // Start TCP Receiver
            Thread receiveThread = new Thread(tcpReceiver);
            receiveThread.setName(name + "-Server-Receiver");
            receiveThread.start();

            // Start pinging the client
            Thread pingThread = new Thread(tcpPing);
            pingThread.setName(name + "-Server-Ping");
            pingThread.start();

            // Join threads before accepting another client
            sendThread.join();
            receiveThread.join();
            pingThread.join();

        } catch (IOException e) {
            logger.error(e);
            logger.info("Error connecting to a client. Server is still listening ...");
        } catch (InterruptedException e) {
            logger.error(e);
            Thread.currentThread().interrupt();
        }
    }

    /**
     * Listen and wait for a client to connect
     */
    @Override
    public void run() {

        // Keep listening on the given port
        while (!closeConnection) {
            try {
                serverSocket = new ServerSocket(port);
                logger.info("Server started and listening on port %d ...", port);

                // Keep accepting clients
                while (!serverSocket.isClosed()) {
                    logger.info("Waiting for a client to connect ...");
                    acceptClient();
                }
            } catch (IOException e) {
                logger.error(e);
                logger.info("Server stopped listening. Will try listening again in %d ms", LISTEN_WAIT);

                try {
                    Thread.sleep(LISTEN_WAIT);
                } catch (InterruptedException ee) {
                    logger.error(ee);
                    Thread.currentThread().interrupt();
                }
            } finally {
                try {
                    logger.info("Closing server socket");
                    if(serverSocket != null && !serverSocket.isClosed()) {
                        serverSocket.close();
                    }
                } catch (IOException e) {
                    logger.error(e);
                }
            }
        }
    }
}
