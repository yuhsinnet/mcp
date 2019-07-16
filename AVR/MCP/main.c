/*
 * MCP.c
 *
 * Created: 2019/7/15 上午 10:45:26
 * Author : SG
 */ 

/*
*	An this project implementing a simple modbus slave device using an
*	ATmega328P running at 20MHz.
*	Baudrate: 38400, 8 data bits, 1 stop bit, no parity
*	Your busmaster can read/write the following data:
*	coils: 0 to 7
*	discrete inputs: 0 to 7
*	input registers: 0 to 3
*	holding registers: 0 to 3
*/

#define clientAddress 0x01

#define ADC_MUX_SETTING 0x0

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <stdbool.h>
//#define F_CPU 20000000
#include "yaMBSiavr.h"
#include "ioconfig.h"
#include <util/delay.h>

volatile uint8_t instate = 0;
volatile uint8_t outstate = 0;
volatile uint16_t inputRegisters[8];
volatile uint16_t holdingRegisters[4];

volatile uint16_t adc_results[4];

void timer0100us_start(void) {
	TCCR0B|=(1<<CS01); //prescaler 8
	TIMSK0|=(1<<TOIE0);
}

/*
*   Modify the following 3 functions to implement your own pin configurations...
*/
void SetOuts(volatile uint8_t in) {
	//PORTD|= (((in & (1<<3))<<4) | ((in & (1<<4))<<1) | ((in & (1<<5))<<1));
	//PORTB|= (((in & (1<<0))<<2) | ((in & (1<<1))) | ((in & (1<<2))>>2));
	//in=~in;
	//PORTB&= ~(((in & (1<<0))<<2) | ((in & (1<<1))) | ((in & (1<<2))>>2));
	//PORTD&= ~(((in & (1<<3))<<4) | ((in & (1<<4))<<1) | ((in & (1<<5))<<1));

	/*
	if (READp(in,0) == HIGH)
	{
		SET(B,2);
	} 
	else
	{
		CLEAR(B,2);
	}



		if (READp(in,1) == HIGH)
		{
			SET(B,3);
		}
		else
		{
			CLEAR(B,3);
		}



			if (READp(in,2) == HIGH)
			{
				SET(B,4);
			}
			else
			{
				CLEAR(B,4);
			}


				if (READp(in,3) == HIGH)
				{
					SET(B,5);
				}
				else
				{
					CLEAR(B,5);
				}
				*/


				PORTB =(PORTB & 0b11000011) | (in << 2);

}

uint8_t ReadIns(void) {
	uint8_t ins=0x00;
	//ins|=(PINC&((1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<4)|(1<<5)));
	//ins|=(((PIND&(1<<4))<<2)|((PIND&(1<<3))<<4));


	//ins = (PINB&(1<<0) << 1);



	//if (READ(B,0) == LOW)
	//{
	//ins = (0b00000001);
	//} 
	//else
	//{
	//ins = (0b00000000);
	//}
	//

	ins = (PIND >>2 ) ;

	ins = (~ins & 0b00111111);

	return ins;
}



void io_conf(void) { 
	/*
	 Outputs: PD6
	 Inputs: PC0, PC1, PC2, PC3, PC4, PC5, PD4, PD3
	*/
	//DDRD=0x00;
	//DDRB=0x00;
	//DDRC=0x00;
	//PORTD=0x00;
	//PORTB=0x00;
	//PORTC=0x00;
	//PORTD|=(1<<0);
	//DDRD |= (1<<2)|(1<<5)|(1<<6)|(1<<7);
	//DDRB |= (1<<0)|(1<<1)|(1<<2)|(1<<3);

	OUTPUT(B,5);
	OUTPUT(B,4);
	OUTPUT(B,3);
	OUTPUT(B,2);


}

ISR(TIMER0_OVF_vect) { //this ISR is called 9765.625 times per second
	modbusTickTimer();

	

}

void modbusGet(void) {


		static uint16_t i;
		static uint16_t j;

		if (j++ >10000)
		{
		
			if (i++ >10)
			{
				TOGGLE(B,0);
				i = 0;
				}

				j=0;
	}

	if (modbusGetBusState() & (1<<ReceiveCompleted))
	{
		switch(rxbuffer[1]) {
			case fcReadCoilStatus: {
				modbusExchangeBits(&outstate,0,8);
			}
			break;
			
			case fcReadInputStatus: {
				volatile uint8_t inps = ReadIns(); //change by test
				modbusExchangeBits(&inps,0,8);
			}
			break;
			
			case fcReadHoldingRegisters: {
				modbusExchangeRegisters(holdingRegisters,0,4);
			}
			break;
			
			case fcReadInputRegisters: {
				modbusExchangeRegisters(inputRegisters,0,8);
			}
			break;
			
			case fcForceSingleCoil: {
				modbusExchangeBits(&outstate,0,8);
				SetOuts(outstate);
			}
			break;
			
			case fcPresetSingleRegister: {
				modbusExchangeRegisters(holdingRegisters,0,4);
			}
			break;
			
			case fcForceMultipleCoils: {
				modbusExchangeBits(&outstate,0,8);
				SetOuts(outstate);
			}
			break;
			
			case fcPresetMultipleRegisters: {
				modbusExchangeRegisters(holdingRegisters,0,4);
			}
			break;
			
			default: {
				modbusSendException(ecIllegalFunction);
			}
			break;
		}
	}
}

//ISR(ADC_vect)
//{
//
		//inputRegisters[0] =ADC;
//
//}


ISR(ADC_vect)
{


		static uint16_t i;
		static uint16_t j;

		if (j++ >100)
		{
			
			if (i++ >20)
			{
				TOGGLE(B,1);
				i = 0;
			}

			j=0;
		}
		

		static uint8_t first_convervision = 1;

		if (++first_convervision >1)
		{

		

		ADCSRA &= ~(1<<ADEN);
		 uint8_t adc_MUX_tmp = (ADMUX & 0x0f);
		 

		 inputRegisters[adc_MUX_tmp] = ADC;

		 ADC = 0;

		 adc_MUX_tmp++;

		 if (adc_MUX_tmp >=6)
		 {
			adc_MUX_tmp=0;
		 }
		
		ADMUX = (ADMUX & 0xf0) | adc_MUX_tmp;

		ADCSRA |= (1<< ADEN);


		
		first_convervision = 1;
		ADCSRA |= (1 << ADSC);

		}
		else
		{
		++first_convervision;
		}



}


static void setup_adc(void)  // by ADC SETUP
{
	/* AVcc as reference, ADC0 as input */
	ADMUX =  0;

	/* Enable ADC and conversion complete interrupt. Trigger first conversion */
	ADCSRA = (1 << ADEN) | (1 << ADIE) | (1 << ADATE);
	ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // 128 prescale


	/* Trigger first conversion */
	ADCSRA |= (1 << ADSC);
}

int main(void)
{
	
	OUTPUT(B,0);
	OUTPUT(B,1);


	OUTPUT(B,5);

	DDRB = 0xff;

	io_conf();

	setup_adc();

	sei();
	modbusSetAddress(clientAddress);
	modbusInit();
    
	timer0100us_start();


	

wdt_enable(7);
    while(1)
    {
		wdt_reset();
	    modbusGet();


uint8_t tmp=0;
		if (READ(D,7) == HIGH)
		{
		
		SETp(tmp,3);
			

		}
		else
		{
			CLEARp(tmp,3);
		}
SetOuts(tmp);

    }
}



