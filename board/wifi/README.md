# WIFI

### Setup WiFi

Follow the steps in the PDF document

#### Troubleshooting

    root@beaglebone:~# sudo ifdown wlan0 && sudo ifup wlan0
    ifdown: interface wlan0 not configured
    wpa_supplicant: /sbin/wpa_supplicant daemon failed to start
    run-parts: /etc/network/if-pre-up.d/wpasupplicant exited with return code 1
    Failed to bring up wlan0.

**Fix**

    service wpa_supplicant stop

Reference: 
 * https://cdn-learn.adafruit.com/downloads/pdf/setting-up-wifi-with-beaglebone-black.pdf
 * https://www.raspberrypi.org/forums/viewtopic.php?f=6&t=5187
