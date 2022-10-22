# Wateringsystem
Small automatic watering system for the ESP32-C3-DevKitM-1 with Discord Webhook integration and Data logging to a Micro-SD Card[0,1].
Created in the Arduino IDE.

## Used Stuff
- ESP32-C3-DevKitM-1 [0]
- Capacative Moisture Sensor [2]
- Pump (You can use basically any 5v pump)
- Micro SD Card Reader WH125 & Micro-SD Card [3]
- Breadboard & Jumper cables
- 5V Relay [4]

## How it works
Every 30 seconds the average value of the last 30 seconds is written to the micro-sd card and every 300 seconds to the discord webhook.

If the moisture level provided by the sensor in the plant is too low, the pump gets activated for a few seconds. You might need to play around with the sensor values a little bit depending on your plant.

For actually using this code, change the values in credentials.h. 

GPIO Pin Layout is pretty much described at the top of watering.ino.

## Read out & Plot the data
With python3 it is very easy to visualize the data using matplotlib. A very small script is provided in waterplot.py. 

To use it copy the logfile "log" into the same directory or change the path in the script. 


## Resources

[0] https://docs.espressif.com/projects/esp-idf/en/latest/esp32c3/hw-reference/esp32c3/user-guide-devkitm-1.html

[1] https://support.discord.com/hc/en-us/articles/360045093012

[2] https://arduinogetstarted.com/tutorials/arduino-soil-moisture-sensor

[3] https://randomnerdtutorials.com/esp32-microsd-card-arduino/

[4] https://www.instructables.com/Relay-5V-Using-Arduino/
