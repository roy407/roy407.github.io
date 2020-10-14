#include<intrins.h>
#include<Reg52.h>
typedef unsigned char uc;
//sbit区
//sbit button=P3^2;
sbit R=P1^7;
sbit G=P3^1;
sbit B=P3^3;
sbit TRIGL=P0^3;
sbit TRIGR=P0^5;
sbit ECHOL=P0^4;
sbit ECHOR=P0^6;
sbit leftlight=P3^4;
sbit rightlight=P3^5;
sbit x1=P2^0;//因故障原因已不用
sbit x2=P2^1;
sbit x3=P2^2;
sbit x4=P2^3;
sbit y1=P2^4;
sbit y2=P2^5;
sbit y3=P2^6;
sbit y4=P2^7;
sbit CLK=P0^1;
sbit DIO=P0^2;
sbit z_left=P1^1;
sbit z_right=P1^4;
sbit f_left=P1^2;
sbit f_right=P1^5;
uc number[10]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};
uc modal=0;
uc path[20];
uc searched=0;
unsigned char timer_s=0;
unsigned int timer_ms=0;
unsigned int lasttimer=0;
uc i=0,j=0;
uc hashmap[5][5];
void modal1();
void modal2();
void modal3();
void modal4();
void modal5();
void(*realmodal[5])()={modal1,modal2,modal3,modal4,modal5};
void light(uc alight);
const uc speed=25;
void forward()
{
	z_left=1;
	z_right=1;
	f_left=0;
	f_right=0;
}
void back()
{
	z_left=0;
	z_right=0;
	f_left=1;
	f_right=1;
}
void turnleft()
{
	z_left=0;
	f_left=1;
	z_right=1;
	f_right=0;
}
void turnright()
{
	z_left=1;
	f_left=0;
	z_right=0;
	f_right=1;
}
void stop()
{
	z_left=0;
	z_right=0;
	f_left=0;
	f_right=0;
}
void move()
{
	if(leftlight==1&&rightlight==1)
	{

		forward();
		light('G');
	}
	else if(leftlight==1&&rightlight==0)
	{
		turnleft();
		light('B');
	}
	else if(leftlight==0&&rightlight==1)
	{
		turnright();
		light('Y');
	}
	else
	{
		back();
		light('R');
	}
}
void Delay(uc i)
{
	for(;i>0;i--)
	_nop_();
}
void turn_left_90()   //??????90?
{
	//static int x=0;
	turnleft();
}
void turn_right_90()   //??????90?
{
	//static int y=0;
	turnright();
}
uc search()
{
	uc k=0,f=0;
	for(;i<5;)
	{
		if(i%2)
		{
			j=4;
			forward();
			if(lasttimer-timer_ms>100)
			{
				j--;
			}
			if(leftlight==1||rightlight==1)
			{
				hashmap[i][j-1]=1;
				j=0;
			}
			if(j==0)
			{
				turn_right_90();
				for(;f<10;f++)
				forward();
				i++;
				turn_right_90();
			}	
		}
		else 
		{
			j=0;
			forward();
			if(lasttimer-timer_ms>100)
			{
				j++;
			}
			if(leftlight==1||rightlight==1)
			{
				hashmap[i][j+1]=1;
				j=4;
			}
			if(j==4)
			{
				turn_left_90();
				for(;k<10;k++)
				forward();
				i++;
				turn_left_90();
			}
		}
	}
	if(i==4&&j==4)return 0;
	return 1;
} 
void _start(void)
{
	CLK=1;
	DIO=1;
	Delay(5);
	DIO=0;
}
void cask(void)
{
	CLK=0;
	Delay(2);
	while(DIO);
	CLK=1;
	Delay(5);
	CLK=0;
}
void _stop(void)
{
	CLK=0;
	Delay(2);
	DIO=0;
	Delay(2);
	CLK=1;
	Delay(2);
	DIO=1;
}
void write_hand(unsigned char i)
{
  unsigned char u;
	for(u=0;u<8;u++)
	{
		CLK=0;
		if(i&0x01)
		{
			DIO=1;
		}
		else
		{
			DIO=0;
		}
		CLK=1;
		i=i>>1;
	}
}	
void Display(uc first,uc second,uc third,uc forth)
{
	_start();
	write_hand(0xc0);     //   1100 0000
	cask();
	write_hand(first);     //   1
	cask();
	write_hand(second);     //   2
	cask();
	write_hand(third);     //   3
	cask();
	write_hand(forth);     //   4
	cask();
	_stop();
	//************************
	_start();
	write_hand(0x8f);    //    1000 1000
	cask();
	_stop();
}
void light(uc light)
{
	if(light=='R')
	{
		R=0;
		G=1;
		B=1;
	}
	else if(light=='G')
	{
		R=1;
		G=0;
		B=1;
	}
	else if(light=='W')
	{
		R=0;
		G=0;
		B=0;
	}
	else if(light=='B') 
	{
		R=1;
		G=1;
		B=0;
	}
	else if(light=='Y')  
	{
		R=0;
		G=0;
		B=1;
	}
}
void Display_1()
{
	uc left,right;
	if(leftlight==0)
		{
			left+=0x01;
		}
		else
		{
			left-=0x01;
		}			
		if(rightlight==0)
		{
			right+=0x01;
		}
		else
		{
			right-=0x01;
		}
		Display(left,right,number[timer_s/10],number[timer_s%10]);
}
void Display_2()
{
	uc left,right;
	if(leftlight==0)
		{
			left+=0x01;
		}
		else
		{
			left-=0x00;
		}			
		if(rightlight==0)
		{
			right+=0x01;
		}
		else
		{
			right-=0x01;
		}
		if(ECHOL==0)
		{
			left+=0x30;
		}
		else
		{
			left-=0x30;
		}
		if(ECHOR==0)
		{
			right+=0x06;
		}
		else
		{
			right-=0x06;
		}
		Display(left,right,number[timer_s/10],number[timer_s%10]);
}
void open_timer()
{
	TMOD|=0x01;
	TH0=56320%256;
	TL0=56320/256;
	TR0=1;   //???16??????
}
void open_main_shutdown()
{
	IE=0x83;
	TCON=0x01;
}
int main()
{
	open_timer();
	open_main_shutdown();
	_start();
	write_hand(0x40);      //  0100 0000
	cask();
	_stop();
	while(1)
	{
		realmodal[modal]();
	}
}
void pressbutton() interrupt 0    //P3.2
{
	modal++;
	if(modal==4)modal=0;
}
void enter_timer() interrupt 1
{
	static char c;
	TH0=56320/256;	 //??????
	TL0=56320%256;
	c++;
	timer_ms+=10;
	if(c==100)
	{
		timer_s+=1;
		c=0;
	}

}
void dog_machine()         //机器狗 
{
	static uc i=0;
	if(path[i]=='A')
	{
		forward();
		if(lasttimer-timer_ms>100)
		{
			i++;
		}
	}
	else if(path[i]=='B')	
	{
		turn_left_90();
	}
	else if(path[i]=='C')
	{
		turn_right_90();
	}
	else if(path[i]=='D')
	{
		back();
		if(lasttimer-timer_ms>100)
		{
			i++;
		}
	}
}
void DFS(uc i,uc j)
{
	//有向图
	if(i==4&&j==4)return; 
	if(hashmap[i][j]==0)
	{
		if(j!=4)
		{
			path[i]='B';
			DFS(i,j+1);
		}
		if(i!=4)
		{
			path[i]='A';
			DFS(i+1,j);
			i++;
		}
	}
	path[i]='#';
} 

