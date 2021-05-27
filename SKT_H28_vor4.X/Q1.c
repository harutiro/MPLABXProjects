//�T���v���v���O�����i���O�ۑ�P�j  file:prog1all.c

#include <xc.h>
#include <pic16f1938.h>
//#include "mono2020.h"

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.
// CONFIG1
#pragma config FOSC = INTOSC    // Oscillator Selection (HS Oscillator, High-speed crystal/resonator connected between OSC1 and OSC2 pins)
#pragma config WDTE = OFF       // Watchdog Timer Enable (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable (PWRT disabled)
#pragma config MCLRE = ON       // MCLR Pin Function Select (MCLR/VPP pin function is MCLR)
#pragma config CP = OFF         // Flash Program Memory Code Protection (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Memory Code Protection (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable (Brown-out Reset disabled)
#pragma config CLKOUTEN = OFF   // Clock Out Enable (CLKOUT function is disabled. I/O or oscillator function on the CLKOUT pin)
#pragma config IESO = OFF       // Internal/External Switchover (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable (Fail-Safe Clock Monitor is disabled)
// CONFIG2
#pragma config WRT = OFF        // Flash Memory Self-Write Protection (Write protection off)
#pragma config VCAPEN = OFF     // Voltage Regulator Capacitor Enable (All VCAP pin functionality is disabled)
#pragma config PLLEN = OFF      // PLL Enable (4x PLL enabled)
#pragma config STVREN = ON      // Stack Overflow/Underflow Reset Enable (Stack Overflow or Underflow will cause a Reset)
#pragma config BORV = LO        // Brown-out Reset Voltage Selection (Brown-out Reset Voltage (Vbor), low trip point selected.)
#pragma config LVP = OFF        // Low-Voltage Programming Enable (High-voltage on MCLR/VPP must be used for programming)

//�v���v���Z�b�T�[
//�h�^�n�|�[�g�֌W
#define DATA    PORTB
#define SW1     RA0
#define SW2     RA1
#define SW3     RA2
#define DIGIT1  RC5
#define DIGIT2  RC6
#define CLK     RC7
#define LEDR    RC2
#define LEDB    RC1
#define LEDG    RC0
#define BUZZ    RC2

//�\���A�X�C�b�`�����`
#define UP      0
#define DN      1
#define PUSH    0
#define NPUSH   1
#define R       0
#define P       1
#define PR      2
#define ON      0
#define OFF     1
#define REN     2
#define SW3R    ((flag_sw3==1)&&(flag_R==1))
#define SW3P    ((flag_sw3==0)&&(flag_P==1))
#define SW3PR   ((flag_sw3==0)&&(flag_P==1))

//�Z�O�����g�� to ���\����`
#define SEG_    0x00
#define SEGa    0x01
#define SEGb    0x02
#define SEGc    0x04
#define SEGd    0x08
#define SEGe    0x10
#define SEGf    0x20
#define SEGg    0x40
#define SEGdp   0x80

//�V�Z�O�����g�����E�����\����`
#define SEG0    (SEGa|SEGb|SEGc|SEGd|SEGe|SEGf)
#define SEG1    (SEGb|SEGc)
#define SEG2    (SEGa|SEGb|SEGd|SEGg|SEGe)
#define SEG3    (SEGg|SEGa|SEGd|SEGb|SEGc)
#define SEG4    (SEGb|SEGc|SEGf|SEGg)
#define SEG5    (SEGf|SEGd|SEGg|SEGa|SEGc)
#define SEG6    (SEGf|SEGe|SEGd|SEGc|SEGa|SEGg)
#define SEG7    (SEGa|SEGb|SEGc|SEGf)
#define SEG8    (SEGa|SEGb|SEGc|SEGd|SEGe|SEGf|SEGg)
#define SEG9    (SEGf|SEGd|SEGg|SEGa|SEGb|SEGc)
#define SEGA    (SEGa|SEGb|SEGc|SEGd|SEGe|SEGg)
#define SEGB    (SEGc|SEGd|SEGe|SEGf|SEGg)
#define SEGC    (SEGd|SEGe|SEGg)
#define SEGD    (SEGb|SEGc|SEGd|SEGe|SEGg)
#define SEGE    (SEGa|SEGd|SEGe|SEGf|SEGg)
#define SEGF    (SEGa|SEGe|SEGf|SEGg)
#define SEGH    (SEGb|SEGc|SEGe|SEGf|SEGg)
#define SEGL    (SEGd|SEGe|SEGf)
#define SEGN    (SEGc|SEGe|SEGg)
#define SEGU    (SEGb|SEGc|SEGd|SEGe|SEGf)
#define SEGP    (SEGa|SEGb|SEGe|SEGf|SEGg)
#define SEGDP   (SEGdp)

