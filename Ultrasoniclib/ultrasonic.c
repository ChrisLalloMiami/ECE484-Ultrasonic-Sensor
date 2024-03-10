/**
 * Source file for HC-SR04 interfacing.
 * 
 * Author: Chris Lallo
 * Date: 3/10/2024
*/

#include "ultrasonic.h"

volatile unsigned short counter;

/**
 * Initializes the ultrasonic sensor, setting up the trigger and echo pins,
 * the respective interrupts, and Timer1.
*/
void Ultrasonic_init() {
    #if TRIG_PORT == D
        PORTD = 0b00000000; // Set all pins of PORTD low which turns it off.
        DDRD |= (1 << TRIG_PIN);
    #elif TRIG_PORT == B
        PORTB = 0b00000000; // Set all pins of PORTB low which turns it off.
        DDRB |= (1 << TRIG_PIN);
    #endif

    #if ECHO_PIN == 2
        EIMSK |= (1 << INT0); // Enable external interrupt INT0
        EICRA |= (1 << ISC00); // Any logical change on INT0 generates an interrupt request
        #define INTERRUPT_VECT INT0_vect
        #define PIN_PORT PIND
        #define PORT PORTD

    #elif ECHO_PIN == 3
        EIMSK |= (1 << INT1); // Enable external interrupt INT1
        EICRA |= (1 << ISC10); // Any logical change on INT1 generates an interrupt request
        #define INTERRUPT_VECT INT1_vect
        #define PIN_PORT PIND
        #define PORT PORTD
    #endif

    // Config and enable Timer1
	TCCR1B = (0 << CS12) | (1 << CS11) | (1 << CS10); 
}

/**
 * Takes a measurement on the ultrasonic sensor, returning either
 * the distance recorded in centimeters or an x, denoting an
 * unreceived echo.
*/
unsigned char Ultrasonic_measure() {
	counter = 0;
	
	// transmit at least 10 us trigger pulse to the HC-SR04 Trig Pin.
	PORT |=  (1 << TRIG_PIN);
	_delay_us(10);
	PORT &= ~(1 << TRIG_PIN);

	while (counter == 0);
	if (counter < 3700) {
        return 0.000004 * counter / 2 * 34300;
    } else {
        return 'x';
    }
}

ISR(INTERRUPT_VECT) {
    if (PIN_PORT & (1 << ECHO_PIN)) {
        TCNT1 = 0;
    } else {
        counter = TCNT1;
    }
}
