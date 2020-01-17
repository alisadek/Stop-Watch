/*
 * stopwatch.c
 *
 *  Created on: Dec 13, 2019
 *      Author: Ali Sadek
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

unsigned char Sec_Units=0,Sec_Tenths=0;
unsigned char Min_Units=0,Min_Tenths=0;
unsigned char Hrs_Units=0,Hrs_Tenths=0;
unsigned char Flag=0;


ISR(INT0_vect){
	cli();
	WDTCR=(1<<WDE);
	while(1);
}
ISR(INT1_vect){
	TCCR1B &=~(1<<CS10) &~(1<<CS11) &~(1<<CS12);
}
ISR(INT2_vect){
	TCCR1B=(1<<WGM12)|(1<<CS10)|(1<<CS12);
}
ISR(TIMER1_COMPA_vect)
{
	Flag=1;
}
void INT0_Reset_Init (void){
	DDRD&=~(1<<PD2);
	MCUCR|= (1<<ISC01);
	MCUCR &=~(1<<ISC00);
	sei();
	GICR|=(1<<INT0);
}

void INT1_Pause_Init (void){
	DDRD&=~ (1<<PD3);
	MCUCR |= (1<<ISC11)|(1<<ISC10);
	GICR |= (1<<INT1);
	sei();

}

void INT2_Resume_Init (void){
	DDRB&=~(1<<2);
	GICR|= (1<<INT2);
	MCUCSR&=~(1<<ISC2);
	sei();
}

void TIMER1_CTC (void){
	TCCR1A |= (1<<FOC1A);
	TCCR1B |= (1<<WGM12)|(1<<CS10)| (1<<CS12);
	TCNT1=0;
	OCR1A= 1000;
	sei();
	TIMSK|= (1<<OCIE1A);
}

void Print_Time(void){
	_delay_ms(5);
		PORTA = 0x01;
		PORTC = (PORTC & 0xF0 ) | (Sec_Units & 0x0F);
		_delay_ms(5);
		PORTA = 0x02;
		PORTC = (PORTC & 0xF0 ) | (Sec_Tenths & 0x0F);
		_delay_ms(5);
		PORTA = 0x04;
		PORTC = (PORTC & 0xF0 ) | (Min_Units & 0x0F);
		_delay_ms(5);
		PORTA = 0x08;
		PORTC = (PORTC & 0xF0 ) | (Min_Tenths & 0x0F);
		_delay_ms(5);
		PORTA = 0x10;
		PORTC = (PORTC & 0xF0 ) | (Hrs_Units & 0x0F);
		_delay_ms(5);
		PORTA = 0x20;
		PORTC = (PORTC & 0xF0 ) | (Hrs_Tenths & 0x0F);

}
int main (void){
		INT0_Reset_Init() //Initialize INT0
		INT1_Pause_Init(); //Initialize INT1
		INT2_Resume_Init(); //Initialize INT2
		TIMER1_CTC(); //Call Timer

		DDRC |= 0x0F; //Set the first 4 bits of Port C as op
		DDRA |= 0x3F; //Set the first 6 bits of Port A as outputs (to control the trans.)
		PORTA = 0x3F; //Turn all segments on
		PORTC = 0x00; //zero on all 7-seg


	while(1)
	{
		if( Flag==1 )
		{
			//Stop watch Hours:Min:Sec Configuration
			Sec_Units++;
			if(Sec_Units == 10)
			{
				Sec_Tenths++;
				Sec_Units=0;
			}
			if(Sec_Tenths == 6)
			{
				Min_Units++;
				Sec_Tenths=0;
				Sec_Units=0;
			}
			if(Min_Units == 10)
			{
				Min_Tenths++;
				Min_Units=0;
				Sec_Tenths=0;
				Sec_Units=0;
			}
			if(Min_Tenths == 6)
			{
				Hrs_Units++;
				Min_Tenths=0;
				Min_Units=0;
				Sec_Tenths=0;
				Sec_Units=0;
			}
			if(Hrs_Units == 10)
			{
				Hrs_Tenths++;
				Min_Tenth=0;
				Min_Units=0;
				Sec_Tenths=0;
				Sec_Units=0;
			}
			Flag=0;
		}
		print();
	}
}
}
