/******************************************************************************/
/*
函数功能：读取编码器信号
输出：
电路设计：
	PD0 左边编码器白线
	PD1 右边编码器白线
	PD2 左边编码器绿线
	PD3 右边编码器绿线
晶振：内部4M
条件：左边绿线正传时落后于白线
实验：还有问题。可能原因有编码器故障，电路板内带上下拉影响了外部信号输入等。程序没问题。需要实验室示波器查错。
*/
/******************************************************************************/

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>


#define uchar unsigned char

unsigned char LInpulses=0;
unsigned char RInpulses=0;

uchar NumberCode[10]={192,249,164,176,153,146,130,248,128,144};//LED number code


int main()
{
	led_flash_port_init();
	set_ext_int();
	while(1)
	{
		led_flash(LInpulses,50);
	}
	return 0;
}




void set_ext_int()										//set external interupt
{
	cli();
	DDRD&=(0xff&(0<<2));
	DDRD&=(0xff&(0<<3));
	PORTD=0X00;										//不使用内部上拉
	MCUCR|=(1<<ISC11)|(1<<ISC10)|(1<<ISC01)|(1<<ISC00);	//上升沿中断
	GICR|=(1<<INT0)|(1<<INT1);
	sei();
}
ISR(INT0_vect)											//左
{
	if((PIND|0x01)==0)									//反转
	{
		LInpulses--;
	}
	else
		LInpulses++;
}
ISR(INT1_vect)
{
	if((PIND|0X02)==0)
	{
		RInpulses++;
	}
	else
		RInpulses--;
}




void led_flash_port_init()
{
	DDRA=0xff;
	PORTA|=0b00001111;
	DDRB=0xff;
}

void led_flash(unsigned int number,uchar BrushingInterval)//Number is a 4 digit number; BrushingInterval in ms.
{
	uchar dig[4]={0};
	dig[0]=number%10;dig[1]=(number%100)/10;dig[2]=(number%1000)/100;dig[3]=(number%10000)/1000;
	PORTA=0b00001110;
	PORTB= NumberCode[dig[3]];
	_delay_ms(BrushingInterval);
	PORTA=0b00001101;
	PORTB= NumberCode[dig[2]];
	_delay_ms(BrushingInterval);
	PORTA=0b00001011;
	PORTB= NumberCode[dig[1]];
	_delay_ms(BrushingInterval);
	PORTA=0b00000111;
	PORTB= NumberCode[dig[0]];
	_delay_ms(BrushingInterval);
}
uchar inp_speed(uchar impulse)	//单位：
{
	uchar intergral=impulse/187*0.1;

}
