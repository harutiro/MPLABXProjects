//サンプルプログラム（事前課題１）  file:prog1all.c

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

//プリプロセッサー
//Ｉ／Ｏポート関係
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
#define BUZZ    RC0

//表示、スイッチ操作定義
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

//セグメントａ to ｇ表示定義
#define SEG_    0x00
#define SEGa    0x01
#define SEGb    0x02
#define SEGc    0x04
#define SEGd    0x08
#define SEGe    0x10
#define SEGf    0x20
#define SEGg    0x40
#define SEGdp   0x80

//７セグメント数字・文字表示定義
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

//ステッピングモータ励磁定義
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

//計算処理用 8,10,16進数
#define OCT     8
#define DEC    10
#define HEX    16

//動作周波数（16MHz）
#define _XTAL_FREQ 16000000

//遅延時間（delay）
#define __delay(x) _delay((unsigned long)((x)))
#define __delay_us(x) _delay((unsigned long)((x)*(_XTAL_FREQ/4000000UL)))
#define __delay_ms(x) _delay((unsigned long)((x)*(_XTAL_FREQ/4000UL)))

//プロトタイプ宣言
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

//グローバル変数定義
unsigned short flag_sw1,flag_sw2,flag_sw3;
unsigned short flag_R,flag_P;
unsigned short flg;
unsigned short cw=0,ccw=0;
int atai10,atai1,atai;
int count;

//グローバル変数定義（ｎ進変換用）
unsigned short code1000,code100,code10,code1,fugou;

//参照用配列（ｎ進変換用テーブル）
unsigned short samp[]={SEG0,SEG1,SEG2,SEG3,SEG4,SEG5,SEG6,SEG7,SEG8,SEG9,SEGA,SEGB,SEGC,SEGD,SEGE,SEGF};
unsigned short samf[]={SEGF,SEGE,SEGD,SEGC,SEGB,SEGA,SEG9,SEG8,SEG7,SEG6,SEG5,SEG4,SEG3,SEG2,SEG1,SEG0};

//配列データ
unsigned short num0[]={SEG0,SEG1,SEG2,SEG3,SEG4,SEG5,SEG6,SEG7,SEG8,SEG9};  //  num0-9  配列 数字表示
unsigned short gusu[]={SEG0,SEG2,SEG4,SEG6,SEG8};                           //  gusu    配列 偶数表示
unsigned short kisu[]={SEG1,SEG3,SEG5,SEG7,SEG9};                           //  kisu    配列 奇数表示
unsigned short moji[]={SEGA,SEGB,SEGC,SEGD,SEGE,SEGF,SEGH,SEGL,SEGN,SEGP};  //  moji    配列 文字全部
unsigned short moja[]={SEGA,SEGB,SEGC,SEGD,SEGE,SEGF};                      //  moja    配列 文字A to F
unsigned short mojf[]={SEGF,SEGE,SEGD,SEGC,SEGB,SEGA};                      //  mojf    配列 文字F to A
unsigned short mojh[]={SEGH,SEGL};                                          //  mojh    配列 文字H to L
unsigned short mojn[]={SEGN,SEGP};                                          //  mojn    配列 文字N to P

//Ｉ／Ｏポート初期化関数
//portinit();
void portinit(void)
{  
    //アナログ入力無効
    ANSELA=0x00;
    ANSELB=0x00; 
    //ポート設定
    PORTA=0x07;
    TRISA=0x07;
    PORTB=0x00;
    TRISB=0x00;
    PORTC=0x07;
    TRISC=0x00; 
}

//タイミング調整関数
//wait(調整時間);
void wait(unsigned short wt)
{
    unsigned int i,j;
    for(i=10*wt;i>0;--i){   
        for(j=10;j>0;--j);
    }
}


//セグメント表示初期化関数
//hyouji();
void hyouji(void)
{
    DIGIT1=DIGIT2=0;                //７セグメント非表示
    wait(1);
}

