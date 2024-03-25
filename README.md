# ECE484-Ultrasonic-Sensor
Interfacing of an ultrasonic sensor (HC-SR04) on an Arduino Uno R3 for ECE484. For use, ensure an appropriate AVR toolchain is available on your system. Change the `path_win_avr` variable in `compile_script.py` to reflect the path to your AVR bin folder, and then run `python compile_script.py 4 com#`, where `#` is the COM port your Arduino is connected to.

For pin configurations, see **ultrasonic.h** in in *Ultrasoniclib*. For example, to use pin 3 (PD3) as the TRIG port, lines 17 and 18 should be:

```cpp
#define TRIG_PIN 3
#define TRIG_PORT D
```

Note that the ECHO pin may only be on pins 2 and 3 (PD2 and PD3), due to the availability of interrupts on the Arduino Uno, hence the lack of an `ECHO_PORT` definition.

For details on this project, visit the [wiki](https://github.com/ChrisLalloMiami/ECE484-Ultrasonic-Sensor/wiki).
