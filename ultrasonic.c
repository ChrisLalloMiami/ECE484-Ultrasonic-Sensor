#define F_CPU 16000000UL // telling controller crystal frequency (16 MHz AVR ATMega328P)
#define BAUD 9600         // Baud rate for UART communication
#define BAUD_PRESCALLER (((F_CPU / (BAUD * 16UL))) - 1)

//pulse length
volatile unsigned short hc_sr04_cnt;

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
// #include <avr/iom328p.h>

void UART_init() {
    UBRR0H = (uint8_t)(BAUD_PRESCALLER >> 8);
    UBRR0L = (uint8_t)(BAUD_PRESCALLER);
    UCSR0B = (1 << TXEN0);  // Enable transmitter
    UCSR0C = (3 << UCSZ00); // Set frame format: 8 data bits, 1 stop bit, no parity
}

// Function to transmit data
void UART_transmit(unsigned char data) {
    while (!(UCSR0A & (1 << UDRE0))); // Wait for empty transmit buffer
    UDR0 = data; // Put data into buffer, sends the data
}

// Function to print string
void UART_print(const char *str) {
    while (*str) {
        UART_transmit(*str++);
    }
}

void UART_print_uint16(uint16_t value) {
    char buffer[6]; // Maximum 5 characters for uint16_t value and 1 for null terminator
    itoa(value, buffer, 10); // Convert uint16_t to string
    UART_print(buffer); // Print the string
}

// Port Initialization
void Port_Init() {
	
	DDRD = 0b00001000; //	Set all pins of the PORTD as output, PORTD3 as input.
	PORTD = 0b00000000; // Set all pins of PORTD low which turns it off.
 
	EIMSK |= (1 << INT0); //enable external interrupt
	EICRA |= (1 << ISC00); // Any logical change on INT0 generates an interrupt request.

	//Config and enable Timer1
	TCCR1B = (0 << CS12) | (1 << CS11) | (1 << CS10); 
}

ISR(INT0_vect) {
    if (PIND & (1 << 2)) {
        TCNT1 = 0;
    } else {
        hc_sr04_cnt = TCNT1;
    }
}

unsigned char hc_sr04_meas(void) {	
	hc_sr04_cnt = 0;
	
	// transmit at least 10 us trigger pulse to the HC-SR04 Trig Pin.
	PORTD |=  (1 << 3);
	_delay_us( 10 );
	PORTD &= ~(1 << 3);

	while (hc_sr04_cnt == 0);
	if (hc_sr04_cnt < 3700) {
        return 0.000004 * hc_sr04_cnt / 2 * 34300;
    } else {
        return 41;
    }
}

int main() {
	unsigned char sm;
    Port_Init(); // Setup ultrasonic and timer configurations
    UART_init(); // Setup UART
	
	sei(); // Enable global interrupts
	while (1) {
		sm = hc_sr04_meas();

		UART_print("Distance: ");
        UART_print_uint16(sm);
        UART_print(" cm");
        UART_print("\n\r");
		_delay_ms(200);
	}
}
