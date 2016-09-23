package data;

import com.MRObject;
import com.fasterxml.jackson.annotation.JsonIgnore;
import com.fasterxml.jackson.annotation.JsonIgnoreProperties;
import com.fasterxml.jackson.annotation.JsonProperty;
import org.apache.commons.io.FileUtils;

import java.io.File;
import java.io.IOException;
import java.io.Serializable;

/**
 * Send an image data and related information from one host to another
 */
@JsonIgnoreProperties(ignoreUnknown = true)
public class ImageMRObject extends MRObject implements Serializable {

    @JsonIgnore
    private byte[] bytes;

    @JsonProperty("image_path")
    private String imagePath;

    @JsonProperty("image_name")
    private String imageName;

    /**
     * Load the image from the JSON file and store it as an array of bytes
     */
    public void storeImageAsBytes() throws IOException {
        bytes = FileUtils.readFileToByteArray(new File(imagePath));
    }

    public String getImagePath() {
        return imagePath;
    }

    public String getImageName() {
        return imageName;
    }
    /**
     * Save image file on host machine
     * @param file
     * @throws IOException
     */
    public void saveImageFile(File file) throws IOException {
        FileUtils.writeByteArrayToFile(file, bytes);
    }

    /**
     * Get byte array of the image
     * @return image bytes
     */
    public byte[] getBytes(){
        return bytes;
    }
}