//�X�e�b�s���O���[�^�㎥��`
#define SM_AB   0x03
#define SM_BC   0x06
#define SM_CD   0x0C
#define SM_DA   0x09
#define SM_OFF  0x00
#define NO      0
#define T2      1
#define H2      2
#define H       1
#define L       5

//�v�Z�����p 8,10,16�i��
#define OCT     8
#define DEC    10
#define HEX    16

//������g���i16MHz�j
#define _XTAL_FREQ 16000000

//�x�����ԁidelay�j
#define __delay(x) _delay((unsigned long)((x)))
#define __delay_us(x) _delay((unsigned long)((x)*(_XTAL_FREQ/4000000UL)))
#define __delay_ms(x) _delay((unsigned long)((x)*(_XTAL_FREQ/4000UL)))

//�v���g�^�C�v�錾
void portinit(void);
void wait(unsigned short wt);
void hyouji(void);
void zenoff(void);
void dynam(unsigned short dig1data,unsigned short dig2data,unsigned short smdata,unsigned int tr);
void hen7(int value,unsigned short ns1);
void hens(unsigned short keta10,unsigned short keta1,unsigned short ns2);
void tactsw(void);
void tact(unsigned short mode);
void fclr(unsigned short flg);
void kaisu(unsigned short mode1);
void jikan(void);
void buzzon();
void buzzof();

//�O���[�o���ϐ���`
unsigned short flag_sw1,flag_sw2,flag_sw3;
unsigned short flag_R,flag_P;
unsigned short flg;
unsigned short cw=0,ccw=0;
int atai10,atai1,atai;
int count;

//�O���[�o���ϐ���`�i���i�ϊ��p�j
unsigned short code1000,code100,code10,code1,fugou;

//�Q�Ɨp�z��i���i�ϊ��p�e�[�u���j
unsigned short samp[]={SEG0,SEG1,SEG2,SEG3,SEG4,SEG5,SEG6,SEG7,SEG8,SEG9,SEGA,SEGB,SEGC,SEGD,SEGE,SEGF};
unsigned short samf[]={SEGF,SEGE,SEGD,SEGC,SEGB,SEGA,SEG9,SEG8,SEG7,SEG6,SEG5,SEG4,SEG3,SEG2,SEG1,SEG0};

//�z��f�[�^
unsigned short num0[]={SEG0,SEG1,SEG2,SEG3,SEG4,SEG5,SEG6,SEG7,SEG8,SEG9};  //  num0-9  �z�� �����\��
unsigned short gusu[]={SEG0,SEG2,SEG4,SEG6,SEG8};                           //  gusu    �z�� �����\��
unsigned short kisu[]={SEG1,SEG3,SEG5,SEG7,SEG9};                           //  kisu    �z�� ��\��
unsigned short moji[]={SEGA,SEGB,SEGC,SEGD,SEGE,SEGF,SEGH,SEGL,SEGN,SEGP};  //  moji    �z�� �����S��
unsigned short moja[]={SEGA,SEGB,SEGC,SEGD,SEGE,SEGF};                      //  moja    �z�� ����A to F
unsigned short mojf[]={SEGF,SEGE,SEGD,SEGC,SEGB,SEGA};                      //  mojf    �z�� ����F to A
unsigned short mojh[]={SEGH,SEGL};                                          //  mojh    �z�� ����H to L
unsigned short mojn[]={SEGN,SEGP};                                          //  mojn    �z�� ����N to P

