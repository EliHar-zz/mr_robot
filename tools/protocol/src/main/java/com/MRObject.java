package com;

import com.fasterxml.jackson.annotation.JsonIgnoreProperties;
import com.fasterxml.jackson.annotation.JsonProperty;
import com.fasterxml.jackson.databind.ObjectMapper;

import java.io.File;
import java.io.IOException;
import java.io.Serializable;
import java.util.UUID;

/**
 * An MRObject object is sent between components running Java programs.
 * An object of this class is serialized and sent. The receiver deserialize the bytes
 * into an object and reads the data.
 *
 * Each class that implements MRObject can be sent over sockets
 */
@JsonIgnoreProperties(ignoreUnknown = true)
public abstract class MRObject implements Serializable {

    @JsonProperty("object_id")
    private String id;

    public MRObject() {
        this.id = UUID.randomUUID().toString();
    }

    /**
     * Save file as JSON on host machine
     * @param file
     * @throws IOException
     */
    public void saveAsJSON(File file) throws IOException{
        ObjectMapper mapper = new ObjectMapper();
        mapper.writeValue(file, this);
    }

    /**
     * Get class name in JSON
     * @return class name
     */
    @JsonProperty("class")
    public String getClassName() {
        return getClass().getSimpleName();
    }

    @Override
    public String toString() {
        return String.format("ID: %s, class: %s", this.id, this.getClass().getSimpleName());
    }
}
