package tcp;

import com.MRObject;
import data.PingMRObject;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

import java.net.Socket;

/**
 * This class pings the other side of the connection to make sure that the connection is still working.
 * A ping is sent using a TCPSender instance which treats it as a normal message to be delivered.
 */
public class TCPPing implements Runnable {
    private Logger logger = LogManager.getFormatterLogger(getClass());

    private Socket socket;
    private TCPSender tcpSender;
    private static final int PING_WAIT = 10000;

    public TCPPing(TCPSender tcpSender) {
        this.tcpSender = tcpSender;
    }

    /**
     * Set socket
     * @param socket
     */
    public void setSocket(Socket socket) {
        this.socket = socket;
    }

    @Override
    public void run() {

        // If socket was not set
        if(socket == null) {
            logger.error("No socket was assigned to the ping instance.");
            return;
        }

        while (!socket.isClosed()) {
            if (tcpSender != null) {
                MRObject mrObject = new PingMRObject();
                tcpSender.send(mrObject);

                try {
                    Thread.sleep(PING_WAIT);
                } catch (InterruptedException e) {
                    logger.error(e);
                    Thread.currentThread().interrupt();
                }
            }
        }
    }
}
