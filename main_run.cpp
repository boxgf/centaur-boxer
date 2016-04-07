#include <avr/io.h>
#include <util/delay.h>
#include <math.h>
#include "lcdnew.h"

#define max_speed 127    // (127/255)

#define drivePort PORTC
#define sensor PINA                    // change here
#define sensor_bits 0b11111000

#define input 0x00
#define output 0xFF


#define set_motor DDRC                // change here
#define set_sensor DDRA

#define motor_10 (1<<0)
#define motor_11 (1<<1)
#define motor_20 (1<<2)
#define motor_21 (1<<3)

#define OCR0 motor1_en
#define OCR2 motor2_en
#define TCNT0 motorTimer0 
#define TCNT1 delayTimer
#define TCNT2 motorTimer2

#define forward (sensor&sensor_bits)==0b01110000 
#define node (sensor&sensor_bits)==0b11111000 && nodeFlag
#define anyRight ((sensor&sensor_bits)==0b00110000)||((sensor&sensor_bits)==0b00111000)||((sensor&sensor_bits)==0b00011000)||((sensor&sensor_bits)==0b00001000) 
#define anyLeft ((sensor&sensor_bits)==0b01100000)||((sensor&sensor_bits)==0b11100000)||((sensor&sensor_bits)==0b11000000)||((sensor&sensor_bits)==0b10000000) 

#define followCond forward||anyRight||anyLeft

#define right_1 (sensor&sensor_bits)==0b00110000
#define right_2	(sensor&sensor_bits)==0b00111000
#define right_3	(sensor&sensor_bits)==0b00011000
#define right_4 (sensor&sensor_bits)==0b00001000

#define left_1 (sensor&sensor_bits)==0b01100000
#define left_2 (sensor&sensor_bits)==0b11100000
#define left_3 (sensor&sensor_bits)==0b11000000
#define left_4 (sensor&sensor_bits)==0b10000000

#define wheelAlign 390      // delay for wheel aign for right ninety
#define degree45 300
#define rightHalf ((sensor&0b00100000)==0b00100000)||((sensor&0b00010000)==0b00010000)   


void PWM_Output1(int duty)
{  
   int x;
   x=((duty*max_speed)/100);
   motor1_en=x;
}

void PWM_Output2(int duty)
{  
   int y;
   y=((duty*max_speed)/100);
   motor2_en=y;
}


void straight(void)
{   
	drivePort|=motor_10;
	drivePort&=~motor_11;
    drivePort|=motor_20;
	drivePort&=~motor_21;

	PWM_Output1(100);
    PWM_Output2(100);	
}


void right(void)
{
    drivePort|=motor_10;
	drivePort&=~motor_11;
    drivePort|=motor_20;
	drivePort&=~motor_21;

	PWM_Output2(100);

	if(right_1)
	{
		PWM_Output1(10);
	}
	else if(right_2)
	{
		PWM_Output1(45);
	}
	else if(right_3)
	{
	    PWM_Output1(60);	
	}
	else if(right_4)
	{
	    PWM_Output1(70);
	}
}

