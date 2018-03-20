#include "cpu.h"

#if SYSTEM_SUPPORT_UCOS
#include "includes.h"					//ucos ʹ��	  
#endif
	 
static u8  fac_us=0; //us��ʱ������
static u16 fac_ms=0; //ms��ʱ������

//#ifdef OS_CRITICAL_METHOD 	//���OS_CRITICAL_METHOD������,˵��ʹ��ucosII��.
//systick�жϷ�����,ʹ��ucosʱ�õ�
//void SysTick_Handler(void)
//{				   
    //OSIntEnter();		//�����ж�
    //OSTimeTick();             //����ucos��ʱ�ӷ������               
    //OSIntExit();              //���������л����ж�
//}
//#endif


void NVIC_Configuration(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	  //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
}

//��ʼ���ӳٺ���
//��ʹ��ucos��ʱ��,�˺������ʼ��ucos��ʱ�ӽ���
//SYSTICK��ʱ�ӹ̶�ΪHCLKʱ�ӵ�1/8
//SYSCLK:ϵͳʱ��
void delay_init(void)	 
{

#ifdef OS_CRITICAL_METHOD 	//���OS_CRITICAL_METHOD������,˵��ʹ��ucosII��.
	u32 reload;
#endif
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);	//ѡ���ⲿʱ��  HCLK/8
	fac_us=SystemCoreClock/8000000;	//Ϊϵͳʱ�ӵ�1/8  
	 
#ifdef OS_CRITICAL_METHOD 	//���OS_CRITICAL_METHOD������,˵��ʹ��ucosII��.
	reload=SystemCoreClock/8000000;		//ÿ���ӵļ������� ��λΪK	   
	reload*=1000000/OS_TICKS_PER_SEC;//����OS_TICKS_PER_SEC�趨���ʱ��
							//reloadΪ24λ�Ĵ���,���ֵ:16777216,��72M��,Լ��1.86s����	
	fac_ms=1000/OS_TICKS_PER_SEC; //����ucos������ʱ�����ٵ�λ	   
	SysTick->CTRL|=SysTick_CTRL_TICKINT_Msk;   	//����SYSTICK�ж�
	SysTick->LOAD=reload; 	      //ÿ1/OS_TICKS_PER_SEC���ж�һ��	
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk;   	//����SYSTICK    
#else
	fac_ms=(u16)fac_us*1000;      //��ucos��,����ÿ��ms��Ҫ��systickʱ����   
#endif
}								    

#ifdef OS_CRITICAL_METHOD	//ʹ����ucos
//��ʱnus
//nusΪҪ��ʱ��us��.		    								   
void delay_us(u32 nus)
{		
    u32 ticks;  
    u32 told,tnow,tcnt=0;
    u32 reload=SysTick->LOAD;	//LOAD��ֵ	
    
    ticks=nus*fac_us; 		//��Ҫ�Ľ�����	  		 
    tcnt=0;
    told=SysTick->VAL;        	//�ս���ʱ�ļ�����ֵ
    while(1)
    {
        tnow=SysTick->VAL;	
        if(tnow!=told)
        {	    
            if(tnow<told)tcnt+=told-tnow; //����ע��һ��SYSTICK��һ���ݼ��ļ������Ϳ�����.
            else tcnt+=reload-tnow+told;	    
            told=tnow;
            if(tcnt>=ticks)break;         //ʱ�䳬��/����Ҫ�ӳٵ�ʱ��,���˳�.
        }  
    }; 									    
}

//��ʱnms
//nms:Ҫ��ʱ��ms��
void delay_ms(u16 nms)
{	
    if(OSRunning==TRUE)            //���os�Ѿ�������	    
    {		  
        if(nms>=fac_ms)            //��ʱ��ʱ�����ucos������ʱ������ 
        {
            OSTimeDly(nms/fac_ms); //ucos��ʱ
        }
        nms%=fac_ms;		   //ucos�Ѿ��޷��ṩ��ôС����ʱ��,������ͨ��ʽ��ʱ    
    }
    delay_us((u32)(nms*1000));	   //��ͨ��ʽ��ʱ,��ʱucos�޷���������.
}