//�h�^�n�|�[�g�������֐�
//portinit();
void portinit(void)
{  
    //�A�i���O���͖���
    ANSELA=0x00;
    ANSELB=0x00; 
    //�|�[�g�ݒ�
    PORTA=0x07;
    TRISA=0x07;
    PORTB=0x00;
    TRISB=0x00;
    PORTC=0x07;
    TRISC=0x00; 
}

//�^�C�~���O�����֐�
//wait(��������);
void wait(unsigned short wt)
{
    unsigned int i,j;
    for(i=10*wt;i>0;--i){   
        for(j=10;j>0;--j);
    }
}


//�Z�O�����g�\���������֐�
//hyouji();
void hyouji(void)
{
    DIGIT1=DIGIT2=0;                //�V�Z�O�����g��\��
    wait(1);
}

//�V�Z�O�k�d�c���k�d�c���X�e�b�s���O���[�^��~�֐�
//zenoff();                               
void zenoff(void)
{
    LEDR=1;
    LEDB=1;
    LEDG=1;
    DATA=SM_OFF;                    //�X�e�b�s���O���[�^��~
    CLK=0;CLK=1;CLK=0;              //�N���b�N�M���o��
}

//�V�Z�O�����g���X�e�b�s���O���[�^�_�C�i�~�b�N�\���쓮�֐�
//dynam�i��ʃf�[�^�A���ʃf�[�^�A�쓮�㎥�A�쓮���x�j
void dynam(unsigned short dig1data,unsigned short dig2data,unsigned short smdata,unsigned int tr)
{
    unsigned int j=0,tm=0;
    unsigned short ext=0,smd=0;     //�ؑ֕ϐ�������
    CLK=0;
    DATA=dig1data;                  //�\���f�[�^�i�[
    DIGIT1=0;DIGIT2=1;              //�Z�O�����g�\������
    if(tr==L){tm=4000;}             //�P�x����
    else{tm=2000;}
    for(j=tm;j>0;--j); 
    DIGIT1=0;DIGIT2=0;              //�V�Z�O�����g��\��
    DATA=dig2data;                  //�\���f�[�^�i�[
    DIGIT2=0;DIGIT1=1;              //�Z�O�����g�\������
    if(tr==L){tm=4000;}             //�P�x����
    else{tm=2000;}
    for(j=tm;j>0;--j); 
    DIGIT1=0;DIGIT2=0;              //�V�Z�O�����g��\��
    switch(smdata){                 //�r�l���㎥����
            case NO:                //���㎥�Œ�~
                DATA=SM_OFF;smd=SEG0;
                CLK=0;CLK=1;CLK=0;  //�N���b�N�M���o��
                break;
            case T2:    //�Q���㎥�i���v���j
                if(flg==1)cw=0;flg=0;
                if((cw==0)&&(ext==0)){cw=1;ccw=0;ext=1;DATA=smd=SM_DA;}
                if((cw==1)&&(ext==0)){cw=2;ccw=3;ext=1;DATA=smd=SM_CD;}
                if((cw==2)&&(ext==0)){cw=3;ccw=2;ext=1;DATA=smd=SM_BC;}
                if((cw==3)&&(ext==0)){cw=0;ccw=1;ext=1;DATA=smd=SM_AB;}
                CLK=0;CLK=1;CLK=0;  //�N���b�N�M���o��
                break;
            case H2:    //�Q���㎥�i�����v���j
                if(flg==1)ccw=0;flg=0;
                if((ccw==0)&&(ext==0)){ccw=1;cw=0;ext=1;DATA=smd=SM_AB;}
                if((ccw==1)&&(ext==0)){ccw=2;cw=3;ext=1;DATA=smd=SM_BC;}
                if((ccw==2)&&(ext==0)){ccw=3;cw=2;ext=1;DATA=smd=SM_CD;}
                if((ccw==3)&&(ext==0)){ccw=0;cw=1;ext=1;DATA=smd=SM_DA;}
                CLK=0;CLK=1;CLK=0;  //�N���b�N�M���o��
                break;
            default:    //�Y���Ȃ�
                cw=ccw=0;
                break;
    }
}

