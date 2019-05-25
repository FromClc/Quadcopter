#include "nrf24l01.h"

static uint8_t TX_ADDRESS[5]={0x1F,0x2E,0x3D,0x4C,0x5B};//本机地址
static uint8_t RX_ADDRESS[5]={0x1F,0x2E,0x3D,0x4C,0x5B};//接收地址

uint8_t NRF24L01_RXDATA[32];

uint8_t NRF24L01_TXDATA[32];

void NRF_Mode(uint8_t Chanal,uint8_t mode ){
	NRF_CE_L;
	//NRF_CSN_L;
	NRF_Write_Reg(FLUSH_TX,0xff);     //清空发送缓冲区
	NRF_Write_Reg(FLUSH_RX,0xff);		//清空接收缓冲区
	NRF_Write_Buf(NRF_WRITE_REG+TX_ADDR,TX_ADDRESS,5);//写TX节点地址
	NRF_Write_Buf(NRF_WRITE_REG+RX_ADDR_P0,RX_ADDRESS,5);//写RX节点地址
	NRF_Write_Reg(NRF_WRITE_REG+EN_AA,0x01);   //使能通道0的自动应答
	NRF_Write_Reg(NRF_WRITE_REG+EN_RXADDR,0x01); //使能通道0的自动应答
	NRF_Write_Reg(NRF_WRITE_REG+SETUP_RETR,0x1a);   
	                   //设置自动重发间隔时间：500微妙；最大自动重发次数：10
	NRF_Write_Reg(NRF_WRITE_REG+RF_CH,Chanal); //设置RF通道为Chanal
	NRF_Write_Reg(NRF_WRITE_REG+RX_PW_P0,32); //设置通道0的有效数据宽度
	NRF_Write_Reg(NRF_WRITE_REG+RF_SETUP,0x0f);
						//设置TX发射参数，0db增益，2Mbps,使能低噪声增益
	if(mode==TX)
		NRF_Write_Reg(NRF_WRITE_REG+CONFIG,0x0e);//发送模式
	else if(mode==RX)
		NRF_Write_Reg(NRF_WRITE_REG+CONFIG,0x0f);//接收模式
	NRF_CE_H;
}

//通过SPI写寄存器
uint8_t NRF_Write_Reg(uint8_t regaddr,uint8_t data)
{
    uint8_t status;
    NRF_CSN_L;                                         //使能SPI传输                                             
    status =SPI1_RW(regaddr);                         //发送寄存器号 
    SPI1_RW(data);                                    //写入寄存器的值
	                                                            
    NRF_CSN_H;                                         //禁止SPI传输
                                                                
    return(status);       		                          //返回状态值
}
//读取SPI寄存器值 ，regaddr:要读的寄存器
uint8_t NRF_Read_Reg(uint8_t regaddr)
{
	   uint8_t reg_val;
 	   NRF_CSN_L;                                         //使能SPI传输	
	                                                            
  	SPI1_RW(regaddr);                                   //发送寄存器号
  	reg_val=SPI1_RW(0);                             //读取寄存器内容
                                                                
  	NRF_CSN_H;                                        //禁止SPI传输	
                                                                
  	return(reg_val);                                 //返回状态值
}	

//上电检测NRF24L01是否在位
//写5个数据然后再读回来进行比较，相同时返回值:0，表示在位;否则返回1，表示不在位	
uint8_t NRF24L01_Check(void)
{
	uint8_t buf[5]={0xA5,0xA5,0xA5,0xA5,0xA5};
	uint8_t buf1[5];
	uint8_t i; 
  //SPIx_SetSpeed(SPI_SPEED_8);
	NRF_Write_Buf(NRF_WRITE_REG+TX_ADDR,buf,5);            //写入5个字节的地址.
	NRF_Read_Buf(TX_ADDR,buf1,5);                          //读出写入的地址 
	for(i=0;i<5;i++)
	{
	    if(buf1[i]!=0xA5)
		{
		    break;					   
		}
	}
	if(i!=5)
	{
		   PrintString("NRF24l01不在位");   
		//NRF24L01不在位
		return 1;
	}
		   PrintString("NRF24l01在位");                     //NRF24L01在位
	return 0;
}	 	 