//７セグＬＥＤ＆ＬＥＤ＆ステッピングモータ停止関数
//zenoff();                               
void zenoff(void)
{
    LEDR=1;
    LEDB=1;
    LEDG=1;
    DATA=SM_OFF;                    //ステッピングモータ停止
    CLK=0;CLK=1;CLK=0;              //クロック信号出力
}

//７セグメント＆ステッピングモータダイナミック表示駆動関数
//dynam（上位データ、下位データ、駆動励磁、駆動速度）
void dynam(unsigned short dig1data,unsigned short dig2data,unsigned short smdata,unsigned int tr)
{
    unsigned int j=0,tm=0;
    unsigned short ext=0,smd=0;     //切替変数初期化
    CLK=0;
    DATA=dig1data;                  //表示データ格納
    DIGIT1=0;DIGIT2=1;              //セグメント表示制御
    if(tr==L){tm=3000;}             //輝度調整
    else{tm=1000;}
    for(j=tm;j>0;--j); 
    DIGIT1=0;DIGIT2=0;              //７セグメント非表示
    DATA=dig2data;                  //表示データ格納
    DIGIT2=0;DIGIT1=1;              //セグメント表示制御
    if(tr==L){tm=3000;}             //輝度調整
    else{tm=1000;}
    for(j=tm;j>0;--j); 
    DIGIT1=0;DIGIT2=0;              //７セグメント非表示
    switch(smdata){                 //ＳＭ相励磁制御
            case NO:                //無励磁で停止
                DATA=SM_OFF;smd=SEG0;
                CLK=0;CLK=1;CLK=0;  //クロック信号出力
                break;
            case T2:    //２相励磁（時計回り）
                if(flg==1)cw=0;flg=0;
                if((cw==0)&&(ext==0)){cw=1;ccw=0;ext=1;DATA=smd=SM_DA;}
                if((cw==1)&&(ext==0)){cw=2;ccw=3;ext=1;DATA=smd=SM_CD;}
                if((cw==2)&&(ext==0)){cw=3;ccw=2;ext=1;DATA=smd=SM_BC;}
                if((cw==3)&&(ext==0)){cw=0;ccw=1;ext=1;DATA=smd=SM_AB;}
                CLK=0;CLK=1;CLK=0;  //クロック信号出力
                break;
            case H2:    //２相励磁（反時計回り）
                if(flg==1)ccw=0;flg=0;
                if((ccw==0)&&(ext==0)){ccw=1;cw=0;ext=1;DATA=smd=SM_AB;}
                if((ccw==1)&&(ext==0)){ccw=2;cw=3;ext=1;DATA=smd=SM_BC;}
                if((ccw==2)&&(ext==0)){ccw=3;cw=2;ext=1;DATA=smd=SM_CD;}
                if((ccw==3)&&(ext==0)){ccw=0;cw=1;ext=1;DATA=smd=SM_DA;}
                CLK=0;CLK=1;CLK=0;  //クロック信号出力
                break;
            default:    //該当なし
                cw=ccw=0;
                break;
    }
}

