# This program uses PWM to light pin P9-14 (EHRPWM1A) 
# at 4 levels of brightness

import Adafruit_BBIO.PWM as PWM
import time

PIN = "P9_14"

# Set pin to have intensity of 0% and a frequency of 1000hz
PWM.start(PIN, 0, 1000)

# Run infinite loop that adjusts intensity of light
while True:
    time.sleep(1)
    PWM.set_duty_cycle(PIN, 30) # intensity = 30%
    
    time.sleep(1)
    PWM.set_duty_cycle(PIN, 60) # intensity = 60%
    
    time.sleep(1)
    PWM.set_duty_cycle(PIN, 100) # intensity = 100%
    
    time.sleep(1)
    PWM.set_duty_cycle(PIN, 0) # intensity = 0%
