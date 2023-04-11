
# Greenpower Winter Of Making

A telemetry system for a Greenpower racing go-kart (https://www.greenpower.co.uk/), built using LoRA and programmed with C++. This project has kindly been funded through the hack club winter of making grant program.

# Currently Implemented

- Temperature of the motor
- Speed of the vehicle
- Temperature of the batteries

TODO:

- Current sensor
- Braking notfications?
- Web app?
- SD Datalogging
- Bigger screens
- Better Error reporting
- Graphing of results?
- Elapsed time of race
- Tyre pressure
- Strain sensor
- Better casing designs
- Gyroscope?
- Swith to imu?
- Battery voltage
- Test the range of the system
- Throttle position
- Battery Voltage
- Recording amp hours used
- Distance travelled
- Lap number


# System Diagram

![Lora Diagram (1)](https://user-images.githubusercontent.com/43633955/218691854-5a6bf5c3-c304-4b04-b4e8-5104f52c9db4.png)


# Required Parts

| Product         | Supplier/Link                         | Cost   |
| --------------- | ------------------------------------- | ------ |
| Heltec Lora32 v2  | https://bit.ly/3zlZaot Amazon  | $55  |
| DS18B20 Sensor| https://bit.ly/3VdB5J5  Amazon | $16 |
| 10000mah PowerBank | https://bit.ly/3v5uVju Amazon | $13 |
| 15k Ohm Resistors | https://bit.ly/3FLcQMF Amazon | $3 |
| 1k Potentiometer | https://bit.ly/3FJpsDI Amazon | $4 |
| 100nF Capacitors| https://bit.ly/3G6lmXE Amazon| $7 |
| Zener Diode | https://bit.ly/3HV55q9 Amazon | $2.5 |
| Perfboard | https://bit.ly/3G37gq5 Amazon | $6.5 |
| Right angle USB MICRO Cable | Already Have | $8 |
| Speed sensor | https://bit.ly/3WocDWD Amazon | $7 |
| 6.2k Resistors | https://bit.ly/3YDCV92 Amazon | $4.5 |
| Total           |                                       | $126.5 |

# Error Codes

- 1 LED BLINK: Oled initalization has failed, check if OLED pins are correct (By default they are) and if the OLED is visibly damaged.
- 2 LED BLINKS: No packet was recieved by the reciever, check antennas on both reciever and transmitter. 
- 3 LED BLINKS: Motor temperature is 85 C, this is normally an indicator that the DS18B20 sensors are broken or not working (Or your vehicle is running too hot).
- 4 LED BLINKS: Heatsink temperature is 85 C, this is normally an indicator that the DS18B20 sensors are broken or not working (Or your vehicle is running too hot).

## License

This project, code and all of it's respecting files is licensed under the AGPL v3 Copyleft license. This means that all projects utilising this code must also be opensource, please respect this.