//���l�f�[�^��\���R�[�h�ɕϊ��֐��i�ln��SEGn�j
//hen7(���l,�\�������邎�i���j��255�܂őΉ�
void hen7(int value,unsigned short ns1)
{
    int value100,value10,value1;
    unsigned int l,m,n;
    fugou=0;
    if(value<0)fugou=1;             //�{�l:fugou=0�C-�l:fugou=1
    if(fugou==1){
        value=(~(value-1));         //�{���l�ɕϊ�(���l-1���r�b�g���]�j
    }
    switch(ns1){
            case DEC:                               //  10�i���\���ɕϊ�
                value100=value/100;                 // 100�ʂ̐��l���Z�o
                value10 =(value-(value100*100))/10; //  10�ʂ̐��l���Z�o
                value1  =value%10;                  //   1�ʂ̐��l���Z�o
                code1000=SEG0;                      //1000�ʂ̕\��=0
                for(l=0;l<=2;++l){
                    if(value100==l)code100=samp[l]; // 100�ʂ̐��l��\���ϊ�
                }
                for(m=0;m<=9;++m){
                    if(value10==m)code10=samp[m];   //  10�ʂ̐��l��\���ϊ�
                }
                for(n=0;n<=9;++n){
                    if(value1==n)code1=samp[n];     //   1�ʂ̐��l��\���ϊ�
                }
                break;
            case OCT:                               //   8�i���\���ɕϊ�
                value100=value>>6;                  // 100�ʂ̐��l���Z�o
                value10 =(value-(value100<<6))>>3;  //  10�ʂ̐��l���Z�o
                value   =value%8;                   //   1�ʂ̐��l���Z�o
                code1000=SEG0;                      //1000�ʂ̕\��=0
                for(l=0;l<=3;++l){
                    if(value100==l)code100=samp[l]; // 100�ʂ̐��l��\���ϊ�
                }
                for(m=0;m<=7;++m){
                    if(value10==m)code10=samp[m];   //  10�ʂ̐��l��\���ϊ�
                }
                for(n=0;n<=7;++n){
                    if(value1==n)code1=samp[n];     //   1�ʂ̐��l��\���ϊ�
                }
                break;
            case HEX:                               //  16�i�\���ɕϊ�
                code1000=SEG0;                      //1000�ʂ̕\��=0
                code100=SEG0;                       // 100�ʂ̕\��=0
                value10 =value>>4;                  //  10�ʂ̐��l���Z�o
                value1  =value%16;                  //   1�ʂ̐��l���Z�o
                for(m=0;m<=15;++m){
                    if(value10==m)code10=samp[m];   //  10�ʂ̐��l��\���ϊ�
                }
                for(n=0;n<=15;++n){
                    if(value1==n)code1=samp[n];     //   1�ʂ̐��l��\���ϊ�
                }
                break;
            default:
                break;
    }
}
//�\���R�[�h�𐔒l�f�[�^�ɕϊ��֐��iSEGn���ln�j
//hens(�\��10��,�\��1��,���̂��i���j���\��FF�Ή�
void hens(unsigned short keta10,unsigned short keta1,unsigned short ns2)
{
    unsigned int m,n;
    keta10=keta10;                                  //10����DP���폜
    keta1=keta1;                                    // 1����DP���폜
    if((ns2==DEC)||(ns2==HEX)){                     //10�i��:DEC�A16�i��:HEX
        for(m=0;m<=15;++m){
            if(keta10==samp[m])atai10=m;            //10���̕\���𐔒l�ɕϊ�
        }
        for(n=0;n<=15;++n){
            if(keta1 ==samp[n])atai1=n;             // 1���̕\���𐔒l�ɕϊ�
        }
        if(keta10==SEG_)atai10=0;                   //10������\�����A���l0
        if(keta1 ==SEG_)atai1 =0;                   // 1������\�����A���l0
        if(ns2==DEC){atai=((atai10*10)+atai1);}     //10�i�����Z�o
        if(ns2==HEX){atai=((atai10*16)+atai1);}     //16�i�����Z�o
    }
}

