#ifndef __IAP_AGREEMENT_H
#define __IAP_AGREEMENT_H	 


typedef struct _PACKAGE_INFO
{
    unsigned char  package_head[2];          // ͬ��ͷ
    unsigned char  package_len[2];           // ���ܳ�
    unsigned char  package_count[2];         // ������
    unsigned char  package_data[1030];       // ����(�����а���1Byte��CRC),ʵ��1025

}PACKAGE_INFO, *pPACKAGE_INFO;


unsigned int iap_agreement_proc(unsigned char usart_x, unsigned char *buf_in, unsigned int len_in);


#endif