void sound_init()
{
	TRIGL=1;
	TRIGR=1;
}
void sound_and_light()
{
	if(ECHOL==0&&ECHOR==0&&leftlight==0&&rightlight==0)
	{
		forward();
	}
	else if((ECHOL==0||leftlight==0)&&(ECHOR==1||rightlight==1))
	{
		turnleft();
	}
	else if((ECHOL==1||leftlight==1)&&(ECHOR==0||rightlight==0))
	{
		turnright();
	}
	else if((ECHOL==1||leftlight==1)&&(ECHOR==1||rightlight==1))
	{
		back();
	}
}
void modal1()   //红外线
{
	move();
	Display_1();
}
void modal2()  //红外线+超声波
{
	sound_init();
	Delay(10);
	sound_and_light();
	Display_2();
}
void modal3()  //超声波测距
{
	uc lefts,rights;
	sound_init();
	Delay(10);
	lasttimer=timer_ms;
			while(ECHOL==0)
			{
				lefts=17*(timer_ms-lasttimer);
				if(lefts>=99)lefts=99;
			}
			while(ECHOR==0)
			{
				rights=17*(timer_ms-lasttimer);
				if(rights>=99)rights=99;
			}
			Display(number[lefts/10],number[lefts%10],number[rights/10],number[rights%10]);
}
void modal4()  //DFS
{
	if(searched==0)
	{
		search();
	}
	if(searched==1)
	{
		DFS(i,j);
	}
	if(searched==2)
	{
		dog_machine();
	}
}
void modal5()
{
	stop();
}