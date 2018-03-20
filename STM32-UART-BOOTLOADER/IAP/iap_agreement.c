#include "iap_agreement.h"
#include "cmd_decode.h"
#include "iap.h"
#include "bsp_usart.h"
#include "cpu.h"


extern unsigned char update;         // ���ֱ�־
extern unsigned char toApp;          // ��ת��APP��־


unsigned int package_count_old=0;    // ���ݰ������ϴ�
unsigned int package_count_new=0;    // ���ݰ���������


unsigned char package_handshake[8] = {0x55, 0x55,   0x00, 0x08,   0x00, 0x00,   0x55,    0xF9};  // ���֣�Ӧ��0xAA�������0xAAȡ����
unsigned char package_ok[8] =        {0x55, 0x55,   0x00, 0x08,   0x00, 0x00,   0xA5,    0x00};  // ���ݰ���ȷ�����Կ�ʼ�´δ���
unsigned char package_error[8] =     {0x55, 0x55,   0x00, 0x08,   0x00, 0x00,   0x5A,    0x00};  // ���ݰ������ش�  
unsigned char package_end[8] =       {0x55, 0x55,   0x00, 0x08,   0x00, 0x00,   0x0A,    0x00};  // ���ݰ�������ϣ�Ӧ��0xF5����������0xF5ȡ����
  

static unsigned int send_to_pc(unsigned char usart_x, unsigned char* data, unsigned int len)
{
    unsigned int send_len=0;
    
    switch(usart_x)
    {
        case 1 :    
            {
                send_len = BSP_USART1_Write(data, len);
            }
            break; 
        case 2 :    
            {
                send_len = BSP_USART2_Write(data, len);
            }
            break; 
        case 3 :    
            {
                send_len = BSP_USART3_Write(data, len);
            }
            break; 
        case 4 :    
            {
                send_len = BSP_USART4_Write(data, len);
            }
            break; 
        default:
            break;            
    }
    
    return send_len;
}


unsigned int iap_agreement_proc(unsigned char usart_x, unsigned char *buf_in, unsigned int len_in)
{
    unsigned int crc=1;
                
    package_count_new = (unsigned int)(((PACKAGE_INFO*)(buf_in))->package_count[0]<<8) | ((PACKAGE_INFO*)(buf_in))->package_count[1];  // ���������
    
    if( len_in == 8 && ((PACKAGE_INFO*)(buf_in))->package_data[0] == 0xAA )       // ��������
    {
        send_to_pc(usart_x, package_handshake, 8);  // Ӧ����������
        
        update = 0;                                 // ���ֳɹ���ֹͣ��ʱ��
        
        package_count_old = 1;
    }
    else if( len_in == 8 && ((PACKAGE_INFO*)(buf_in))->package_data[0] == 0xF5 )  // ������ϣ����һ��
    {                    
        package_end[4] = (unsigned char)(package_count_new>>8);  // ��������8λ
        package_end[5] = (unsigned char)package_count_new;       // ��������8λ    
        package_end[7] = CMD_CRC_Data(package_end, 7);           // CRC

        send_to_pc(usart_x, package_end, 8);        // Ӧ�����ݰ������������
        
        delay_ms(100);
        
        toApp = 1;    // ��ת��APP��־
    }
    else if( len_in == 1031  )                                                    // �ļ����ݴ���
    {           
        if( package_count_new == package_count_old )
        {
            crc = CMD_CRC_Check(buf_in, len_in);  // �������ݰ�У���CRC 
            if(crc == 0)   // CRC��ȷ�����Ϳ��Խ�����һ������
            {                                                        
                iap_write_appbin(ApplicationAddress+1024*(package_count_new-1), ((PACKAGE_INFO*)(buf_in))->package_data, 1024);   // ����һ����д��FLASH                            
                package_ok[4] = (unsigned char)(package_count_old>>8);     // ��������8λ
                package_ok[5] = (unsigned char)package_count_old;          // ��������8λ    
                package_ok[7] = CMD_CRC_Data(package_ok, 7);               // CRC                
                
                send_to_pc(usart_x, package_ok, 8); 
                                                                                               
                package_count_old ++; 
            }
            else          // CRC�����ش���package_count_old��
            {
                package_error[4] = (unsigned char)(package_count_old>>8);  // ��������8λ
                package_error[5] = (unsigned char)package_count_old;       // ��������8λ    
                package_error[7] = CMD_CRC_Data(package_error, 7);         // CRC
                
                send_to_pc(usart_x, package_error, 8);                   
            }
        } 
        else             // �����������ش���package_count_old��
        {
            package_error[4] = (unsigned char)(package_count_old>>8);      // ��������8λ
            package_error[5] = (unsigned char)package_count_old;           // ��������8λ    
            package_error[7] = CMD_CRC_Data(package_error, 7);             // CRC
                
            send_to_pc(usart_x, package_error, 8); 
        }
    }
   
    return 0;
}