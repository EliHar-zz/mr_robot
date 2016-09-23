package string;

import com.fasterxml.jackson.core.JsonProcessingException;
import com.fasterxml.jackson.databind.ObjectMapper;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

/**
 * Additional helper functions for strings
 */
public class StringUtilsPlus {
    private static Logger logger = LogManager.getFormatterLogger(StringUtilsPlus.class);

    private StringUtilsPlus(){}

    /**
     * Convert object to JSON string
     * @return object to JSON string
     */
    public static String objectToJsonString(Object object) {
        try {
            ObjectMapper mapper = new ObjectMapper();
            return mapper.writeValueAsString(object);
        } catch (JsonProcessingException e) {
            logger.error(e);
        }
        return null;
    }
}
