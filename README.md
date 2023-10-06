# arduino_lpf2

This is a proof of concept for integrating LPF2 sensor uart in the BluePad32 code.

## Arduino LPF2 library

Based on the sensor emulation library https://github.com/lawrie/EV3_Dexter_Industries_Sensors/tree/master/EV3_arduino I extended this library to be compatible with both ESP32 and the new Lego Mindstorms Robot inventor and Lego SPIKE Prime. The library can be found in [LPF2](https://github.com/ste7anste7an/arduino_lpf2/tree/main/LMS_ESP32_firmware/components/LPF2).

A precompiled firmware is avalailable in [prebuild firmware](https://github.com/ste7anste7an/arduino_lpf2/tree/main/LMS_ESP32_firmware/firmware).

An example of Mindstorms word block code is provided: [test_multi_sensor](https://github.com/ste7anste7an/arduino_lpf2/blob/main/Mindstorms_code/test_multi_sensor.lms) 

In this testcode a strip of neopixels should be connected to Pin 12, and a servo motor to pin 23. Furthermore, you shoudl connect a PS4 or Nintendo bluetooth remote control.

## Debug commands in Mindstorms App

Maarten Pennings explains how to [enable debug](https://github.com/maarten-pennings/Lego-Mindstorms/blob/main/ms4/faq.md) mode in the Lego Mindstorms Robot inventor app: This debug mode allows you to enable the debug blocks in the word block language.

![debug word blocks](./images/debug_word_blocks.png?raw=true "Debug Word Blocks")

Using these blocks allows to use proprietary sensors (build e.g. using arduino sensor emulation code) from Mindstorms word blokcs.
The blocks provide the following features:
- read data from port A using sendor mode M0 reading data Ds0 (when mutple values are provided, this paramater allows you to choose a specific value, and the type of reading RAW.
- This command allows to send data to the sensor using mode M0. The data can be multple values when provided as a string separated by spaces.
- this block switchs the sensot to another mode
- The PWM is not used in this POC.

## POF lms code

### Gamepad reading

The folloowing myblock allows to read the `x` and `y` of the gamepad joystick and the buttons and D-pad:
![debug word blocks](./images/gamepad_myblock.png?raw=true "Gamepad Blocks")

The sensor that is emulated uses the following sensor mode setup:

```
 sensor.create_mode("TEST0", true, DATA16, 4, 5, 0,0.0f,500.0f,0.0f,100.0f,0.0f,1024.0f,0,ABSOLUTE); //map in and map out
```


For driving neopizels, we need to send values to the sensor. In the arduino code, a callback function can be attached to de speicific mode of the sensor. When a value (or values) are received from the robot, this callback function is called.

```
 sensor.get_mode(0)->setCallback(parse_cmd_short);  // attach call back function to mode 0
```

The `parse_cmd_short` parses the 4 word values and depending on the value fo the first word, it determines whether neopixels are driven, or sensor motors (&4096)  are oprerated.

An example myblock for driving neopixels is shown below:

![debug word blocks](./images/neopixel_myblock.png?raw=true "Gamepad Blocks")

When sending a word to the sensor, we need to tranmit 2 single bytes to the specific mode. Somehow, when sending byte values 
larger than 127, things get confused. Therefore, I use base 127 to convert from bytes to word: `byte_0 + 127*byte_1 = word`.