//�`���^�h�~����Ԏ擾�֐��i���F0�A���F1�j
//tactsw()
void tactsw(void)
{
    unsigned short a,b,c;
    b=c=0;
    while(1){
            a=SW3;                                  //SW3��Ԏ擾
            if(b==a)++c;
            else c=0;
            if(c>10){flag_sw3=(~a)&0x01;return;}    //10��ǂ�œ��l
            b=a;
    }
}

//�^�N�gSW�̌��o�֐��i��:ON�A��:OFF�A�A��:REN�j
//tact(���o���[�h)
void tact(unsigned short mode)
{
    switch(mode){
            case ON:                           	    	//�����������o
                tactsw();
                if(flag_sw3==0)flag_R=1;                //�t���OR�Z�b�g
                break;
            case OFF:                                   //�����������o
                tactsw();
                if(flag_sw3==1)flag_P=1;                //�t���OP�Z�b�g
                break;
            case REN:                                   //��-�� �A�����o
                tactsw();
                if(flag_sw3==0)flag_R=1;                //�t���OPR�Z�b�g
                if((flag_sw3==1)&&(flag_R==1))flag_P=1;
                break;
            default:                                    //�Y���Ȃ�
                break;
    }
}

//�t���O�N���A�֐��iflag:R�Aflag:P�Aflag:PR) 
//fclr(�N���A�Ώۃt���O�j
void fclr(unsigned short flg)
{
    switch(flg){
                case R:                             //���t���OR�Ώ�
                    flag_R=0;
                    break;
                case P:                             //���t���OP�Ώ�
                    flag_P=0;
                    break;
                case PR:                            //��-���t���OPR�Ώ�
                    flag_P=flag_R=0;
                    break;
                default:                            //�Y���Ȃ�
                    break;
    }
}

//�^�N�gSW����񐔃J�E���g�֐��i��:ON�A��:OFF�j
//kaisu(���o���[�h) 
void kaisu(unsigned short mode1)
{
    switch(mode1){
                case ON:                            //������̏ꍇ
                    tact(ON);
                    if(SW3R){++count;fclr(R);}      //����񐔃J�E���g
                    break;
                case OFF:                           //������̏ꍇ
                    tact(OFF);
                    if(SW3P){++count;fclr(P);}      //����񐔃J�E���g
                    break;
                default:                            //�Y���Ȃ�
                    break;
    }
}

//�^�N�gSW���쎞�ԃJ�E���g�֐��i�����Ă���ԁj
//jikan() 
void jikan(void)
{
    if(SW3==PUSH){++count;}                         //�����Ă���ԑ���
    else{count=0;}                                  //���������N���A
}

//�u�U�[���n�m�֐�
//buzzon() 
void buzzon(void)
{
    unsigned short t;
    for(t=30;t>0;t--){
        BUZZ=1;
    }
    for(t=30;t>0;t--){
        BUZZ=0;
    }
}    

//�u�U�[���n�e�e�֐�
//buzzoff() 
void buzzof(void)
{
unsigned short n,t;
    for(t=30;t>0;t--){
        BUZZ=1;
    }
    for(t=30;t>0;t--){
        BUZZ=1;
    }
}   

