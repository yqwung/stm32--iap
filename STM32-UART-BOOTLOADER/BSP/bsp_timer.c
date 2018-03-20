#include "bsp_timer.h"	 
#include "sys_config.h"


unsigned int count = 0;
extern unsigned char update;
extern unsigned char toApp;        // ��ת��APP��־


//---------------------------------------------------------------------------------------------------
//    brief     : ͨ�ö�ʱ���жϳ�ʼ��
//     arr      : �Զ���װֵ
//     psc      : ʱ��Ԥ��Ƶ��
//    return    : NO
//---------------------------------------------------------------------------------------------------
void BSP_TIM3_Init(unsigned short arr, unsigned short psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    NVIC_InitTypeDef         NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //ʱ��ʹ��,����ʱ��ѡ��ΪAPB1��2������APB1Ϊ36M
	
    //��ʱ��TIM3��ʼ��
    TIM_TimeBaseStructure.TIM_Period = arr;              //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
    TIM_TimeBaseStructure.TIM_Prescaler = psc;           //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;      //����ʱ�ӷָ�:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);              //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
    
    //�ж����ȼ�NVIC����
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;              //TIM3�ж�
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;    //��ռ���ȼ�0��
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;           //�����ȼ�3��
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;              //IRQͨ����ʹ��
    NVIC_Init(&NVIC_InitStructure);                              //��ʼ��NVIC�Ĵ���
    
    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);                   //ʹ��ָ����TIM3�ж�,��������ж�

    TIM_Cmd(TIM3, ENABLE);                                       //ʹ��TIMx					 
}


//---------------------------------------------------------------------------------------------------
//    brief     : ��ʱ���жϷ������
//    return    : NO
//---------------------------------------------------------------------------------------------------
void TIM3_IRQHandler(void)  
{
    if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  // ���TIM3�����жϷ������
    {
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );   // ���TIMx�����жϱ�־ 
               
        if( update )                      // û���յ���������
        {
            count ++;
            if(count == toAppDelay)       // 1s�ж�һ��
            {
                count = 0;
                toApp = 1;                // �ȴ���ʱ��ת��APP    
                TIM_Cmd(TIM3, DISABLE);
            }
        }
    }
}

