/* usart.c
	functions and definitions for usart control
*/

#include "usart.h"
#include <stdio.h>
#include <avr/io.h>
#include <avr/power.h>
#include <avr/pgmspace.h>

#define RX_BUFSIZE 127

static int usart0_putchar(char c, FILE *stream);
int usart0_getchar(FILE *stream);

static FILE usart0_stdio = FDEV_SETUP_STREAM(usart0_putchar, usart0_getchar ,_FDEV_SETUP_RW);

int usart0_getchar(FILE *stream) {
	uint8_t c;
	char *cp, *cp2;
	static char b[RX_BUFSIZE];
	static char *rxp;

	if (rxp == 0) {
		for (cp = b;;) {
			loop_until_bit_is_set(UCSR0A, RXC0);
			if (UCSR0A & _BV(FE0))	return _FDEV_EOF;
			if (UCSR0A & _BV(DOR0))	return _FDEV_ERR;
			c = UDR0;
			/* behaviour similar to Unix stty ICRNL */
			if (c == '\r') c = '\n';
			if (c == '\n') {
				*cp = c;
				usart0_putchar(c, stream);
				rxp = b;
				break;
			}
			else if (c == '\t') 	c = ' ';
			
			if ((c >= (uint8_t)' ' && c <= (uint8_t)'\x7e') || c >= (uint8_t)'\xa0') {
				if (cp == b + RX_BUFSIZE - 1)
					usart0_putchar('\a', stream);
				else {
					*cp++ = c;
					usart0_putchar(c, stream);
				}
				continue;
			}
				
			switch (c) {
			  case 'c' & 0x1f:
				return -1;
				
			  case '\b':
			  case '\x7f':
				if (cp > b) {
					usart0_putchar('\b', stream);
					usart0_putchar(' ', stream);
					usart0_putchar('\b', stream);
					cp--;
				}
				break;
				
			  case 'r' & 0x1f:
				usart0_putchar('\r', stream);
				for (cp2 = b; cp2 < cp; cp2++)
					usart0_putchar(*cp2, stream);
				break;
				
			  case 'u' & 0x1f:
				while (cp > b) {
					usart0_putchar('\b', stream);
					usart0_putchar(' ', stream);
					usart0_putchar('\b', stream);
					cp--;
				}
				break;
				
			  case 'w' & 0x1f:
				while (cp > b && cp[-1] != ' ') {
					usart0_putchar('\b', stream);
					usart0_putchar(' ', stream);
					usart0_putchar('\b', stream);
					cp--;
				}
				break;
			}
		}
	}
	c = *rxp++;
	if (c == '\n')	rxp = 0;
	return c;
}


static int usart0_putchar(char c, FILE *stream) {

  if (c == '\n')
	usart0_putchar('\r', stream);
  loop_until_bit_is_set(UCSR0A, UDRE0);
  UDR0 = c;
  return 0;
}

/*
ISR(USART0_RX_vect) {
	UDR0=q_pop(rx_queue);
}

ISR(USART0_UDRE_vect) {
}

ISR(USART0_TX_vect) {
}
*/

void usart_init(void) {
	power_usart0_enable();

	/* Set baud rate (12bit) */
	UBRR0 = UBRR_VALUE;
	#if USE_2X
	UCSR0A |= (1 << U2X0);
	#else
	UCSR0A &=(uint8_t)~(1 << U2X0);
	#endif
	/* Double the uart clock */
	//UCSR0A |=(1<<U2X0);
	/* Enable receiver and transmitter */
	UCSR0B = (1<<RXEN0)|(1<<TXEN0)	;
	/* Enable r/t interupts, hangles input when used with some buffering functions */
	//UCSR0B |=(1<<RXCIE0)|(1<<TXCIE0);
	/* Set frame format: 8data, 1stop bit */
	UCSR0C = (0<<USBS0)|(1<<UCSZ00)|(1<<UCSZ01);
	
	stdout=stdin=&usart0_stdio;
	printf_P(PSTR("usart: init usart0... [done]\n"));
}

