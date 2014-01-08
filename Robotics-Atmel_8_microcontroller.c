#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

//#include <inttypes.h>

/* ------ Perception of Sensor Inputs -----*/


#define north1		0x01			/*00000001*/
#define west1 		0x02			/*00000010*/
#define east1 		0x04			/*00000100*/
#define south1 		0x08			/*00001000*/
#define fire 		0x10			/*00010000*/

/*------------------------------------------*/

/*-------- Motor Driving : 10 implies Forward , 01 implies Backward for motors-----------------
----------- First octet - Motor 1, Second octet - Motor 2 ------------------------------------*/

#define forward 	0b00011011			/* Motor 1 & 2 forward */
#define backward 	0b00101101			/* Motor 1 & 2 backward */
#define right 		0b00101011			/* Motor 1 forward & Motor 2 Backward */
#define left 		0b00011101			/* Motor 1 backward & Motor 1 forward */

/* ----------------------- */

#define JNO 50						// Maximum Number of junctions the bot can Navigate

/*----------- Checking Functions: Zero if path Un-Available and Non-Zero if Path Available- */

#define nc (PIND & (north1))
#define	ec (PIND & (east1))
#define wc (PIND & (west1)) 

/* ----------------------- */


int count;					
int north[JNO],east[JNO],west[JNO];	
void Motor_Control(char);
void reverse(void);
void Extinguish_Fire(void);
void Movement(void);	
int Align(char);

int main()
{	
	DDRD = 0b01100000;
	DDRC = 0b00111111;
	while (1)
	{
		if(!(PIND & fire))		// <------- Check if the fire is detected by the sensor (0 if detected)
		{
			Motor_Control('s');
			Extinguish_Fire();
			continue;
		}
		
		if (nc ==0 || ec != 0 || ec!= 0)	// Navigate only if north is Not Available or east is Available or west is Available
		{
			count++;
			if (nc == 0) north[count] = 0;
			else north[count] = 1;
			if (ec != 0) east[count] = 1;
			else east[count] = 0;
			if(wc != 0) west[count] = 1;
			else west[count] = 0;
			Movement();
			continue;
		}
		
/*		if(nc == 0 && ec ==0 && wc ==0)	
		{
			Motor_Control('r');
			reverse();
			continue;
		}
		
		if(ec != 0 && wc == 0)				//If east is working but not west turn east
		{
			Motor_Control('e');
			continue;
		}
		if(wc != 0 && ec == 0)				//If west is woking but not east turn east
		{
			Motor_Control('w');
			continue;
		}*/
	}
}
void Extinguish_Fire()
{
	PORTD |= _BV(5) | _BV(6);
	while (!(PIND & fire))				//Loop checks whether the fire is extin
	{	
		_delay_ms(11000);
	}
	PORTD &= ~_BV(5);				// 	Stops The fan 
	PORTD &= ~_BV(6);				//	Stops The fan
	
}


void Movement()
{
	if (north[count] != 0 && north[count] != -1) 	//If North is available Take North & mark it taken
	{
		north[count] = 0;
		Motor_Control('f');
	}
	else
	if (east[count] == 1 && east[count] != -1) 	//Navigate East
	{
		east[count] = -1;
		Motor_Control('e');
	}
	else
	if (west[count] == 1 && west[count] != -1) 	//Navigate West
	{
		west[count] = -1;
		Motor_Control('w');
	}
	else if ((north[count] == -1 || north[count] == 0) && (east[count] == -1 || east[count] == 0) && (west[count] == -1 || west [count] ==0))
	{
		reverse();
		
	}
}

void reverse()					//Retraces the maze backwards
{
	count--;
	Motor_Control('r');
	while (1)
	{
		if (nc ==0 || ec != 0 || ec!= 0)
		{
			if (east[count] == -1)
			{
				east[count] = 0;
				Motor_Control('w');
				break;
			}
			else 
			if (west[count] == -1)
			{
				west[count] = 0;
				Motor_Control('e');
				break;
			}
		}
	}
}

void Motor_Control(char c)		//Controlling the Motor
{

	switch (c)
	{
		case 'f':
		{
			PORTC = forward;
			break;
		}
		case 'r':
		{
			PORTC = backward;
			break;
		}
		case 'e':
		{
			PORTC = right;
			if (Align('r')) PORTC = forward;
			break;
		}
		case 'w':
		{
			PORTC = left;
			if (Align('r')) PORTC = forward;
			break;
		}
		case 's' :
		{
			PORTC = 0b00000000;
		}
	}
}

int Align(char c)			//Checks Whether The Bot Has turned Properly (90 degrees)
{
	int a=0,b=0;
	a = PIND;
	a &= 0b00001111;
	b |= a;
	if (c=='r')			//Code for right Alignment
	{
		if(b & _BV(3))
		{
			b = b << 1;
			b |= 0x01;
		}
		else b = b<<1;
	}
	
	if (c=='l')
	{
		if(b & _BV(1))
		{
			b = b >>1;
			b|= 0x08;
		}
		else b = b>>1;
	}
	
	while(1)
	{
		if (b & PIND)
		{
			return(1);
		}
		else continue;
	}
	return 0;
}