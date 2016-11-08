package gateway;

import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
import watcher.Watcher;

import java.io.File;
import java.io.IOException;
import java.util.Properties;

/**
 * Driver class for the Gateway component
 */
public class Gateway {

    private final String CONFIG_PROPERTIES = "/config.properties";
    private final String SAVE_IMAGE_PATH = "save_image_path";
    private final String LOAD_IMAGE_PATH = "load_image_path";
    private Properties properties;
    private Logger logger = LogManager.getFormatterLogger(getClass());

    public static void main(String[] args) {
        Gateway gateway = new Gateway();
        if(gateway.loadProperties()) {

            // Start services
            gateway.startServices();
        } else {
            throw new RuntimeException("No properties file found. Exiting Gateway ...");
        }
    }

    public Gateway() {
        properties = new Properties();
    }

    public void startServices() {
        // Load image path folder
        File loadImageFolder = new File(properties.getProperty(LOAD_IMAGE_PATH));

        // Start watcher
        Watcher watcher = new Watcher(loadImageFolder);
        watcher.watch();
    }

    /**
     * Load properties
     * @return
     */
    public boolean loadProperties() {
        try {
            properties.load(getClass().getResourceAsStream(CONFIG_PROPERTIES));
        } catch (IOException e) {
            logger.error(e);
            return false;
        }
        return true;
    }
}
