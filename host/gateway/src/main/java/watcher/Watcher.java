package watcher;

import org.apache.commons.io.monitor.FileAlterationListener;
import org.apache.commons.io.monitor.FileAlterationMonitor;
import org.apache.commons.io.monitor.FileAlterationObserver;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

import java.io.File;

/**
 * Watch service provided by Apache
 */
public class Watcher {

    private Logger logger = LogManager.getFormatterLogger(getClass());
    private final long POLL_TIME = 5 * 1000;
    private FileAlterationMonitor monitor;

    public Watcher(File folder) {

        // If folder doesn't exist
        if (!folder.exists()) {
            logger.error("Watcher not started. Folder not found: " + folder.getAbsolutePath());
            return;
        }

        // Prepare monitor
        FileAlterationObserver observer = new FileAlterationObserver(folder);
        monitor = new FileAlterationMonitor(POLL_TIME);

        // Add listener
        observer.addListener(new FileAlterationListener() {
            @Override
            public void onStart(FileAlterationObserver observer) {
                logger.info("Started watching directory: " + observer.getDirectory().getAbsolutePath());
            }

            @Override
            public void onDirectoryCreate(File directory) {

            }

            @Override
            public void onDirectoryChange(File directory) {

            }

            @Override
            public void onDirectoryDelete(File directory) {

            }

            @Override
            public void onFileCreate(File file) {
                logger.info("File created: " + file.getAbsolutePath());
            }

            @Override
            public void onFileChange(File file) {
                logger.info("File modified: " + file.getAbsolutePath());
            }

            @Override
            public void onFileDelete(File file) {
                logger.info("File deleted: " + file.getAbsolutePath());
            }

            @Override
            public void onStop(FileAlterationObserver observer) {
                logger.info("Stopped wathing directory: " + observer.getDirectory().getAbsolutePath());
            }
        });
        monitor.addObserver(observer);
    }

    /**
     * Start watching the directory
     */
    public void watch() {
        try {
            monitor.start();
        } catch (Exception e) {
            logger.error(e);
        }
    }
}