//数値データを表示コードに変換関数（値n→SEGn）
//hen7(数値,表示させるｎ進数）※255まで対応
void hen7(int value,unsigned short ns1)
{
    int value100,value10,value1;
    unsigned int l,m,n;
    fugou=0;
    if(value<0)fugou=1;             //＋値:fugou=0，-値:fugou=1
    if(fugou==1){
        value=(~(value-1));         //＋数値に変換(数値-1→ビット反転）
    }
    switch(ns1){
            case DEC:                               //  10進数表示に変換
                value100=value/100;                 // 100位の数値を算出
                value10 =(value-(value100*100))/10; //  10位の数値を算出
                value1  =value%10;                  //   1位の数値を算出
                code1000=SEG0;                      //1000位の表示=0
                for(l=0;l<=2;++l){
                    if(value100==l)code100=samp[l]; // 100位の数値を表示変換
                }
                for(m=0;m<=9;++m){
                    if(value10==m)code10=samp[m];   //  10位の数値を表示変換
                }
                for(n=0;n<=9;++n){
                    if(value1==n)code1=samp[n];     //   1位の数値を表示変換
                }
                break;
            case OCT:                               //   8進数表示に変換
                value100=value>>6;                  // 100位の数値を算出
                value10 =(value-(value100<<6))>>3;  //  10位の数値を算出
                value   =value%8;                   //   1位の数値を算出
                code1000=SEG0;                      //1000位の表示=0
                for(l=0;l<=3;++l){
                    if(value100==l)code100=samp[l]; // 100位の数値を表示変換
                }
                for(m=0;m<=7;++m){
                    if(value10==m)code10=samp[m];   //  10位の数値を表示変換
                }
                for(n=0;n<=7;++n){
                    if(value1==n)code1=samp[n];     //   1位の数値を表示変換
                }
                break;
            case HEX:                               //  16進表示に変換
                code1000=SEG0;                      //1000位の表示=0
                code100=SEG0;                       // 100位の表示=0
                value10 =value>>4;                  //  10位の数値を算出
                value1  =value%16;                  //   1位の数値を算出
                for(m=0;m<=15;++m){
                    if(value10==m)code10=samp[m];   //  10位の数値を表示変換
                }
                for(n=0;n<=15;++n){
                    if(value1==n)code1=samp[n];     //   1位の数値を表示変換
                }
                break;
            default:
                break;
    }
}
//表示コードを数値データに変換関数（SEGn→値n）
//hens(表示10桁,表示1桁,元のｎ進数）※表示FF対応
void hens(unsigned short keta10,unsigned short keta1,unsigned short ns2)
{
    unsigned int m,n;
    keta10=keta10;                                  //10桁のDPを削除
    keta1=keta1;                                    // 1桁のDPを削除
    if((ns2==DEC)||(ns2==HEX)){                     //10進数:DEC、16進数:HEX
        for(m=0;m<=15;++m){
            if(keta10==samp[m])atai10=m;            //10桁の表示を数値に変換
        }
        for(n=0;n<=15;++n){
            if(keta1 ==samp[n])atai1=n;             // 1桁の表示を数値に変換
        }
        if(keta10==SEG_)atai10=0;                   //10桁が非表示時、数値0
        if(keta1 ==SEG_)atai1 =0;                   // 1桁が非表示時、数値0
        if(ns2==DEC){atai=((atai10*10)+atai1);}     //10進数を算出
        if(ns2==HEX){atai=((atai10*16)+atai1);}     //16進数を算出
    }
}

//チャタ防止＆状態取得関数（押：0、離：1）
//tactsw()
void tactsw(void)
{
    unsigned short a,b,c;
    b=c=0;
    while(1){
            a=SW3;                                  //SW3状態取得
            if(b==a)++c;
            else c=0;
            if(c>10){flag_sw3=(~a)&0x01;return;}    //10回読んで同値
            b=a;
    }
}

//タクトSWの検出関数（押:ON、離:OFF、連続:REN）
//tact(検出モード)
void tact(unsigned short mode)
{
    switch(mode){
            case ON:                           	    	//押した時検出
                tactsw();
                if(flag_sw3==0)flag_R=1;                //フラグRセット
                break;
            case OFF:                                   //離した時検出
                tactsw();
                if(flag_sw3==1)flag_P=1;                //フラグPセット
                break;
            case REN:                                   //押-離 連続検出
                tactsw();
                if(flag_sw3==0)flag_R=1;                //フラグPRセット
                if((flag_sw3==1)&&(flag_R==1))flag_P=1;
                break;
            default:                                    //該当なし
                break;
    }
}

//フラグクリア関数（flag:R、flag:P、flag:PR) 
//fclr(クリア対象フラグ）
void fclr(unsigned short flg)
{
    switch(flg){
                case R:                             //押フラグR対象
                    flag_R=0;
                    break;
                case P:                             //離フラグP対象
                    flag_P=0;
                    break;
                case PR:                            //押-離フラグPR対象
                    flag_P=flag_R=0;
                    break;
                default:                            //該当なし
                    break;
    }
}

