#include "iap.h"
#include "bsp_stmflash.h"


typedef        void (*pFunction)(void);     
pFunction      Jump_To_Application;
uint32_t       JumpAddress;


//---------------------------------------------------------------------------------------------------
//    brief     : ��ת��Ӧ�ó���
//    return    : NO
//---------------------------------------------------------------------------------------------------
void iap_to_app(void)
{
    if (((*(__IO uint32_t*)ApplicationAddress) & 0x2FFE0000 ) == 0x20000000)    
    {
        JumpAddress = *(__IO uint32_t*) (ApplicationAddress + 4);                  
        Jump_To_Application = (pFunction) JumpAddress;                              
        __set_MSP(*(__IO uint32_t*) ApplicationAddress);                             
        Jump_To_Application();                                                                 
    }    
}

 
//---------------------------------------------------------------------------------------------------
//    brief    : ��APP����д��ָ��FLASHλ��
//  appxaddr   : FLASH��ַ
//   appbuf    : APP��������
//  appsize    : APP�������ݴ�С�����ٸ�unsigned char��
//   return    : NO
//---------------------------------------------------------------------------------------------------
void iap_write_appbin(uint32_t appxaddr, uint8_t *appbuf, uint32_t appsize)
{
    uint16_t t;
    uint16_t i=0;
    uint16_t temp;
    uint32_t fwaddr=appxaddr;        // ��ǰд��ĵ�ַ
    uint8_t *dfu=appbuf;
    uint16_t iapbuf[1024];
    
    for(t=0; t<appsize; t+=2)
    {						    
        temp = (uint16_t)dfu[1]<<8;
        temp += (uint16_t)dfu[0];	  
        dfu += 2;               // ƫ��2���ֽ�
        iapbuf[i++] = temp;	    
        if(i == 1024)
        {
            i = 0;
            STMFLASH_Write(fwaddr, iapbuf, 1024);	
            fwaddr += 2048;     // ƫ��2048  16=2*8.����Ҫ����2.
        }
    }
    if(i)
        STMFLASH_Write(fwaddr, iapbuf, i); // ������һЩ�����ֽ�д��ȥ.  
}