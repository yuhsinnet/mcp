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
#define F_CPU 20000000UL
#define clientAddress 0x01

#define ADC_MUX_SETTING 0x0

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <stdbool.h>

#include "yaMBSiavr.h"
#include "ioconfig.h"
#include <util/delay.h>
#include <avr/eeprom.h>

volatile uint8_t instate = 0;
volatile uint8_t outstate = 0;
volatile uint16_t inputRegisters[8];
volatile uint16_t holdingRegisters[4];

volatile bool isTenTime = false;
volatile uint16_t TenTime_sum;

uint8_t  error_times;

uint16_t EEMEM myeeprom[4];

bool timer01_start;
bool timer01_is_up;

bool timer02_start;
bool timer02_is_up;

bool timer03_start;
bool timer03_is_up;

bool timer04_start;
bool timer04_is_up;
void time1ms_start(void) {

    TCCR2A |= (1 << WGM21);
    TCCR2B |= (1 << CS22) | (1 << CS21);
    TIMSK2 |= (1 << OCIE2A);
    TIFR2 |= (1 << OCF2A);
    OCR2A = 38;


}

void timer0100us_start(void) {
    TCCR0B |= (1 << CS01); //prescaler 8
    TIMSK0 |= (1 << TOIE0);
}

/*
 *   Modify the following 3 functions to implement your own pin configurations...
 */
void SetOuts(volatile uint8_t in) {

    PORTB = (PORTB & 0b11000011) | (in << 2);

}

uint8_t ReadIns(void) {
    uint8_t ins = 0x00;

    ins = (PIND >> 2);

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
	static uint16_t timer03_CV;
	static uint16_t timer04_CV;

    if (i++ > 500) {
        TOGGLE(B, 1);
        i = 0;
    }
	// 延遲讀取計時器
    if (timer01_start == true) {
        if (timer01_CV++ > 5000) {
            timer01_is_up = true;
            timer01_CV = 0;
        }
    } else {

        timer01_CV = 0;
        timer01_is_up = false;

    }
    //////////////////////////////////////////////////////////////////////////錯誤燈號閃爍計時器
    if (timer02_start == true) {
        if (timer02_CV++ > 1000) {
            timer02_is_up = true;
            timer02_CV = 0;
        }
    } else {

        timer02_CV = 0;
        timer02_is_up = false;

    }
	    //////////////////////////////////////////////////////////////////////////錯誤延遲觸發計時器
	    if (timer03_start == true) {
		    if (timer03_CV++ > 5000) {
			    timer03_is_up = true;
			    timer03_CV = 0;
		    }
		    } else {

		    timer03_CV = 0;
		    timer03_is_up = false;

	    }
	    //////////////////////////////////////////////////////////////////////////錯誤重送間隔計時器
	    if (timer04_start == true) {
		    if (timer04_CV++ > 10000) {
			    timer04_is_up = true;
			    timer04_CV = 0;
		    }
		    } else {

		    timer04_CV = 0;
		    timer04_is_up = false;

	    }

}

ISR(TIMER0_OVF_vect) { //this ISR is called 9765.625 times per second
    modbusTickTimer();



}

void modbusGet(void) {


    static uint16_t i;
    static uint16_t j;

    if (j++ > 10000) {

        if (i++ > 10) {
            TOGGLE(B, 0);
            i = 0;
        }

        j = 0;
    }

    if (modbusGetBusState() & (1 << ReceiveCompleted)) {
        switch (rxbuffer[1]) {
            case fcReadCoilStatus:
            {//讀線圈寄存器//
                modbusExchangeBits(&outstate, 0, 8);
            }
                break;

            case fcReadInputStatus:
            {//讀離散輸入寄存器//
                volatile uint8_t inps = ReadIns();
                modbusExchangeBits(&inps, 0, 8); //讀取ADC 值//
            }
                break;

            case fcReadHoldingRegisters:
            {//讀保持寄存器//
                modbusExchangeRegisters(holdingRegisters, 0, 4);
            }
                break;

            case fcReadInputRegisters:
            {//讀輸入寄存器//
                modbusExchangeRegisters(inputRegisters, 0, 8);
            }
                break;

            case fcForceSingleCoil:
            {//寫單個線圈寄存器//
                modbusExchangeBits(&outstate, 0, 8);
                SetOuts(outstate);
            }
                break;

            case fcPresetSingleRegister:
            {//寫單個保持寄存器//
                modbusExchangeRegisters(holdingRegisters, 0, 4);

                //寫入eeprom//

                eeprom_update_block((const void*) &holdingRegisters, (void*) &myeeprom, 8);
                eeprom_busy_wait();
                //寫入eeprom//

            }
                break;

            case fcForceMultipleCoils:
            {//寫多個線圈寄存器//
                modbusExchangeBits(&outstate, 0, 8);
                SetOuts(outstate);
            }
                break;

            case fcPresetMultipleRegisters:
            {//寫多個保持寄存器//
                modbusExchangeRegisters(holdingRegisters, 0, 4);

                //寫入eeprom//

                eeprom_update_block((const void*) &holdingRegisters, (void*) &myeeprom, 8);
                eeprom_busy_wait();
                //寫入eeprom//

            }
                break;

            default:
            {
                modbusSendException(ecIllegalFunction);
            }
                break;
        }
    }
}

