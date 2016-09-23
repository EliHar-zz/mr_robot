package tcp;

import com.MRObject;
import data.PingMRObject;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
import tcp.listener.AbstractObjectListener;

import java.io.EOFException;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.StreamCorruptedException;
import java.net.Socket;
import java.util.concurrent.ConcurrentLinkedDeque;

/**
 * A library class that is used to receive objects of type MRObject
 * This class uses the ObjectInputStream to receive an object as input from the sender
 * Object received are enqueue automatically but dequeue manually
 */
public class TCPReceiver implements Runnable {

    private Logger logger = LogManager.getFormatterLogger(getClass());
    private Socket socket;
    private ConcurrentLinkedDeque<MRObject> objectDeque;
    private AbstractObjectListener abstractObjectListener;

    public TCPReceiver() {
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
     * Read an object from the queue
     * @return MRObject object or null
     */
    private MRObject receive() {
        if(!objectDeque.isEmpty()) {
            return objectDeque.poll();
        }
        return null;
    }

    /**
     * Set abstract client listener
     * @param abstractObjectListener
     */
    public void setAbstractObjectListener(AbstractObjectListener abstractObjectListener) {
        this.abstractObjectListener = abstractObjectListener;
    }

    /**
     * Read a MRObject object from the socket input stream and add it to the queue
     * @param objectInputStream
     */
    private MRObject receiveObject(ObjectInputStream objectInputStream) {
        try {
            // If no object in the input stream, readObject() will wait until an object is available
            return (MRObject) objectInputStream.readObject();
        } catch (EOFException e) {
            logger.error(e);
            logger.error("Reached input stream EOF ... maybe connection is lost?");
            if(socket!= null && !socket.isClosed()) {
                try {
                    socket.close();
                    logger.info("Closing socket after failing to peek/read from input stream");
                } catch (IOException ee) {
                    logger.error(ee);
                }
            }
        } catch (IOException | ClassNotFoundException e) {
            logger.error(e);
        }
        return null;
    }

    @Override
    public void run() {

        // If socket was not set
        if(socket == null) {
            logger.error("No socket was assigned to the receiver.");
            return;
        }

        // Start polling objects from the queue
        Thread receiver = new Thread() {
            @Override
            public void run() {
                while(!socket.isClosed()) {
                    MRObject objectReceived = receive();

                    // If there is data received and a listener was set
                    if(objectReceived != null && abstractObjectListener != null) {
                        abstractObjectListener.onDataReceived(objectReceived);
                    }
                }
            }
        };
        receiver.start();

        try (ObjectInputStream objectInputStream = new ObjectInputStream(socket.getInputStream())) {
            while(!socket.isClosed()) {
                MRObject mrObject = receiveObject(objectInputStream);
                if(mrObject == null) {
                    logger.info("Data was not received!");
                } else {
                    logger.info("Data received: %s", mrObject.toString());
                    if(!(mrObject instanceof PingMRObject)) {
                        objectDeque.offer(mrObject);
                    }
                    logger.info("Receiver queue size is now: %d", objectDeque.size());
                }
            }
        } catch (StreamCorruptedException e) {
            logger.error(e);
            logger.error("Expecting an object but received unrecognized data for the system. Maybe someone is connected from outside the system?");
        } catch (IOException e) {
            logger.error(e);
        } finally {
            logger.info("Connection will be closed");
            if(!socket.isClosed()) {
                try {
                    socket.close();
                } catch (IOException ee) {
                    logger.error(ee);
                }
            }
        }
    }
}