void left(void)
{
    drivePort|=motor_10;
	drivePort&=~motor_11;
    drivePort|=motor_20;
	drivePort&=~motor_21;
	
	PWM_Output1(100);
	
	if(left_1)   	
	{
		PWM_Output2(10);
	}
	else if(left_2)   
	{
		PWM_Output2(45);
	}
	else if(left_3)   
	{
		PWM_Output2(60);
	}
	else if(left_4)   
	{
		PWM_Output2(70);
	}
}
void delayfollow(int delay)
{

	while(delay>0)
	{
		while(TCNT1<=1000)    // 1 ms delay 
		{

			if((forward) 
			straight();                      
			 
			if(anyRight)
			right();

			if(anyLeft)
			left();

		}
		
		TCNT1=0;
		delay--;
		
		}
}

void diffRight(void)
{
    drivePort|=motor_10;
	drivePort&=~motor_11;
    drivePort&=~motor_20;
	drivePort|=~motor_21;
	
	PWM_Output1(100);
	PWM_Output2(100);
}

void diffleft(void)
{
    drivePort|=motor_10;
	drivePort&=~motor_11;
    drivePort&=~motor_20;
	drivePort|=~motor_21;
	
	PWM_Output1(100);
	PWM_Output2(100);
}

void right_ninety(void)         // rotate left on the intersection
{
delayfollow(wheelAlign);

diffRight();
delay(degree45);
while(!rightHalf)
diffRight();
}

void left_ninety(void)         // rotate left on the intersection
{
delayfollow(wheelAlign);

diffleft();
delay(degree45);
while(!leftHalf)
diffleft();
}

void stop(void)
{
	PWM_Output1(100);
	PWM_Output2(100);
    
	drivePort|=motor_10;
	drivePort|=motor_11;
    drivePort|=motor_20;
	drivePort|=motor_21;
	
}

void lineFollow()
{
        if(forward)
		straight();
		if(anyRight)
		right();
		if(anyLeft)
		left();
}


void main()
{
	MCUCSR|= (1<<JTD);
	MCUCSR|= (1<<JTD);
	
	TCCR2|=(1<<WGM20)|(1<<COM21)|(1<<CS20);
	TCCR0|=(1<<WGM02)|(1<<COM01)|(1<<CS00);
	
	motor1_en=max_speed;
	motor2_en=max_speed;
	
	
	set_sensor=input;    //input from sensors 
	set_motor=output;    //output to motors
	
	
	DDRB=0xff;        // LCD output 
	TCCR1B |= (0<<CS11) | (1<<CS10); 
	lcd_init();

	bool nodeFlag=0;     //1 if entered node
	Position blockPosition;//Postion is of type (3,4)
	
	//Position ultraSonic(Postion &inP)
	
	while(1)
	{   
	
		while(followCondition)   
		{
		lineFollow();
		if(shouldBlockCheck&& blockdistanceInFront()<=catchableDistance)  // isBlock returns if block is 3 cm from front sensor & shouldBlockCheck is 1 if block found earlier
		{																//catchabledistance is the distance at which the catchblock can be called to catch the 
		catchblock();   //to catch block
		shouldBlockCheck=0;
		blockInHand=1;//sets block caught postion to 1;
		pathQ=findPath(currentState.botPos, Position(0,-1))//find shortest path from botPos to 0,-1
		}
		nodeFlag=1; 
        }
		                                                      //blockPosition.P[1]
		if(node)
		{
		
		nodeFlag=0;
		if(!blockInHand)
		{
		blockPositions = ultraSonic(currentState);  //currentState is the currentState state of the bot
		//                                             make new state with the block if found     currentState()
		
	    if(blockPositions.isthere())
		{
		currentState.placeBlocks(blockPositions)//placeblocks, front , left...
		pathQ=makePath(&currentState,Position(0,2))
		}//erase prev bloc position
		
		}
		noOfBlocks=currentState.blockCount()
		direction=pathQ.pop()
		action=currentState>>direction
		currentState=generateSuccessorState(currentState,direction);
		
		if(noOfBlocks-currentState.blockCount()>0)// make it variable
		shouldBlockCheck=1;
		
		
		take(action);
		
		if(currentState.botPos==Position(2,0) && current.botdirection=='s')
		{
		if(action=='F')
		delayFollow(allignWheel+catchLength);
		else
		delayFollow(catchLength); //length to travel after alligning wheel to catch/leave block

		leaveBlock();
		reverseFollow(catchLength);
		left_ninety(0);     //without delay follow
		
		
		currentState.botDirection='e';
		pathQ.push('S');
		//pathQ.push('E');
		
		
		}
		
		if(currentState.botPos==Position(3,0) && current.botdirection=='s')
		{
	 
		reverseFollow(allignWheel);
		
		while(blockDistanceInFront()>=detectBlock);
		while(blockdistanceInFront()>=catchableDistance)
		linefollow();
		catchBlock();
		
		reverseFollow(catchLength);
		over_turn(0);                             //without delay follow
		
		currentState.botDirection='n';
		pathQ.push('E');
		
	    }
		if(currentState.botPos==Position(3,1) && current.botdirection=='e')
		{
		
		delayFollow(allignWheel+catchLength);
		
		
		leaveBlock();
		
		
		reverseFollow(catchLength);
		left_ninety(0);                             //without delay follow
		pathQ=makePath(&currentState,Position(3,3));
		pathQ.append('N');
		currentState.botDirection='n';
		pathQ.push('E');
		
	}
	}	
		
	}


