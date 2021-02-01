//サンプルプログラム（事前課題３）  file:prog3.c

#include <xc.h>
#include <pic16f1938.h>
#include "mono2020.h"

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
    int c=0,i=0,j=0,k=0,m=0,n=0,l=0,mode=0,exit=0;
//初期状態（スタート後） 
    //SW1：下　SW2：下　SW3：離す
    while((SW1!=1)||(SW2!=1)||(SW3!=1));  
    //__delay_ms(2000);
    
    //ここから編集   
    while(1){
        dynam(SEG_,SEG_,SM_OFF,H);
        while(SW1==UP){
            dynam(SEG1,SEGU,SM_OFF,H);
            tact(ON);
            if(SW3R){
                for(n=100;n>0;n--){
                    dynam(SEG1,SEGU,SM_OFF,H);     
                }
                for(n=120;n>0;n--){
                    dynam(SEG1,SEGU,T2,L);
                }               
                mode=1;break;
            }
        }
        fclr(R);
        while(SW2==UP){
            dynam(SEGU,SEG2,SM_OFF,H);
            tact(OFF);
            if(SW3P){
                 for(n=200;n>0;n--){
                    dynam(SEGU,SEG2,SM_OFF,H);     
                }
                for(n=240;n>0;n--){
                    dynam(SEGU,SEG2,H2,H);
                }    
                mode=2;break;
            }
        }
        fclr(P);
        while((mode==1)||(mode==2)){
            if(mode==1){dynam(SEG1,SEGU,SM_OFF,H);}
            else{dynam(SEGU,SEG2,SM_OFF,H);}
            if((SW1==DN)&&(SW2==DN)){mode=0;exit=1;break;}
        }
        while(exit==1){
            dynam(SEGD,SEGD,SM_OFF,H);
            jikan();
            if(count>400){exit=0;break;}
        }
    }    
}
