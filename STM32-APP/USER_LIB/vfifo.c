#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "vfifo.h"

#pragma pack (1)

kfifo_t* kfifo_alloc(unsigned int vsize)   //����һ��ѭ������
{
    kfifo_t* fifo;	
    
    fifo=(kfifo_t*)((unsigned char*)malloc(vsize+sizeof(kfifo_t)));   //�û�����Ĵ�С���϶���ͷ�Ĵ�С;
       
    fifo->data=sizeof(kfifo_t)+(unsigned char*)fifo;
    fifo->fifo_size=vsize;
    fifo->unused_size=vsize;
    fifo->in_addr=fifo->data;
    fifo->out_addr=fifo->data;
    fifo->read_lock=0;
    fifo->write_lock=0;
    memset((unsigned char *)fifo->data,0,vsize);
    
    return fifo;	
}

void kfifo_free(kfifo_t *fifo)    //�ͷ�һ��ѭ������
{	
    if(fifo)
    {
        kfree(fifo);
    }
}

unsigned int kfifo_in(kfifo_t *fifo,unsigned char *buf, unsigned int len)   //������
{
    unsigned int l,recycl;
    
    //LOCK
    if((fifo->write_lock > 0) ||(fifo->read_lock > 0))
    {
        return 0;
    }
    fifo->write_lock = 1;
    
    if(len<=0) 
    {
        fifo->write_lock = 0;
        
        return 0;
    }
    
    
    l=kmin(fifo->unused_size,len);  //�ж�Ҫд��������Ƿ񳬳����ռ�;
    fifo->unused_size=fifo->unused_size-l;
    //���������ֻд��һ����;
    
    //���û�е�����β������ȫ��д��;
    if((l+fifo->in_addr)<(fifo->fifo_size+fifo->data)) 
    {
        memcpy(fifo->in_addr,buf,l);	//��������;
        fifo->in_addr=fifo->in_addr+l;  //��ָ���ƶ�;
    }
    else
    {
        recycl=fifo->fifo_size+fifo->data-fifo->in_addr;
        memcpy(fifo->in_addr,buf,recycl);	//��������ǰ�벿��;
        memcpy(fifo->data,buf+recycl,l-recycl);
        fifo->in_addr=fifo->data+l-recycl;  //��ָ���ƶ�;   
    }
    
    fifo->write_lock = 0;
    
    return l;
}


unsigned int kfifo_from_pp(kfifo_t *fifo,ppfifo_t *ppfifo)   //��ƹ�����������Ƶ� vFIFO
{
    unsigned int l=0, recycl=0;
    unsigned int len_1 = 0;
    unsigned int len_2 = 0;
    
    len_1 = ppfifo_out(ppfifo,ppfifo->ppdata);
    
    len_2 = ppfifo_out(ppfifo,(ppfifo->ppdata + len_1));
    if((len_1 == 0)&&(len_2 == 0))
    {
        return 0;
    }
        
    l=kmin(fifo->unused_size,len_1 + len_2);  //�ж�Ҫд��������Ƿ񳬳����ռ�;
    fifo->unused_size=fifo->unused_size-l;
    
    if((l+fifo->in_addr)<(fifo->fifo_size+fifo->data)) 
    {
        memcpy(fifo->in_addr,ppfifo->ppdata,l);	//��������;
        fifo->in_addr=fifo->in_addr+l;  //��ָ���ƶ�;
    }
    else
    {
        recycl=fifo->fifo_size+fifo->data-fifo->in_addr;
        memcpy(fifo->in_addr,ppfifo->ppdata,recycl);	//��������ǰ�벿��;
        memcpy(fifo->data,ppfifo->ppdata+recycl,l-recycl);
        fifo->in_addr=fifo->data+l-recycl;  //��ָ���ƶ�;  
    }
    
    return l;
}


unsigned int kfifo_out(kfifo_t *fifo,unsigned char *buf, unsigned int len)    //������
{
    unsigned int l,recycl;
    
    
    if((fifo->write_lock > 0) ||(fifo->read_lock > 0))
    {
        return 0;
    }
    fifo->read_lock = 1;
    if(len<=0) 
    {
        fifo->read_lock = 0;
        
        return 0;
    }
    ///////////////////////;
    
    l=kmin(fifo->fifo_size-fifo->unused_size,len);
    fifo->unused_size=fifo->unused_size+l;
    //  l  ���ܹ���ȡ��ȥ�Ĵ�С;
    
    //���û�е�����β������ȫ����ȡ;
    if((l+fifo->out_addr)<(fifo->fifo_size+fifo->data)) 
    {
        memcpy(buf,fifo->out_addr,l);	//��������;
        //memset(fifo->out_addr,0,l);
        fifo->out_addr=fifo->out_addr+l;  //��ָ���ƶ�;
    }
    else
    {
        recycl=fifo->fifo_size+fifo->data-fifo->out_addr;
        memcpy(buf,fifo->out_addr,recycl);	//��������ǰ�벿��;
        //memset(fifo->out_addr,0,recycl);
        memcpy(buf+recycl,fifo->data,l-recycl);
        //memset(fifo->data,0,l-recycl);
        fifo->out_addr=l-recycl+fifo->data;  //��ָ���ƶ�;
    }
    fifo->read_lock = 0;
    
    return l;
}


#pragma pack ()

#ifdef __cplusplus
}
#endif

