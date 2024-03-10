/**
 * Main method for the interfacing of an HC-SR04 Ultrasonic Sensor
 * on an Arduino Uno R3 for ECE484.
 * 
 * Author: Chris Lallo
 * Date: 3/10/2024
*/

#include <avr/io.h>
#include "UARTlib/uart.h"
#include "Ultrasoniclib/ultrasonic.h"

int main() {
	unsigned char sm;

    Ultrasonic_init(); // Setup ultrasonic and timer configurations
    UART_init(); // Setup UART
	sei(); // Enable global interrupts

    UART_print("Ultrasonic sensor demo ready!\n\r");
    
	while (1) {
		sm = Ultrasonic_measure();

        if (sm == 'x') {
            UART_print("No echo received!\n\r");
        } else {
            UART_print("Distance: ");
            UART_print_uint16(sm);
            UART_print(" cm");
            UART_print("\n\r");
        }
		_delay_ms(200);
	}
}
