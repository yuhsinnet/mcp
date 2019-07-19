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
#include <avr/eeprom.h>

volatile uint8_t instate = 0;
volatile uint8_t outstate = 0;
volatile uint16_t inputRegisters[8];
volatile uint16_t holdingRegisters[4];

volatile uint16_t adc_results[4];


uint16_t EEMEM eeprom[4];

bool timer01_start;
bool timer01_up;

bool timer02_start;
bool timer02_up;

void time1ms_start(void){

	TCCR2A |= (1<<WGM21);
	TCCR2B |= (1<<CS22)|(1<<CS21);
	TIMSK2 |= (1<< OCIE2A);
	TIFR2 |= (1<<OCF2A);
	OCR2A = 38;


}


void timer0100us_start(void) {
	TCCR0B|=(1<<CS01); //prescaler 8
	TIMSK0|=(1<<TOIE0);
}

/*
*   Modify the following 3 functions to implement your own pin configurations...
*/
void SetOuts(volatile uint8_t in) {
	
				PORTB =(PORTB & 0b11000011) | (in << 2);

}

uint8_t ReadIns(void) {
	uint8_t ins=0x00;

	ins = (PIND >>2 ) ;

	ins = (~ins & 0b00111111);

	return ins;
}



void io_conf(void) { 

	DDRB = 0xff;


}

ISR(TIMER2_COMPA_vect)//this ISR is called 1001.6025 times per second
{
		static uint16_t i;
		static uint16_t timer01_CV;
		static uint16_t timer02_CV;


			if (i++ >500)
			{
				TOGGLE(B,1);
				i = 0;
			}

			if (timer01_start == true)
			{
				if (timer01_CV++ > 5000)
				{
					timer01_up = true;
					timer01_CV = 0;
				}
			} 
			else
			{

				timer01_CV = 0;
				timer01_up =false;

			}
			//////////////////////////////////////////////////////////////////////////
			if (timer02_start == true)
			{
				if (timer02_CV++ > 1000)
				{
					timer02_up = true;
					timer02_CV = 0;
				}
			}
			else
			{

				timer02_CV = 0;
				timer02_up =false;

			}



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
			case fcReadCoilStatus: {//讀線圈寄存器//
				modbusExchangeBits(&outstate,0,8);
			}
			break;
			
			case fcReadInputStatus: {//讀離散輸入寄存器//
				volatile uint8_t inps = ReadIns(); 
				modbusExchangeBits(&inps,0,8); //讀取ADC 值//
			}
			break;
			
			case fcReadHoldingRegisters: {//讀保持寄存器//
				modbusExchangeRegisters(holdingRegisters,0,4);
			}
			break;
			
			case fcReadInputRegisters: {//讀輸入寄存器//
				modbusExchangeRegisters(inputRegisters,0,8);
			}
			break;
			
			case fcForceSingleCoil: {//寫單個線圈寄存器//
				modbusExchangeBits(&outstate,0,8);
				SetOuts(outstate);
			}
			break;
			
			case fcPresetSingleRegister: {//寫單個保持寄存器//
				modbusExchangeRegisters(holdingRegisters,0,4);

				//寫入eeprom//

				eeprom_update_block((const void*) &holdingRegisters,(void*) &eeprom,8);
				eeprom_busy_wait();
				//寫入eeprom//

			}
			break;
			
			case fcForceMultipleCoils: {//寫多個線圈寄存器//
				modbusExchangeBits(&outstate,0,8);
				SetOuts(outstate);
			}
			break;
			
			case fcPresetMultipleRegisters: {//寫多個保持寄存器//
				modbusExchangeRegisters(holdingRegisters,0,4);

				//寫入eeprom//

				eeprom_update_block((const void*) &holdingRegisters,(void*) &eeprom,8);
				eeprom_busy_wait();
				//寫入eeprom//

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




	io_conf();

	setup_adc();

	
	modbusSetAddress(clientAddress);
	modbusInit();
    
	timer0100us_start();
	time1ms_start();



sei();
	eeprom_read_block((void*) &holdingRegisters,(const void*) &eeprom,8);
	

wdt_enable(7);
    while(1)
    {
		wdt_reset();
	    modbusGet();

		static bool error_cour = false;

		if (READ(D,2) == LOW)
		{
			error_cour = false;//清除錯誤狀態
			CLEARp(outstate,0);
		}

		bool in_coura_zone = (inputRegisters[0] > holdingRegisters[0]) &(inputRegisters[0] < holdingRegisters[1]);
		bool active_mode = (READ(D,7) == LOW ) & (READ(D,6) == LOW);
		//bool active_mode = (READ(D,7) == LOW );

		if (error_cour == false)
		{

					if (active_mode)
					{
						SETp(outstate,3);
						timer01_start = true;
						if (timer01_up == true)
						{		
									if ( in_coura_zone == false )
									{
										CLEARp(outstate,3);
										error_cour = true;
									}
						}


					}
					else
					{
						CLEARp(outstate,3);
						timer01_start = false;
					}


			}
			else
			{
				CLEARp(outstate,3);
				timer01_start = false;


				timer02_start = true ;
				if (timer02_up == true )
				{
					TOGGLEp(outstate,0);
					timer02_up = false;
					timer02_start = false;
				}


			}


SetOuts(outstate);

    }
}



