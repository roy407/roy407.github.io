#include<intrins.h>
#include<Reg52.h>
typedef unsigned char uc;
//sbit区
//sbit button=P3^2;
sbit R=P1^7;
sbit G=P3^1;
sbit B=P3^3;
sbit TRIGL=P0^5;
sbit TRIGR=P0^4;
sbit ECHOL=P0^6;
sbit ECHOR=P0^3;
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
void Display(uc first,uc second,uc third,uc forth);
uc number[10]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};
uc modal=0;
uc path[20];
uc searched=0;
unsigned char timer_s=0;
unsigned int timer_ms=0;
unsigned int timer_newms=1;
unsigned int lasttimer=0;
uc i=0,j=0;
uc hashmap[5][5];
void modal1();
void modal2();
void modal3();
void modal4();//模式4一分为三
void modal5();
void modal6();
void modal7();
void(*realmodal[7])()={modal1,modal2,modal3,modal4,modal5,modal6,modal7};
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
void turn_left_90()  
{
	unsigned int newlasttimer=timer_ms;
	while(timer_ms-newlasttimer<=700)
	{
		turnleft();
		Display(0x38/*L*/,0x79/*E*/,0x71/*F*/,0x07/*T*/);
	}
}
void turn_right_90()   
{
	unsigned int newlasttimer=timer_ms;
	while(timer_ms-newlasttimer<=700)
	{
		turnright();
		Display(0x7b/*R*/,0x06/*I*/,0x3b/*G*/,0x76/*H*/);
	}
}
uc search()
{
	lasttimer=timer_ms;
	while(1)
	{
		
	for(;i<5;)
	{
		if(i%2)
		{
			j=4;
			Display(number[0],number[i+1],number[0],number[j+1]);
			while(timer_ms-lasttimer<1000)
			forward();
				j--;
				lasttimer=timer_ms;
			if(leftlight==1||rightlight==1)
			{
				hashmap[i][j-1]=1;
				j=0;
			}
			if(j==0)
			{
				lasttimer=timer_ms;
				turn_right_90();
				while(timer_ms-lasttimer<1000)
				forward();
				turn_right_90();
			}	
		}
		else 
		{
			j=0;
			Display(number[0],number[i+1],number[0],number[j+1]);
			while(timer_ms-lasttimer<1000)
			forward();
				j++;
				lasttimer=timer_ms;
			if(leftlight==1||rightlight==1)
			{
				hashmap[i][j+1]=1;
				j=4;
			}
			if(j==4)
			{
				turn_left_90();
			}
		}
	}
	if(i==4&&j==4)return 0;
	return 1;
	}
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
	uc left=0x00,right=0x00;
	if(leftlight==0)
		{
			left|=0x01;
		}
		if(rightlight==0)
		{
			right|=0x01;
		}
		Display(left,right,number[timer_s/10],number[timer_s%10]);
}
void Display_2()
{
	uc left=0x00,right=0x00;
	if(leftlight==0)
		{
			left|=0x01;
		}	
		if(rightlight==0)
		{
			right|=0x01;
		}
		if(ECHOL==0)
		{
			left|=0x30;
		}
		if(ECHOR==0)
		{
			right|=0x06;
		}
		Display(left,right,number[timer_s/10],number[timer_s%10]);
}
void open_timer()
{
	TMOD|=0x01;
	TH0=56320/256;
	TL0=56320%256;
	TR0=1;   
}
void open_another_timer()
{
	TMOD|=0x01;
	TH1=64536/256;
	TL1=64536%256;
	ET1=1;
	TR1=1;
}
void open_main_shutdown()
{
	IE=0x83;
	TCON=0x01;
}
void open_board()
{
	_start();
	write_hand(0x40);      //  0100 0000
	cask();
	_stop();
}
int main()
{
	open_main_shutdown();
	open_timer();
	open_board();
	while(1)
	{
		if(modal==2)
		{
			open_another_timer();
		}
		realmodal[modal]();
	}
}
void pressbutton() interrupt 0    //P3.2
{
	modal++;
	if(modal==7)modal=0;
	Display(0x00,0x00,0x00,number[modal+1]);
}
void enter_timer() interrupt 1
{
	static unsigned char c=0;
	TMOD|=0x01;
	TH0=56320/256;
	TL0=56320%256;
	TR0=1;
	c++;
	timer_ms+=10;
	if(c==100)
	{
		timer_s+=1;
		c=0;
	}
}
void enter_another_timer() interrupt 3
{
	TMOD|=0x01;
	TH1=64536/256;
	TL1=64536%256;
	TR1=1;
	timer_newms+=1;    //每次1ms
	Display(number[0],number[0],number[0],number[timer_newms%10]);
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
void DFS()
{
	//有向图
	if(i==4&&j==4)return; 
	if(hashmap[i][j]==0)
	{
		if(j!=4)
		{
			path[i]='B';
			DFS();
		}
		if(i!=4)
		{
			path[i]='A';
			DFS();
			i++;
		}
	}
	path[i]='#';
} 

void sound_init()
{
	TRIGL=1;
	TRIGR=1;
	Delay(15);
	TRIGL=0;
	TRIGR=0;

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
	/*sound_init();
	Delay(10);
	Display_2();*/
	move();
	Display_1();
}
void modal2()  //红外线+超声波
{
	sound_init();
	sound_and_light();
	Display_2();
}
void modal3()  //超声波测距
{
	unsigned int s=0,bai=0,shi=0;
	sound_init();
	lasttimer=timer_newms;
	light('R');
	while(timer_newms-lasttimer<=20)
	{
		light('W');
			if(ECHOL==0)
			{
				s=17*(timer_ms-lasttimer);
				light('G');
			}
	}
	bai=s%1000;
	shi=bai%100;
	Display(number[s/1000],number[bai/100],number[shi/10],number[shi%10]);
}
void modal4()  //DFS
{
	search();
}
void modal5()
{
	stop();
}
void modal6()
{
	DFS();
}
void modal7()
{
	dog_machine();
}