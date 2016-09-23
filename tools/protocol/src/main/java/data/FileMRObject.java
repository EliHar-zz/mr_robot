package data;

import com.MRObject;
import com.fasterxml.jackson.annotation.JsonIgnore;
import com.fasterxml.jackson.annotation.JsonIgnoreProperties;
import com.fasterxml.jackson.annotation.JsonProperty;
import org.apache.commons.io.FileUtils;

import java.io.File;
import java.io.IOException;

/**
 * Send file from one host to another
 */
@JsonIgnoreProperties(ignoreUnknown = true)
public class FileMRObject extends MRObject {

    @JsonIgnore
    private byte[] bytes;

    @JsonProperty("file_name")
    private String fileName;

    public FileMRObject(File file) throws IOException {
        bytes = FileUtils.readFileToByteArray(file);
        fileName = file.getName();
    }

    /**
     * Get file bytes
     * @return file byte array
     */
    public byte[] getBytes() {
        return bytes;
    }

    /**
     * Get file name
     * @return file name
     */
    public String getFileName() {
        return fileName;
    }
}