//在指定位置写指定长度的数据
//*pBuf:数据指针 
//返回值,此次读到的状态寄存器值
uint8_t NRF_Write_Buf(uint8_t regaddr, uint8_t *pBuf, uint8_t datalen){

	uint8_t status,ctr;	
 	  NRF_CSN_L;                                         //使能SPI传输
                                                                
  	status = SPI1_RW(regaddr);                                  //发送寄存器值(位置),并读取状态值
  	for(ctr=0; ctr<datalen; ctr++)                              
          SPI1_RW(*pBuf++);                                     //写入数据	 
                                                                
  	NRF_CSN_H;                                         //关闭SPI传输
                                                                
  	return status;                                              //返回读到的状态值
}

//在指定位置读出指定长度的数据
//*pBuf:数据指针
//返回值,此次读到的状态寄存器值 
uint8_t NRF_Read_Buf(uint8_t regaddr,uint8_t *pBuf,uint8_t datalen){
	uint8_t status,ctr;
  	NRF_CSN_L;                                         //使能SPI传输                                                            
  	status=SPI1_RW(regaddr);                                    //发送寄存器值(位置),并读取状态值   	
	
 	for(ctr=0;ctr<datalen;ctr++)                                
          pBuf[ctr]=SPI1_RW(0);                              //读出数据
  	NRF_CSN_H;                                               //关闭SPI传输
                                                                
  	return status;                                              //返回读到的状态值
}

//启动NRF24L01发送一次数据
//txbuf:待发送数据首地址
//返回值:发送完成状况
uint8_t NRF24L01_TxPacket(uint8_t *txbuf)
{
    uint8_t state; 
//SPI1_SetSpeed();  
	 NRF_CE_L;
    
//    NRF_Write_Buf(NRF_WRITE_REG+RX_ADDR_P0,(uint8_t*)pair.addr,RX_ADR_WIDTH);
  	NRF_Write_Buf(WR_TX_PLOAD,txbuf,TX_PLOAD_WIDTH);
    
 NRF_CE_H;                                          //启动发送
    
	while(NRF_IRQ_RAED!=0);                              //等待发送完成
    
	state=NRF_Read_Reg(STATUS);                            //读取状态寄存器的值	   
	NRF_Write_Reg(NRF_WRITE_REG+STATUS,state);             //清除TX_DS或MAX_RT中断标志
    
	if(state&MAX_TX)                                            //达到最大重发次数
	{	
        NRF_Write_Reg(FLUSH_TX,0xff);                      //清除TX FIFO寄存器 	
		return MAX_TX; 
	}
	if(state&TX_OK)                                             //发送完成
	{
		return TX_OK;
	}
	return 0xff;                                                //其他原因发送失败
}

//启动NRF24L01发送一次数据
//txbuf:待发送数据首地址
//返回值:0，接收完成；其他，错误代码
uint8_t NRF24L01_RxPacket(uint8_t *rxbuf)
{
	uint8_t state;	
	state=NRF_Read_Reg(STATUS);                            //读取状态寄存器的值    	 
	NRF_Write_Reg(NRF_WRITE_REG+STATUS,state);             //清除TX_DS或MAX_RT中断标志
	if(state&RX_OK)                                             //接收到数据
	{
		NRF_Read_Buf(RD_RX_PLOAD,rxbuf,RX_PLOAD_WIDTH);        //读取数据
		NRF_Write_Reg(FLUSH_RX,0xff);                      //清除RX FIFO寄存器 
		return RX_OK; 
	}	   
	return ERROR;                                               //没收到任何数据
}

