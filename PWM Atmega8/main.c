/*
 * Atmega8 microcontroller firmware 
 *
 * PWM Atmega8.c (main)
 * Author : Kiselov
 */ 

#define F_CPU 8000000UL
#include <avr/io.h>
#include <stdlib.h>
#include <util/delay.h>

void ADC_Init() {
	ADMUX |= (1 << REFS0);
	ADCSRA |= (1 << ADPS0) | (1 << ADPS1) | (1 << ADPS2) | (1 << ADEN);
}

uint16_t ADC_Read(uint8_t channel) {
	channel = channel & 0b00000111;
	ADMUX = (ADMUX & 0xF8) | channel;
	ADCSRA |= (1 << ADSC);
	while(ADCSRA & (1<<ADSC));
	return ADC;
}

int main(void)
{
	uint16_t brightness = 0x00;
	DDRB= 1<<4; // Enable fourth b pin for output
	PORTB=0x00; // Empty all b ports
	ASSR=0x00; // Empty asynchronous status register
	TCCR2=0b01101110; // Fast PWM, Clear OC2 on Compare Match, clkT2S/256 (From prescaler)
	
	// Init adc (start adc conversion)
	ADC_Init();
	
    /* Main loop */
    while (1) 
    {
		brightness = ADC_Read(0) * 0.249267;
		OCR2 = brightness;
		_delay_ms(100);
    }
}

