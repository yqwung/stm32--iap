#include <stm32f10x_conf.h>
#include <stm32f10x.h>
#include "includes.h"


DECODE_TABLE usart1_5555_table;
DECODE_TABLE usart2_5555_table;
DECODE_TABLE usart3_5555_table;
DECODE_TABLE usart4_5555_table;
unsigned char update;                 // ���������־
unsigned char toApp = 0;              // ��ת��APP��־i


int main(void)
{         
    unsigned int i=0, j=0;
    unsigned int len_1=0, len_2=0;
    unsigned char rec[1100]={0};
    unsigned char package_ok[1100]={0};
    
    
    //------------------------------------------------------System Init----------------------------------------------------------//
    NVIC_Configuration();
    SystemInit();
    NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x00);      // �����ж�������ָ������Bootloader
    delay_init();
    BSP_LED_BUZZER_Init();      
    BSP_USART_Init();
    BSP_TIM3_Init(9999, 7200);   // ��ʱ1S��toAppDelayS�ڲ��յ�����������Զ���ת��APP
    
    update = 1;                  // û���յ���������
    toApp = 0;                   // ��ת��־               
    
    
    //-------------------------------------------------------USART Test----------------------------------------------------------//
    BSP_USART1_Printf("---This is Bootloader for STM32!\r\n");
    BSP_USART1_Printf("---Build Time : %s %s\r\n", __DATE__, __TIME__);
    BSP_USART1_Printf("---Please Wait %d s ......  \r\n", TO_APP_DELAY);
    BSP_USART2_Printf("---This is Bootloader for STM32!\r\n");
    BSP_USART2_Printf("---Build Time : %s %s\r\n", __DATE__, __TIME__);
    BSP_USART2_Printf("---Please Wait %d s ......  \r\n", TO_APP_DELAY);
    BSP_USART3_Printf("---This is Bootloader for STM32!\r\n");
    BSP_USART3_Printf("---Build Time : %s %s\r\n", __DATE__, __TIME__);
    BSP_USART3_Printf("---Please Wait %d s ......  \r\n", TO_APP_DELAY);
    BSP_USART4_Printf("---This is Bootloader for STM32!\r\n");
    BSP_USART4_Printf("---Build Time : %s %s\r\n", __DATE__, __TIME__);
    BSP_USART4_Printf("---Please Wait %d s ......  \r\n", TO_APP_DELAY);
    
    
    //--------------------------------------------------------while(1)-----------------------------------------------------------//
    while(1)
    {
        // USART1---------------------------------------------------------------------------------------------------- 
#if 1      
        vfifo_from_pp(usart1_vfifo, usart1_ppfifo);
        len_1 = vfifo_out(usart1_vfifo, rec, 1050);
        
        if(len_1>0)
        {
            for(i=0; i<len_1; i++)
            {
                len_2 = CMD_5555_Decode(rec[i], &usart1_5555_table, package_ok); 
                if(len_2>0)
                {                 
#if 0   // ���Դ�ӡ��Ϣ  
                    BSP_USART1_Printf("receive :\r\n"); 
                    for(j=0; j<len_2; j++)
                    {
                        BSP_USART1_Printf(" %02X ", package_ok[j]);    
                    }
                    BSP_USART1_Printf("\r\n"); 
#endif                                
                    iap_agreement_proc(1, package_ok, len_2);  
                }
            }
        }
        len_1 = 0;
        len_2 = 0;      
#endif 
        
        
        // USART2---------------------------------------------------------------------------------------------------- 
#if 1      
        vfifo_from_pp(usart2_vfifo, usart2_ppfifo);
        len_1 = vfifo_out(usart2_vfifo, rec, 1050);
        
        if(len_1>0)
        {
            for(i=0; i<len_1; i++)
            {
                len_2 = CMD_5555_Decode(rec[i], &usart2_5555_table, package_ok); 
                if(len_2>0)
                {                                                
                    iap_agreement_proc(2, package_ok, len_2);  
                }
            }
        }
        len_1 = 0;
        len_2 = 0;      
#endif        
        
 
        // USART3---------------------------------------------------------------------------------------------------- 
#if 1      
        vfifo_from_pp(usart3_vfifo, usart3_ppfifo);
        len_1 = vfifo_out(usart3_vfifo, rec, 1050);
        
        if(len_1>0)
        {
            for(i=0; i<len_1; i++)
            {
                len_2 = CMD_5555_Decode(rec[i], &usart3_5555_table, package_ok); 
                if(len_2>0)
                {                                                
                    iap_agreement_proc(3, package_ok, len_2);  
                }
            }
        }
        len_1 = 0;
        len_2 = 0;      
#endif 


        // USART4---------------------------------------------------------------------------------------------------- 
#if 1      
        vfifo_from_pp(usart4_vfifo, usart4_ppfifo);
        len_1 = vfifo_out(usart4_vfifo, rec, 1050);
        
        if(len_1>0)
        {
            for(i=0; i<len_1; i++)
            {
                len_2 = CMD_5555_Decode(rec[i], &usart4_5555_table, package_ok); 
                if(len_2>0)
                {                                                
                    iap_agreement_proc(4, package_ok, len_2);  
                }
            }
        }
        len_1 = 0;
        len_2 = 0;      
#endif         
        
        
        // ��ת��APP------------------------------------------------------------------------------------------------ 
        if( toApp == 1 )                   // ��ת��APP��־
        {
			toApp = 0;
			//-------------------------------------------------------USART Test----------------------------------------------------------//
			BSP_USART1_Printf("---Jump_To_Application!\r\n");
			BSP_USART2_Printf("---Jump_To_Application!\r\n");
			BSP_USART3_Printf("---Jump_To_Application!\r\n");
			BSP_USART4_Printf("---Jump_To_Application!\r\n");

			USART_Cmd(USART1, DISABLE);    // ʧ�ܴ���
			USART_DeInit(USART1);
			USART_Cmd(USART2, DISABLE);    // ʧ�ܴ���
			USART_DeInit(USART2);
			USART_Cmd(USART3, DISABLE);    // ʧ�ܴ���
			USART_DeInit(USART3);
			USART_Cmd(UART4, DISABLE);     // ʧ�ܴ���
			USART_DeInit(UART4);
			TIM_Cmd(TIM3, DISABLE);        // ʧ�ܶ�ʱ��
			TIM_DeInit(TIM3);              // �����ʱ������

			RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC |
			RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE | RCC_APB2Periph_GPIOF |
			RCC_APB2Periph_GPIOG,  DISABLE);
			//RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, DISABLE);

			ppfifo_free(usart1_ppfifo);    // �ͷŴ�������Ŀռ�
			vfifo_free(usart1_vfifo);      
			ppfifo_free(usart2_ppfifo);    // �ͷŴ�������Ŀռ�
			vfifo_free(usart2_vfifo);   
			ppfifo_free(usart3_ppfifo);    // �ͷŴ�������Ŀռ�
			vfifo_free(usart3_vfifo);   
			ppfifo_free(usart4_ppfifo);    // �ͷŴ�������Ŀռ�
			vfifo_free(usart4_vfifo);

			BSP_IWDG_Init(1600);           // ��ʼ����ֵ9s            
			__set_PRIMASK(1);              // �����ж�

			iap_to_app();                  // ��ת��APPִ��
        }       
    }
}

