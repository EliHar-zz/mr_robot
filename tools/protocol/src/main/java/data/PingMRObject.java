package data;

import com.MRObject;

import java.io.Serializable;

/**
 * This class is used only to ping the other side of a connection.
 * Ping is meant to be lightweight to increase communication performance.
 * Objects of this type will not be queue in the receiver side
 */
public class PingMRObject extends MRObject implements Serializable {}