//タクトSW操作回数カウント関数（押:ON、離:OFF）
//kaisu(検出モード) 
void kaisu(unsigned short mode1)
{
    switch(mode1){
                case ON:                            //押操作の場合
                    tact(ON);
                    if(SW3R){++count;fclr(R);}      //操作回数カウント
                    break;
                case OFF:                           //離操作の場合
                    tact(OFF);
                    if(SW3P){++count;fclr(P);}      //操作回数カウント
                    break;
                default:                            //該当なし
                    break;
    }
}

//タクトSW操作時間カウント関数（押している間）
//jikan() 
void jikan(void)
{
    if(SW3==PUSH){++count;}                         //押している間増分
    else{count=0;}                                  //離した時クリア
}

//ブザーをＯＮ関数
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

//ブザーをＯＦＦ関数
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

//タイミング調整関数
//wait(調整時間);
void waitSEG(unsigned short seg1,unsigned short seg2,unsigned short wt)
{

    wt = wt / 10;
    unsigned int i;

    for(i=wt;i>0;--i){   
        dynam(seg1,seg2,0,0);
    }
}

//タイミング調整関数
//wait(調整時間);
int waitSEGStop(unsigned short seg1,unsigned short seg2,unsigned short wt,int dousa)
{
    //状態
    int cpSW1 = SW1;
    int cpSW2 = SW2;
    int cpSW3 = SW3;
    
    

    wt = wt / 10 + wt/1000*60;
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
        if(cpSW3 != SW3){
            if((0b001 & dousa) == 0b001){
            
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

    //状態
    int cpSW1 = SW1;
    int cpSW2 = SW2;
    int cpSW3 = SW3;

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
        if(cpSW3 != SW3){
            if((0b001 & dousa) == 0b001){
            
                return i;
            }
        }
    }

    return 0;
}

void buzz (int dousa,int time){

    int i = 0;
    int j = 0;

    for(j=time;j>0;j--){

        if(dousa == 1){
            for(i=0;i<=2500;i++){
                buzzon();
            }
            for(i=0;i<=2500;i++){
                buzzof();
            }
        }

        if(dousa == 5){
            for(i=0;i<=10000;i++){
                buzzon();
            }
            for(i=0;i<=10000;i++){
                buzzof();
            }
        }

    }
    
    
}

int buzzStop (int dousa,int time,int rokku){

    int i = 0;
    int j = 0;

    //状態
    int cpSW1 = SW1;
    int cpSW2 = SW2;
    int cpSW3 = SW3;

    for(j=time;j>0;j--){

        if(dousa == 1){
            for(i=0;i<=2500;i++){
                buzzon();
                
                tact(ON);

                if(cpSW1 != SW1){
                    if((0b100 & rokku) == 0b100){
                    
                        fclr(PR);
                        buzzof();
                        return j;
                    }
                }
                if(cpSW2 != SW2){
                    if((0b010 & rokku) == 0b010){
                    
                        fclr(PR);
                        buzzof();
                        return j;
                    }
                }
                if(SW3R){
                    if((0b001 & rokku) == 0b001){
                    
                        fclr(PR);
                        buzzof();
                        return j;
                    }
                }
                if(SW3 != cpSW3){
                    if((0b001 & rokku) == 0b001){
                        
                        buzzof();
                        return j;
                    }
                }
            }
            for(i=0;i<=2500;i++){
                buzzof();

                tact(ON);

                if(cpSW1 != SW1){
                    if((0b100 & rokku) == 0b100){
                    
                        fclr(PR);
                        buzzof();
                        return j;
                    }
                }
                if(cpSW2 != SW2){
                    if((0b010 & rokku) == 0b010){
                    
                        fclr(PR);
                        buzzof();
                        return j;
                    }
                }
                if(SW3R){
                    if((0b001 & rokku) == 0b001){
                    
                        fclr(PR);
                        buzzof();
                        return j;
                    }
                }
                if(SW3 != cpSW3){
                    if((0b001 & rokku) == 0b001){
                        
                        buzzof();
                        return j;
                    }
                }
            }
        }

        if(dousa == 5){
            for(i=0;i<=7500;i++){
                buzzon();

                tact(ON);

                if(cpSW1 != SW1){
                    if((0b100 & rokku) == 0b100){
                    
                        fclr(PR);
                        buzzof();
                        return j;
                    }
                }
                if(cpSW2 != SW2){
                    if((0b010 & rokku) == 0b010){
                    
                        fclr(PR);
                        buzzof();
                        return j;
                    }
                }
                if(SW3R){
                    if((0b001 & rokku) == 0b001){
                    
                        fclr(PR);
                        buzzof();
                        return j;
                    }
                }
                if(SW3 != cpSW3){
                    if((0b001 & rokku) == 0b001){
                        
                        buzzof();
                        return j;
                    }
                }
            }
            for(i=0;i<=7500;i++){
                buzzof();

                tact(ON);

                if(cpSW1 != SW1){
                    if((0b100 & rokku) == 0b100){
                    
                        fclr(PR);
                        buzzof();
                        return j;
                    }
                }
                if(cpSW2 != SW2){
                    if((0b010 & rokku) == 0b010){
                    
                        fclr(PR);
                        buzzof();
                        return j;
                    }
                }
                if(SW3R){
                    if((0b001 & rokku) == 0b001){
                    
                        fclr(PR);
                        buzzof();
                        return j;
                    }
                }
                if(SW3 != cpSW3){
                    if((0b001 & rokku) == 0b001){
                        
                        buzzof();
                        return j;
                    }
                }
            }
        }

    }
    buzzof();
    return 0;
    
    
}

/*  メイン関数（問題にあわせて変更）    */
void main(void)
{
//内部クロック（16MHz）                                                  
    OSCCON=0x7A; 
//ポートイニシャライズ 
    portinit();
//７セグメント消灯、ＬＥＤ消灯、ステッピングモータ停止
    zenoff();
//ローカル変数定義 
    int i=0,j=0,k=0,m=0,n=0;
//初期状態（スタート後） 
    //SW1：下　SW2：下　SW3：離す
    while((SW1!=1)||(SW2!=1)||(SW3!=1));  

// ================================================================================

    hukki:

    //ここから編集
    LEDR=LEDG=LEDB=OFF;
    
    //フラグリセット
    fclr(PR);


    // //最初の起動ロック
    // while(1){
    //     if(SW1 == UP || SW2 ==UP){
    //         break;
    //     }
    // }

    int time = 0;

    int ans = 0;

    unsigned short segL = 0;
    unsigned short segR = 0;

    int ataiL = 0;
    int ataiR = 0;

    int zyoutai = 0;

    
    //メインの動き
    
    while(1){

        dynam(segL,0,0,0);

        kaisu(ON);

        switch(count){
            case 1: segL = SEGd;
                    break;

            case 2: segL = SEGa;
                    break;

            case 3: segL = SEGa|SEGd;
                    count = 1;
                    break;

            default: break;

        }

        if(segL != 0 && SW1 ==UP &&SW2 ==DN){
            jikan();
            if(count >= 500){
                waitSEG(segL,SEGc|SEGe|SEGg,1000);
                waitSEG(segL,0,1000);
                
                zyoutai = 1
            }
            
        }

        if(segL != 0 && SW1 ==DN &&SW2 ==UP){
            jikan();
            if(count >= 500){
                waitSEG(segL,SEGb|SEGf|SEGg,1000);
                waitSEG(segL,0,1000);

                zyoutai = 2


            }
        }

        if(segL != 0 && SW1 ==UP &&SW2 ==UP){
            jikan();
            if(count >= 500){
                waitSEG(segL,SEGb|SEGc|SEGe|SEGf,1000);
                waitSEG(segL|SEGa,0,1000);

                zyoutai = 3


            }

        }
        
        if(segL != 0 && SW1 ==DN && SW2 ==DN && zyoutai != 0){
            
            
        }

    }
}

