package tcp;

import com.MRObject;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

import java.io.IOException;
import java.io.ObjectOutputStream;
import java.net.Socket;
import java.util.concurrent.ConcurrentLinkedDeque;

/**
 * A library class that is used to send objects of type MRObject
 * This class uses the ObjectOutputStream to send an object as input for the receiver
 * Object sent through this class are enqueued manually but sent automatically
 */
public class TCPSender implements Runnable {

    private Logger logger = LogManager.getFormatterLogger(getClass());
    private ConcurrentLinkedDeque<MRObject> objectDeque;
    private Socket socket;

    /**
     * Create the sender logic.
     */
    public TCPSender() {
        this.objectDeque = new ConcurrentLinkedDeque<>();
    }

    /**
     * Set the active socket
     * @param socket
     */
    public void setSocket(Socket socket) {
        this.socket = socket;
    }

    /**
     *  Send a MRObject object by enqueuing it
     * @param mrObject
     * @return true if sent successfully
     */
    public void send(MRObject mrObject) {
        objectDeque.offer(mrObject);
    }

    /**
     * Set object to the socket output stream
     * @param objectOutputStream
     * @param mrObject
     */
    private boolean sendObject(ObjectOutputStream objectOutputStream, MRObject mrObject) {
        try {
            objectOutputStream.writeObject(mrObject);
            objectOutputStream.flush();
            return true;
        } catch (IOException e) {
            logger.error(e);
        }
        return false;
    }

    @Override
    public void run() {

        // If socket was not set
        if(socket == null) {
            logger.error("No socket was assigned to the sender.");
            return;
        }

        try (ObjectOutputStream objectOutputStream = new ObjectOutputStream(socket.getOutputStream())) {
            while(!socket.isClosed()) {
                if(!objectDeque.isEmpty()) {
                    MRObject pollObject = objectDeque.poll();
                    if(sendObject(objectOutputStream, pollObject)) {
                        logger.info("Data sent successfully: %s", pollObject.toString());
                        logger.info("Sender queue size is now: %d", objectDeque.size());
                    } else {
                        logger.debug("Data was not sent");
                        throw new IOException("Failed to send data ... maybe connection is lost?");
                    }
                }
            }
        } catch (IOException e) {
            logger.error(e);
            try {
                logger.error("Closing socket after failing to send data");
                if(!socket.isClosed()) {
                    socket.close();
                }
            } catch (IOException ee) {
                logger.error(ee);
            }
        }
    }
}
