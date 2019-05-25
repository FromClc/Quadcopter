#include "nrf24l01.h"

static uint8_t TX_ADDRESS[5]={0x1F,0x2E,0x3D,0x4C,0x5B};//������ַ
static uint8_t RX_ADDRESS[5]={0x1F,0x2E,0x3D,0x4C,0x5B};//���յ�ַ

uint8_t NRF24L01_RXDATA[32];

uint8_t NRF24L01_TXDATA[32];

void NRF_Mode(uint8_t Chanal,uint8_t mode ){
	NRF_CE_L;
	//NRF_CSN_L;
	NRF_Write_Reg(FLUSH_TX,0xff);     //��շ��ͻ�����
	NRF_Write_Reg(FLUSH_RX,0xff);		//��ս��ջ�����
	NRF_Write_Buf(NRF_WRITE_REG+TX_ADDR,TX_ADDRESS,5);//дTX�ڵ��ַ
	NRF_Write_Buf(NRF_WRITE_REG+RX_ADDR_P0,RX_ADDRESS,5);//дRX�ڵ��ַ
	NRF_Write_Reg(NRF_WRITE_REG+EN_AA,0x01);   //ʹ��ͨ��0���Զ�Ӧ��
	NRF_Write_Reg(NRF_WRITE_REG+EN_RXADDR,0x01); //ʹ��ͨ��0���Զ�Ӧ��
	NRF_Write_Reg(NRF_WRITE_REG+SETUP_RETR,0x1a);   
	                   //�����Զ��ط����ʱ�䣺500΢�����Զ��ط�������10
	NRF_Write_Reg(NRF_WRITE_REG+RF_CH,Chanal); //����RFͨ��ΪChanal
	NRF_Write_Reg(NRF_WRITE_REG+RX_PW_P0,32); //����ͨ��0����Ч���ݿ��
	NRF_Write_Reg(NRF_WRITE_REG+RF_SETUP,0x0f);
						//����TX���������0db���棬2Mbps,ʹ�ܵ���������
	if(mode==TX)
		NRF_Write_Reg(NRF_WRITE_REG+CONFIG,0x0e);//����ģʽ
	else if(mode==RX)
		NRF_Write_Reg(NRF_WRITE_REG+CONFIG,0x0f);//����ģʽ
	NRF_CE_H;
}

//ͨ��SPIд�Ĵ���
uint8_t NRF_Write_Reg(uint8_t regaddr,uint8_t data)
{
    uint8_t status;
    NRF_CSN_L;                                         //ʹ��SPI����                                             
    status =SPI1_RW(regaddr);                         //���ͼĴ����� 
    SPI1_RW(data);                                    //д��Ĵ�����ֵ
	                                                            
    NRF_CSN_H;                                         //��ֹSPI����
                                                                
    return(status);       		                          //����״ֵ̬
}
//��ȡSPI�Ĵ���ֵ ��regaddr:Ҫ���ļĴ���
uint8_t NRF_Read_Reg(uint8_t regaddr)
{
	   uint8_t reg_val;
 	   NRF_CSN_L;                                         //ʹ��SPI����	
	                                                            
  	SPI1_RW(regaddr);                                   //���ͼĴ�����
  	reg_val=SPI1_RW(0);                             //��ȡ�Ĵ�������
                                                                
  	NRF_CSN_H;                                        //��ֹSPI����	
                                                                
  	return(reg_val);                                 //����״ֵ̬
}	

//�ϵ���NRF24L01�Ƿ���λ
//д5������Ȼ���ٶ��������бȽϣ���ͬʱ����ֵ:0����ʾ��λ;���򷵻�1����ʾ����λ	
uint8_t NRF24L01_Check(void)
{
	uint8_t buf[5]={0xA5,0xA5,0xA5,0xA5,0xA5};
	uint8_t buf1[5];
	uint8_t i; 
  //SPIx_SetSpeed(SPI_SPEED_8);
	NRF_Write_Buf(NRF_WRITE_REG+TX_ADDR,buf,5);            //д��5���ֽڵĵ�ַ.
	NRF_Read_Buf(TX_ADDR,buf1,5);                          //����д��ĵ�ַ 
	for(i=0;i<5;i++)
	{
	    if(buf1[i]!=0xA5)
		{
		    break;					   
		}
	}
	if(i!=5)
	{
		   PrintString("NRF24l01����λ");   
		//NRF24L01����λ
		return 1;
	}
		   PrintString("NRF24l01��λ");                     //NRF24L01��λ
	return 0;
}	 	 


