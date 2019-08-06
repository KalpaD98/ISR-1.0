
#include <asf.h>

//*******************************************************************//
//Micro controller			:ATmega32
//System Clock				:16MHz
//Project					:Intruder scare off robot
//Authors					:Kalpa,Niroopama,Gajitha,Lashini,Niranjan
//******************************************************************//

#define	F_CPU	16000000
/*Defines a macro for the delay.h header file. F_CPU is the micro controller frequency value for the delay.h header file.
Default value of F_CPU in delay.h header file is 16000000(16MHz)*/

#include <time.h>
/*for timing of the robot active time*/

#include<avr/io.h>
/*Includes io.h header file where all the Input/Output Registers and its Bits are defined for all AVR micro controllers*/

#include<util/delay.h>
/*Includes delay.h header file which defines two functions, _delay_ms (millisecond delay) and _delay_us (microsecond delay)*/

#define 	REF 	615
/*Defines a macro for the Reference value of the sensors*/

/*ADC Function Declarations*/
void adc_init(void);
int read_adc_channel(unsigned char channel);

//other function declarations
int Drive(void);
int Flasher(void);
int Sound(void);
int WaterGun(void);
int Laser(void);

int main(void)
{
	Drive();
}

/* obstacle avoidance definition */

int Drive(void)
{
	DDRB=0x0f;
	/*PB0,PB1,PB2 and PB3 pins of PortB are declared output (i/p1,i/p2,i/p3,i/p4 pins of DC Motor Driver are connected)*/

	int left_sensor_value,front_sensor_value,right_sensor_value;
	/*Variable declarations*/

	adc_init();
	/*ADC initialization*/
	
	/*Start of infinite loop*/
	while(1)
	{
		left_sensor_value=read_adc_channel(0);
		/*Reading left IR sensor value*/
		
		front_sensor_value=read_adc_channel(1);
		/*Reading front IR sensor value*/

		right_sensor_value=read_adc_channel(2);
		/*Reading right IR sensor value*/
		
		/*Checking the sensor values with the reference value*/
		//if(front_sensor_value > REF)
		if(front_sensor_value > REF && left_sensor_value > REF && right_sensor_value > REF) //testing phase
		{
			PORTB=0x0a;
			/*Robot will move in forward direction*/
		}
		else if(left_sensor_value < REF)
		{
			PORTB=0x08;
			/*Robot will move in RIGHT direction*/
		}
		else if(right_sensor_value < REF)
		{
			PORTB=0x02;
			/*Robot will move in LEFT direction*/
		}
		else
		{
			//PORTB=0x08;
			//Robot will move in RIGHT direction
			
			
			//test code
			PORTB=0x05; 
			_delay_ms(300);
			//Robot will move in BACKWARD direction
			PORTB=0x08;
			//Robot will rotate in RIGHT direction until clear path is found
			
			//
		}
	}
	
}


/*ADC Function Definitions*/
void adc_init(void)
{
	ADCSRA=(1<<ADEN)|(1<<ADSC)|(1<<ADATE)|(1<<ADPS2);
	SFIOR=0x00;
}

int read_adc_channel(unsigned char channel)
{
	int adc_value;
	unsigned char temp;
	ADMUX=(1<<REFS0)|channel;
	_delay_ms(1);
	temp=ADCL;
	adc_value=ADCH;
	adc_value=(adc_value<<8)|temp;
	return adc_value;
}

int Laser(void)
{
	DDRD |= 0xff;
	TCCR1A |= 1<<WGM11 | 1<<COM1A1 | 1<<COM1A0;
	TCCR1B |= 1<<WGM12 | 1<<WGM13 | 1<<CS10;
	ICR1 = 19999;
	
	while (1) // 1 is temporary
	{
		
			
			OCR1A = ICR1 - 800;
			_delay_ms(300);
			
			
			OCR1A = ICR1 - 2000;
			_delay_ms(300);
		
	}
	
}

int Flasher(void)
{
	DDRB |= 1 << PINB0;
	while (1) // 1 is temporary
	{
		PORTB |= 1 << PINB0;
		_delay_ms(1000);
		PORTB &= ~(1 << PINB0);
		_delay_ms(1000);
	}
	
}