/**
 * Library for interfacing with an HC-SR04 Ultrasonic Sensor on an
 * Arduino Uno R3.
 * 
 * Author: Chris Lallo
 * Date: 3/10/2024
*/

#ifndef ULTRASONIC_H
#define ULTRASONIC_H

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>

#define TRIG_PIN 2
#define TRIG_PORT D

// MUST be on an interrupt pin: PD2 or PD3 (ie. Arduino pin 2 or 3)
#define ECHO_PIN 3

void Ultrasonic_init();
unsigned char Ultrasonic_measure();

#endif // ULTRASONIC_H
