#include "pair_freq.h"


_PAIR pair = {0,{0x1F,0x2E,0x3D,0x4C,0x5B},{4,14,24,34}};




//��ȡоƬID
void get_chip_id(void)
{
    chip_id[0] = *(__IO u32 *)(0X1FFFF7F0); // ���ֽ�
    chip_id[1] = *(__IO u32 *)(0X1FFFF7EC); //
    chip_id[2] = *(__IO u32 *)(0X1FFFF7E8); // ���ֽ�
    
    ID = (chip_id[0] | chip_id[1] | chip_id[2]); 
}


//��Ƶ
void wait_pairing(void)
{
    while(1)
    {    
        //���Ÿ������ƽ��ж�Ƶ
        if(tx.thr>900)
        {
            pair.flag = 1;
            
            //��ַװ���Ƶ����������
            pair.addr[0] = *((uint8_t *)&ID);		 
            pair.addr[1] = *(((uint8_t *)&ID)+1);	     
            pair.addr[2] = *(((uint8_t *)&ID)+2);		
            pair.addr[3] = *(((uint8_t *)&ID)+3);		
            pair.addr[4] = pair.addr[0];		
            
            //Ƶ��װ������
            pair.freq_channel[0] = pair.addr[0]%100;
            pair.freq_channel[1] = pair.addr[1]%100;
            pair.freq_channel[2] = pair.addr[2]%100;
            pair.freq_channel[3] = pair.addr[3]%100;            
        }
        else if(tx.thr<100)
        {
            if(pair.flag==1)
            {
                pair.flag = 2;
                
                Clr_NRF24L01_CE();	
                
                //д���µĵ�ַ�͵�ַ
                NRF24L01_Write_Buf(SPI_WRITE_REG+TX_ADDR,(uint8_t*)pair.addr,TX_ADR_WIDTH);    //дTX�ڵ��ַ 
                NRF24L01_Write_Buf(SPI_WRITE_REG+RX_ADDR_P0,(uint8_t*)pair.addr,RX_ADR_WIDTH); //����TX�ڵ��ַ,��ҪΪ��ʹ��ACK	 
//                NRF24L01_Write_Reg(SPI_WRITE_REG+RF_CH,pair.freq_channel[1]);                  //����RFͨ��Ϊ40
                
                Set_NRF24L01_CE();                                          //CEΪ��,10us����������
                
                break;
            }
        }            
    }
}