#else     //����ucosʱ

//��ʱnus
//nusΪҪ��ʱ��us��.		    								   
void delay_us(u32 nus)
{		
    u32 temp;	
    
    SysTick->LOAD=nus*fac_us;                //ʱ�����	  		 
    SysTick->VAL=0x00;                       //��ռ�����
    SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ; //��ʼ����	 
    do
    {
        temp=SysTick->CTRL;
    }
    while(temp&0x01&&!(temp&(1<<16)));       //�ȴ�ʱ�䵽��   
    SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk; //�رռ�����
    SysTick->VAL =0X00;                      //��ռ�����	 
}

//��ʱnms
//ע��nms�ķ�Χ
//SysTick->LOADΪ24λ�Ĵ���,����,�����ʱΪ:
//nms<=0xffffff*8*1000/SYSCLK
//SYSCLK��λΪHz,nms��λΪms
//��72M������,nms<=1864 
void delay_ms(u16 nms)
{	 		  	  
    u32 temp;	
    
    SysTick->LOAD=(u32)nms*fac_ms;           //ʱ�����(SysTick->LOADΪ24bit)
    SysTick->VAL =0x00;                      //��ռ�����
    SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ; //��ʼ����  
    do
    {
        temp=SysTick->CTRL;
    }
    while(temp&0x01&&!(temp&(1<<16)));       //�ȴ�ʱ�䵽��   
    SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk; //�رռ�����
    SysTick->VAL =0X00;                      //��ռ�����	  	    
}

#endif

/**************************************************************************************************************************/
//���Ź���ʼ��
void Wdt_Init(void)
{
    // Enable write access to IWDG_PR and IWDG_RLR registers 
    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);  //IWDG->KR = 0x5555  �����Ź��Ĵ���д�빦��
    
    // IWDG counter clock: 40KHz(LSI) / 256 = 156 Hz(6.4ms) 
    IWDG_SetPrescaler(IWDG_Prescaler_256);   //IWDG->PR = 0x06;  ���Ź�ʱ�ӷ�Ƶ
    
    // Set counter reload value to 1250   5s/6.4ms=781
    IWDG_SetReload(781);           //IWDG->RLR =0xFFF    ι��ʱ��
    
    // Red IWDG counter
    IWDG_ReloadCounter();   //IWDG->KR = 0xAAAA
    
    // Enable IWDG (the LSI oscillator will be enabled by hardware) 
    IWDG_Enable();   //IWDG->KR = 0xCCCC  �������Ź�
}

//ι�� 
void Kick_Dog(void)
{
    //Reload IWDG counter  
    IWDG_ReloadCounter();   //IWDG->KR = 0xAAAA  ʹ  IWDG->RLR�е�ֵ���¼��ص��������У�����������Ź���λ       
}

/**************************************************************************************************************************/
//�����λ
void SoftReset(void) 
{  
    __set_FAULTMASK(1);      // �ر������ж�
    NVIC_SystemReset();      // ��λ
}

/**************************************************************************************************************************/




void BSP_IWDG_Init(uint16_t ms)
{
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	IWDG_SetPrescaler(IWDG_Prescaler_256);//40K/256=156HZ(6.4ms)
	//IWDG_SetReload(((ms>>5)*5) & 0x0FFF);
	// xs /6.4 = ms   xs: ���ι��ʱ�� ms:��ʼ����ֵ
	IWDG_SetReload((ms) & 0x0FFF);
	IWDG_ReloadCounter();
	IWDG_Enable();
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Disable);
}

void BSP_IWDG_Reset(void)
{
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	IWDG_ReloadCounter();
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Disable);
}

void BSP_SoftReset(void)
{
	__set_FAULTMASK(1); 	 // �ر������ж�	
	NVIC_SystemReset();// ��λ
}