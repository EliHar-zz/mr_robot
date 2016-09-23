package tcp.listener;

import com.MRObject;

@FunctionalInterface
public interface AbstractObjectListener {
    void onDataReceived(MRObject mrObject);
}