void clear(void){
    dynam(SEGD,SEGN,0,0);
    __delay_ms(1000);
    while(1){
        if(SW1 == UP){
            break;
        }
    }

}

//�^�C�~���O�����֐�
//wait(��������);
void waitSEG(unsigned short seg1,unsigned short seg2,unsigned short wt)
{

    wt = wt / 10;
    unsigned int i;

    for(i=wt;i>0;--i){   
        dynam(seg1,seg2,0,0);
    }
}

//�^�C�~���O�����֐�
//wait(��������);
int waitSEGStop(unsigned short seg1,unsigned short seg2,unsigned short wt,int dousa)
{
    //���
    int cpSW1 = SW1;
    int cpSW2 = SW2;
    
    

    wt = wt / 10;
    unsigned int i;

    for(i=wt;i>0;--i){   
        dynam(seg1,seg2,0,0);
        
        tact(ON);

        if(cpSW1 != SW1){
            if((0b100 & dousa) == 0b100){
            
                fclr(PR);
                return 1;
            }
        }
        if(cpSW2 != SW2){
            if((0b010 & dousa) == 0b010){
            
                fclr(PR);
                return 1;
            }
        }
        if(SW3R){
            if((0b001 & dousa) == 0b001){
            
                fclr(PR);
                return 1;
            }
        }
    }

    return 0;
}

void motor (int kakudo,unsigned short dig1data,unsigned short dig2data,unsigned short smdata,unsigned int tr){

    for(int i = kakudo;i>=0;i--){
        dynam(dig1data,dig2data,smdata,tr);
    }
}

int motorStop (int kakudo,unsigned short dig1data,unsigned short dig2data,unsigned short smdata,unsigned int tr,int dousa){

    //���
    int cpSW1 = SW1;
    int cpSW2 = SW2;

    for(int i = kakudo;i>=0;i--){
        dynam(dig1data,dig2data,smdata,tr);

        tact(ON);

        if(cpSW1 != SW1){
            if((0b100 & dousa) == 0b100){
            
                fclr(PR);
                return i;
            }
        }
        if(cpSW2 != SW2){
            if((0b010 & dousa) == 0b010){
            
                fclr(PR);
                return i;
            }
        }
        if(SW3R){
            if((0b001 & dousa) == 0b001){
            
                fclr(PR);
                return i;
            }
        }
    }

    return 0;
}

/*  ���C���֐��i���ɂ��킹�ĕύX�j    */
void main(void)
{
//�����N���b�N�i16MHz�j                                                  
    OSCCON=0x7A; 
//�|�[�g�C�j�V�����C�Y 
    portinit();
//�V�Z�O�����g�����A�k�d�c�����A�X�e�b�s���O���[�^��~
    zenoff();
//���[�J���ϐ���` 
    int i=0,j=0,k=0,m=0,n=0;
//������ԁi�X�^�[�g��j 
    //SW1�F���@SW2�F���@SW3�F����
    while((SW1!=1)||(SW2!=1)||(SW3!=1));  

    //��������ҏW
    LEDR=LEDG=LEDB=OFF;
    
    //�t���O���Z�b�g
    fclr(PR);


     //�ŏ��̋N�����b�N
    while(1){
        if(SW1 == UP){
            break;
        }
    }

    int time = 0;

    int ans = 0;

    unsigned short segL = 0;
    unsigned short segR = 0;

    int ataiL = 0;
    int ataiR = 0;

    int zyoutai = 0;

    //���C���̓���
    hukki:
    while(1){
        
        if(SW1 == UP && SW2 ==DN){
            dynam(SEGU,SEGP,0,0);

            for
        }
        
        if(SW1 ==DN && SW2 ==DN){
            dynam(SEGD,SEGN,0,0);
        }
        
    }
}