ISR(ADC_vect) {

    static uint8_t times;


	if (isTenTime == false)
	{
	

	if (++times > 20)
	{
		
		isTenTime = true;
		times = 0;
	}
	else
	{
		TenTime_sum = TenTime_sum + ADC;
	}
}



    /*
        static uint8_t first_convervision = 1;

        if (++first_convervision > 1) {



            ADCSRA &= ~(1 << ADEN);
            uint8_t adc_MUX_tmp = (ADMUX & 0x0f);


            inputRegisters[adc_MUX_tmp] = ADC;

            ADC = 0;

            adc_MUX_tmp++;

            if (adc_MUX_tmp >= 6) {
                adc_MUX_tmp = 0;
            }

            ADMUX = (ADMUX & 0xf0) | adc_MUX_tmp;

            ADCSRA |= (1 << ADEN);



            first_convervision = 1;
            ADCSRA |= (1 << ADSC);

        } else {
            ++first_convervision;
        }


     */
}

static void setup_adc(void) // by ADC SETUP
{
    /* AVcc as reference, ADC0 as input */
    ADMUX = 0;


        ADMUX = (1 << REFS0);

    /* Enable ADC and conversion complete interrupt. Trigger first conversion */
    ADCSRA = (1 << ADEN) | (1 << ADIE) | (1 << ADATE);
    ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // 128 prescale


    /* Trigger first conversion */
    ADCSRA |= (1 << ADSC);
}

int main(void) {




    io_conf();

    setup_adc();


    modbusSetAddress(clientAddress);
    modbusInit();

    timer0100us_start();
    time1ms_start();



    sei();
    eeprom_read_block((void*) &holdingRegisters, (const void*) &myeeprom, 8);

	_delay_ms(5000);
    wdt_enable(7);
    while (1) {


	if (isTenTime == true) //ADC 20次平均
	{
		 inputRegisters[0] =  TenTime_sum / 20;
		 TenTime_sum = 0;
		 isTenTime = false;
		 
	}

        wdt_reset();
        modbusGet();

        static uint8_t error_cour_state = 0;

		static bool is_setFire;

		#define  clear_error 0
		#define Over_coura_state 1
		#define Down_coura_state 2

		#define error_times_limit 3

        if (((READ(D, 5) == LOW) | (timer04_is_up)) & (error_times < error_times_limit)) {
            error_cour_state = clear_error; //清除錯誤狀態
            CLEARp(outstate, 0);
			CLEARp(outstate, 1);
			CLEARp(outstate, 3);
			               timer01_start = false;
			               timer02_start = false;
			               timer03_start = false;
			               timer04_start = false;
						   timer04_is_up = false;
						   is_setFire = false;
        }

		bool Over_coura = (inputRegisters[0] > holdingRegisters[0]); //過電流狀態
		bool down_coura = (inputRegisters[0] < holdingRegisters[1]);// 過低電流狀態



        bool in_coura_zone = Over_coura & down_coura;// 在正常電流值
        //bool active_mode = (READ(D, 7) == LOW) & (READ(D, 6) == LOW); // 水塔水位觸發開關
        bool active_mode = (READ(D,7) == LOW );// 水塔水位觸發開關(測試)

        if (error_cour_state == 0) {

            if (active_mode) {
                SETp(outstate, 3);
                timer01_start = true;
                if (timer01_is_up == true) {// 開機送水延遲判讀
                    if (in_coura_zone == 0) {

					timer03_start = true; // 異常狀態發生延後判讀
					if (timer03_is_up)
					{
                        CLEARp(outstate, 3);

                        if (Over_coura)
                        {
	                        error_cour_state = Over_coura_state;
                        }
                        if (down_coura)
                        {
	                        error_cour_state = Down_coura_state;
                        }
					}
					else
					{
						SETp(outstate, 0);
						SETp(outstate, 1);
					}


                        
                    }
					else
					{
					            CLEARp(outstate, 0);
					            CLEARp(outstate, 1);
					timer03_start = false;
					//error_times = 0;
					}

                }


            } else {
                CLEARp(outstate, 3);
                timer01_start = false;
				timer02_start = false;
				timer03_start = false;
				//timer04_start = false;
            }


        } else {
		
		if (is_setFire == false)
		{
		
			error_times = error_times +1 ;
		
		is_setFire = true;
		}


		if (error_times < error_times_limit)
		{
			timer04_start = true;
		}


        
		    CLEARp(outstate, 3);
            timer01_start = false;


            timer02_start = true;
            if (timer02_is_up == true) {

			if (error_cour_state == Over_coura_state)
			{
				TOGGLEp(outstate, 1);
			}
			if (error_cour_state == Down_coura_state)
			{
				TOGGLEp(outstate, 0);
			}               
                timer02_is_up = false;
                timer02_start = false;
            }


        }


        SetOuts(outstate);

    }
}



