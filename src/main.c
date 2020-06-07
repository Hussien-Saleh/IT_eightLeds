/*
 a program to control 8-leds by external interrupt INT1
 requirements:
 - 8 leds are connected to PORTC, with positive logic configuration
 - a roll action is performed using the Leds, each led for half a second
 - the first led is lit and roll down to the last led then back to the first led 
 - the operation is done continuously 
 - Enable the internal pull up resistance at INT1 pin (PD3)
 - when INT1 is triggered all leds flash five times in five seconds
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdint.h>

volatile uint8_t interruptFlag = 0;

ISR(INT1_vect)
{
	interruptFlag = 1;
}

int main(void)
{
    cli();
	DDRD  &= (~(1<<PD3));  // Configure INT1/PD3 as input pin
	PORTD |= (1<<PD3);     // Enable the internal pull up resistor at PD3 pin
	GICR  |= (1<<INT1);    // Enable external interrupt pin INT1
	MCUCR |= (1<<ISC11);   // falling edge trigger
	MCUCR &= ~(1<<ISC10);
    sei();

	DDRC  = 0xFF;          // configure PORTC pins as output pins
	PORTC = 0x01;          // first led is on at the beginning 

    while(1)
    {
		if(interruptFlag == 0)
		{
			_delay_ms(500);
			PORTC = (PORTC<<1);
			if(PORTC == 0x00)
			{
				PORTC = 0x01;
			}
		}
		else if(interruptFlag == 1)
		{
			for(uint8_t i= 1; i<=5; i++)
			{
				PORTC = 0xFF;
				_delay_ms(500);
				PORTC = 0x00;
				_delay_ms(500);
			}
			interruptFlag = 0; 
		}
	}
}
