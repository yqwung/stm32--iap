#include "bsp_led_buzzer.h"


// LED1-->PA0  LED2-->PA1  �����ӷ�
// BUZZER-->PB5
void BSP_LED_BUZZER_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
 	
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOG , ENABLE);  //ʹ��PA,PG�˿�ʱ��

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 ;// LED1-->PA0  LED2-->PA1
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	   //�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;      
    GPIO_Init(GPIOA, &GPIO_InitStructure);                 
   
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;	           //BUZZER-->PB5 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	   //�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;      
    GPIO_Init(GPIOG, &GPIO_InitStructure);                 

    //GPIO_SetBits(GPIOA,GPIO_Pin_0);                        //LED1Ϩ��	
    GPIO_SetBits(GPIOA,GPIO_Pin_1);                        //LED2Ϩ��		
    GPIO_ResetBits(GPIOG, GPIO_Pin_8);			   //PB5 ����� �رշ�����
    
    GPIO_ResetBits(GPIOA,GPIO_Pin_0);
    //GPIO_ResetBits(GPIOA,GPIO_Pin_1);
}

