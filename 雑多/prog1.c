//�T���v���v���O�����i���O�ۑ�P�j  file:prog1.c

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
    int i=0,j=0,k=0,m=0,n=0;
//������ԁi�X�^�[�g��j 
    //SW1�F���@SW2�F���@SW3�F����
    while((SW1!=1)||(SW2!=1)||(SW3!=1));  

    //��������ҏW
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
