//サンプルプログラム（事前課題１）  file:prog1.c

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
    int i=0,j=0,k=0,m=0,n=0;
//初期状態（スタート後） 
    //SW1：下　SW2：下　SW3：離す
    while((SW1!=1)||(SW2!=1)||(SW3!=1));  

    //ここから編集
    LEDR=LEDG=LEDB=OFF;
    while(SW2==DN);
    while(1){
        if(SW2==UP){
            LEDR=ON;LEDG=OFF;LEDB=OFF;
        }
        else{
            LEDR=OFF;LEDG=OFF;LEDB=ON;
        }
    }
}
