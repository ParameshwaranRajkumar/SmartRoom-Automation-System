// PIC16F887 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = ON       // Power-up Timer Enable bit (PWRT enabled)
#pragma config MCLRE = ON       // RE3/MCLR pin function select bit (RE3/MCLR pin function is MCLR)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = ON        // Internal External Switchover bit (Internal/External Switchover mode is enabled)
#pragma config FCMEN = ON       // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is enabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.
#define _XTAL_FREQ 4000000
#include<xc.h>
int total_1=0,total_2=0,def_temp=20,a=0,b=0,c=0;
void enable()
{
    RE1=1;
    __delay_ms(7);
    RE1=0;
    __delay_ms(7);
}
void lcd(char ins, char data)
{
    RE0=ins;
    PORTD=data;
    enable();
}
void string(char *ptr)
{
    while(*ptr!='\0')
    {
        lcd(1,*ptr++);
    }
}
void lm35()
{
    ADCON0=0X81;
    GO=1;
    while(GO);
    total_1=(ADRESH<<8)|(ADRESL);
}
void ldr()
{
    ADCON0=0X85;
    GO=1;
    while(GO);
    total_2=(ADRESH<<8)|(ADRESL);
}
void main()
{
    int temp=0,voltage=0,num=0,rem=0;
    int store[10];
    PORTA=PORTC=PORTD=PORTE=0X00;
    TRISC=TRISD=TRISE=0X00;
    TRISA=0XCF;
    ANSEL=0X03;
    ANSELH=0X00;
    ADCON1=0X90;
    lcd(0,0x38);
    lcd(0,0x0C);
    lcd(0,0x80);
    while(1)
    {
        if(RA2==1)
        {
            if(RA6==1)
            {
                while(RA6==1);
                def_temp++;
            }
            if(RA7==1)
            {
                while(RA7==1);
                def_temp--;
            }
            lm35();
            ldr();
            voltage=(int)((float)total_1*4.88758);
            temp=voltage/10;
            //temp=(int)((float)total_1*0.488);
            if(temp>def_temp)
            {
                RC0=1;
            }
            if(temp==def_temp||temp<def_temp)
            {
                RC0=0;
            }
            if(total_2>700)
            {
                RC1=0;
            }
     
            if(total_2<700)
            {
                RC1=1;
            }
        }
        lcd(0,0x80);
         string("temp");
         lcd(1,'=');
         int i=0;
         num=def_temp;
        while(num)
        {
            rem=num%10;
            num=num/10;
            store[i++]=rem;
        }
        //store[i]='\0';
        while(i>0)
        {
            lcd(1,store[--i]+48);
        }
            lcd(1,223);
            lcd(1,'C');
            lcd(0,0x8A);//fan cond
            lcd(1,'F');
            lcd(1,'=');
            if(RC0==1)
            {
                string("ON ");
                //lcd(0,0x01);
            }
            if(RC0==0)
            {
                string("OFF");
            }
            lcd(0,0xC0);//light cond
            string("Light");
            lcd(1,'=');
            if(RC1==1)
            {
                string("ON ");
                //lcd(0,0x01);
            }
            if(RC1==0)
            {
                string("OFF");
            }
        
    }
}