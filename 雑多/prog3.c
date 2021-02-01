//�T���v���v���O�����i���O�ۑ�R�j  file:prog3.c

#include <xc.h>
#include <pic16f1938.h>
#include "mono2020.h"

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
    int c=0,i=0,j=0,k=0,m=0,n=0,l=0,mode=0,exit=0;
//������ԁi�X�^�[�g��j 
    //SW1�F���@SW2�F���@SW3�F����
    while((SW1!=1)||(SW2!=1)||(SW3!=1));  
    //__delay_ms(2000);
    
    //��������ҏW   
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
