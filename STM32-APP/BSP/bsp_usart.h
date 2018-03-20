/*
 * File      : usart.h
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2009, RT-Thread Development Team
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rt-thread.org/license/LICENSE
 *
 * Change Logs:
 * Date           Author       Notes
 * 2009-01-05     Bernard      the first version
 */

#ifndef __BSP_USART_H__
#define __BSP_USART_H__

#include  <stm32f10x_conf.h>
#include  "vfifo.h"
#include  "PPfifo.h"

#define	  USING_UART1
#define   USING_UART2
#define	  USING_UART3
#define	  USING_UART4

#define	  BAUD         115200


extern ppfifo_t*    usart1_ppreceive;    
extern kfifo_t*     usart1_receive;

extern ppfifo_t*    usart2_ppreceive;    
extern kfifo_t*     usart2_receive;

extern ppfifo_t*    usart3_ppreceive;    
extern kfifo_t*     usart3_receive;

extern ppfifo_t*    usart4_ppreceive;   
extern kfifo_t*     usart4_receive;    


void BSP_USART_Init(void);     //4·���ڳ�ʼ��

/*
 * USART1
 */
unsigned int BSP_USART1_Write(char* data, unsigned int len);
unsigned int BSP_USART1_Printf(char* fmt, ...);
//void BSP_USART1_ISR(void);


/*
 * USART2
 */
unsigned int BSP_USART2_Write(char* data, unsigned int len);
unsigned int BSP_USART2_Printf(char* fmt, ...);
//void BSP_USART2_ISR(void);


/*
 * USART3
 */
unsigned int BSP_USART3_Write(char* data, unsigned int len);
unsigned int BSP_USART3_Printf(char* fmt, ...);
//void BSP_USART3_ISR(void);


/*
 * UART4
 */
unsigned int BSP_USART4_Write(char* data, unsigned int len);
unsigned int BSP_USART4_Printf(char* fmt, ...);
//void BSP_USART4_ISR(void);


#endif

/***************************************************************************************************
* ����һ������    USART_SendData(USART1,data); 
* ���Ͷ������    unsigned int BSP_USART4_Write(char* data, unsigned int len);
* �����ı��ַ���  unsigned int BSP_USART4_Printf(char* fmt, ...); 
***************************************************************************************************/

/***************************************************************************************************
*��������         ���յ��������ڽ����ж�ʱ����ƹ��FIFO��ppfifo_in(usart1_ppreceive, &chr, 1);��
*                 ��ʱ�䴦����յ�������ʱ�������ݴ�ƹ��FIFO�Ƶ�FIFO
*                 ��len = kfifo_lcpp_in(usart1_receive,usart1_ppreceive);��
*                 Ȼ���usart1_receive��ȡ���ݣ�kfifo_out(usart1_receive,&buf,1) ��
***************************************************************************************************/