//��ָ��λ��дָ�����ȵ�����
//*pBuf:����ָ�� 
//����ֵ,�˴ζ�����״̬�Ĵ���ֵ
uint8_t NRF_Write_Buf(uint8_t regaddr, uint8_t *pBuf, uint8_t datalen){

	uint8_t status,ctr;	
 	  NRF_CSN_L;                                         //ʹ��SPI����
                                                                
  	status = SPI1_RW(regaddr);                                  //���ͼĴ���ֵ(λ��),����ȡ״ֵ̬
  	for(ctr=0; ctr<datalen; ctr++)                              
          SPI1_RW(*pBuf++);                                     //д������	 
                                                                
  	NRF_CSN_H;                                         //�ر�SPI����
                                                                
  	return status;                                              //���ض�����״ֵ̬
}

//��ָ��λ�ö���ָ�����ȵ�����
//*pBuf:����ָ��
//����ֵ,�˴ζ�����״̬�Ĵ���ֵ 
uint8_t NRF_Read_Buf(uint8_t regaddr,uint8_t *pBuf,uint8_t datalen){
	uint8_t status,ctr;
  	NRF_CSN_L;                                         //ʹ��SPI����                                                            
  	status=SPI1_RW(regaddr);                                    //���ͼĴ���ֵ(λ��),����ȡ״ֵ̬   	
	
 	for(ctr=0;ctr<datalen;ctr++)                                
          pBuf[ctr]=SPI1_RW(0);                              //��������
  	NRF_CSN_H;                                               //�ر�SPI����
                                                                
  	return status;                                              //���ض�����״ֵ̬
}

//����NRF24L01����һ������
//txbuf:�����������׵�ַ
//����ֵ:�������״��
uint8_t NRF24L01_TxPacket(uint8_t *txbuf)
{
    uint8_t state; 
//SPI1_SetSpeed();  
	 NRF_CE_L;
    
//    NRF_Write_Buf(NRF_WRITE_REG+RX_ADDR_P0,(uint8_t*)pair.addr,RX_ADR_WIDTH);
  	NRF_Write_Buf(WR_TX_PLOAD,txbuf,TX_PLOAD_WIDTH);
    
 NRF_CE_H;                                          //��������
    
	while(NRF_IRQ_RAED!=0);                              //�ȴ��������
    
	state=NRF_Read_Reg(STATUS);                            //��ȡ״̬�Ĵ�����ֵ	   
	NRF_Write_Reg(NRF_WRITE_REG+STATUS,state);             //���TX_DS��MAX_RT�жϱ�־
    
	if(state&MAX_TX)                                            //�ﵽ����ط�����
	{	
        NRF_Write_Reg(FLUSH_TX,0xff);                      //���TX FIFO�Ĵ��� 	
		return MAX_TX; 
	}
	if(state&TX_OK)                                             //�������
	{
		return TX_OK;
	}
	return 0xff;                                                //����ԭ����ʧ��
}

//����NRF24L01����һ������
//txbuf:�����������׵�ַ
//����ֵ:0��������ɣ��������������
uint8_t NRF24L01_RxPacket(uint8_t *rxbuf)
{
	uint8_t state;	
	state=NRF_Read_Reg(STATUS);                            //��ȡ״̬�Ĵ�����ֵ    	 
	NRF_Write_Reg(NRF_WRITE_REG+STATUS,state);             //���TX_DS��MAX_RT�жϱ�־
	if(state&RX_OK)                                             //���յ�����
	{
		NRF_Read_Buf(RD_RX_PLOAD,rxbuf,RX_PLOAD_WIDTH);        //��ȡ����
		NRF_Write_Reg(FLUSH_RX,0xff);                      //���RX FIFO�Ĵ��� 
		return RX_OK; 
	}	   
	return ERROR;                                               //û�յ��κ�����
}

