/*
 * main.c
 *
 *  Created on: 6 січ. 2016
 *      Author: sd
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "UART/UART.h"

#define MAX_PULSE 3000

#define TIM_VAL (TCNT1/2)
UART * port;
bool flag = 0;

inline void sendData(uint8_t num, uint16_t val)
{

	uint8_t crc = 0;
	crc ^= '$';
	crc ^= 'R';
	crc ^= 'C';
	crc ^= num;
	crc ^= ((uint8_t*) &val)[0];
	crc ^= ((uint8_t*) &val)[1];

	cli();
	port->print("$RC");
	port->putch(num);
	port->WriteCOM(2, (unsigned char*) &val);
	port->putch(crc);
	sei();
}

ISR(TIMER1_OVF_vect)
{
	flag = 1;
}

ISR(INT0_vect)
{
	static uint8_t counter = 0;
	if ((TIM_VAL > MAX_PULSE) || (flag))
		counter = 0;
	else
		sendData(counter++, TIM_VAL);
	TCNT1 = 0;
	flag = 0;
}

ISR(USART_TX_vect)
{
	port->tx_byte_int();
}

void init()
{

	// Input/Output Ports initialization
	// Port B initialization
	// Function: Bit7=In Bit6=In Bit5=In Bit4=In Bit3=In Bit2=In Bit1=In Bit0=In
	DDRB = (0 << DDB7) | (0 << DDB6) | (0 << DDB5) | (0 << DDB4) | (0 << DDB3)
			| (0 << DDB2) | (0 << DDB1) | (0 << DDB0);
	// State: Bit7=T Bit6=T Bit5=T Bit4=T Bit3=T Bit2=T Bit1=T Bit0=T
	PORTB = (0 << PORTB7) | (0 << PORTB6) | (0 << PORTB5) | (0 << PORTB4)
			| (0 << PORTB3) | (0 << PORTB2) | (0 << PORTB1) | (0 << PORTB0);

	// Port C initialization
	// Function: Bit6=In Bit5=In Bit4=In Bit3=In Bit2=In Bit1=In Bit0=In
	DDRC = (0 << DDC6) | (0 << DDC5) | (0 << DDC4) | (0 << DDC3) | (0 << DDC2)
			| (0 << DDC1) | (0 << DDC0);
	// State: Bit6=T Bit5=T Bit4=T Bit3=T Bit2=T Bit1=T Bit0=T
	PORTC = (0 << PORTC6) | (0 << PORTC5) | (0 << PORTC4) | (0 << PORTC3)
			| (0 << PORTC2) | (0 << PORTC1) | (0 << PORTC0);

	// Port D initialization
	// Function: Bit7=In Bit6=In Bit5=In Bit4=In Bit3=In Bit2=In Bit1=In Bit0=In
	DDRD = (0 << DDD7) | (0 << DDD6) | (0 << DDD5) | (0 << DDD4) | (0 << DDD3)
			| (0 << DDD2) | (0 << DDD1) | (0 << DDD0);
	// State: Bit7=T Bit6=T Bit5=T Bit4=T Bit3=T Bit2=T Bit1=T Bit0=T
	PORTD = (0 << PORTD7) | (0 << PORTD6) | (0 << PORTD5) | (0 << PORTD4)
			| (0 << PORTD3) | (0 << PORTD2) | (0 << PORTD1) | (0 << PORTD0);

	// Timer/Counter 0 initialization
	// Clock source: System Clock
	// Clock value: Timer 0 Stopped
	// Mode: Normal top=0xFF
	// OC0A output: Disconnected
	// OC0B output: Disconnected
	TCCR0A = (0 << COM0A1) | (0 << COM0A0) | (0 << COM0B1) | (0 << COM0B0)
			| (0 << WGM01) | (0 << WGM00);
	TCCR0B = (0 << WGM02) | (0 << CS02) | (0 << CS01) | (0 << CS00);
	TCNT0 = 0x00;
	OCR0A = 0x00;
	OCR0B = 0x00;

	// Timer/Counter 1 initialization
	// Clock source: System Clock
	// Clock value: 2000,000 kHz
	// Mode: Normal top=0xFFFF
	// OC1A output: Disconnected
	// OC1B output: Disconnected
	// Noise Canceler: Off
	// Input Capture on Falling Edge
	// Timer Period: 32,768 ms
	// Timer1 Overflow Interrupt: On
	// Input Capture Interrupt: Off
	// Compare A Match Interrupt: Off
	// Compare B Match Interrupt: Off
	TCCR1A = (0 << COM1A1) | (0 << COM1A0) | (0 << COM1B1) | (0 << COM1B0)
			| (0 << WGM11) | (0 << WGM10);
	TCCR1B = (0 << ICNC1) | (0 << ICES1) | (0 << WGM13) | (0 << WGM12)
			| (0 << CS12) | (1 << CS11) | (0 << CS10);
	TCNT1H = 0x00;
	TCNT1L = 0x00;
	ICR1H = 0x00;
	ICR1L = 0x00;
	OCR1AH = 0x00;
	OCR1AL = 0x00;
	OCR1BH = 0x00;
	OCR1BL = 0x00;

	// Timer/Counter 2 initialization
	// Clock source: System Clock
	// Clock value: Timer2 Stopped
	// Mode: Normal top=0xFF
	// OC2A output: Disconnected
	// OC2B output: Disconnected
	ASSR = (0 << EXCLK) | (0 << AS2);
	TCCR2A = (0 << COM2A1) | (0 << COM2A0) | (0 << COM2B1) | (0 << COM2B0)
			| (0 << WGM21) | (0 << WGM20);
	TCCR2B = (0 << WGM22) | (0 << CS22) | (0 << CS21) | (0 << CS20);
	TCNT2 = 0x00;
	OCR2A = 0x00;
	OCR2B = 0x00;

	// Timer/Counter 0 Interrupt(s) initialization
	TIMSK0 = (0 << OCIE0B) | (0 << OCIE0A) | (0 << TOIE0);

	// Timer/Counter 1 Interrupt(s) initialization
	TIMSK1 = (0 << ICIE1) | (0 << OCIE1B) | (0 << OCIE1A) | (1 << TOIE1);

	// Timer/Counter 2 Interrupt(s) initialization
	TIMSK2 = (0 << OCIE2B) | (0 << OCIE2A) | (0 << TOIE2);

	// External Interrupt(s) initialization
	// INT0: On
	// INT0 Mode: Rising Edge
	// INT1: Off
	// Interrupt on any change on pins PCINT0-7: Off
	// Interrupt on any change on pins PCINT8-14: Off
	// Interrupt on any change on pins PCINT16-23: Off
	EICRA = (0 << ISC11) | (0 << ISC10) | (1 << ISC01) | (1 << ISC00);
	EIMSK = (0 << INT1) | (1 << INT0);
	EIFR = (0 << INTF1) | (1 << INTF0);
	PCICR = (0 << PCIE2) | (0 << PCIE1) | (0 << PCIE0);

	// USART initialization
	// USART disabled
	UCSR0B = (0 << RXCIE0) | (0 << TXCIE0) | (0 << UDRIE0) | (0 << RXEN0)
			| (0 << TXEN0) | (0 << UCSZ02) | (0 << RXB80) | (0 << TXB80);

	// Analog Comparator initialization
	// Analog Comparator: Off
	// The Analog Comparator's positive input is
	// connected to the AIN0 pin
	// The Analog Comparator's negative input is
	// connected to the AIN1 pin
	ACSR = (1 << ACD) | (0 << ACBG) | (0 << ACO) | (0 << ACI) | (0 << ACIE)
			| (0 << ACIC) | (0 << ACIS1) | (0 << ACIS0);
	ADCSRB = (0 << ACME);
	// Digital input buffer on AIN0: On
	// Digital input buffer on AIN1: On
	DIDR1 = (0 << AIN0D) | (0 << AIN1D);

	// ADC initialization
	// ADC disabled
	ADCSRA = (0 << ADEN) | (0 << ADSC) | (0 << ADATE) | (0 << ADIF)
			| (0 << ADIE) | (0 << ADPS2) | (0 << ADPS1) | (0 << ADPS0);

	// SPI initialization
	// SPI disabled
	SPCR = (0 << SPIE) | (0 << SPE) | (0 << DORD) | (0 << MSTR) | (0 << CPOL)
			| (0 << CPHA) | (0 << SPR1) | (0 << SPR0);

	// TWI initialization
	// TWI disabled
	TWCR = (0 << TWEA) | (0 << TWSTA) | (0 << TWSTO) | (0 << TWEN)
			| (0 << TWIE);

	// Global enable interrupts
	sei();
}

int main()
{
	init();

	UART outPort(UDR0, 115200);
	port = &outPort;

	while (1)
		;

	return 0;
}

