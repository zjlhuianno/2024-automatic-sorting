#include "Lidar_task.h"

LidarPointTypedef Pack_Data[12];/* �״���յ����ݴ������������֮��,����ʮ������ */
LidarPointTypedef Pack_sum;		/* ���������� */

uint8_t USART6_Receive_buf[1];          //����6�����ж����ݴ�ŵĻ�����
uint16_t receive_cnt = 0;
uint8_t confidence;
uint16_t lidar_distance, noise, reftof;
uint32_t peak, intg;

void Lidar_task(void const * argument)
{
	while(!ins_init_flag)
			osDelay(1);
	HAL_UART_Receive_IT(&huart6,USART6_Receive_buf,sizeof(USART6_Receive_buf)); //�򿪴���6�����ж�
	
	while(1)
	{

		if(chassis_flag == 8 && lidar_distance < 350)
		{
			chassis_flag = 9;
		}
		
		osDelay(1);
	}
	
}

void data_process(void)/*���ݴ����������һ֮֡��ɽ������ݴ���*/
{
	/* ������� */
	uint8_t i;
	static uint16_t count = 0;
	
	for(i=0;i<12;i++)/* 12����ȡƽ�� */
	{
		if(Pack_Data[i].distance != 0)  /* ȥ��0�ĵ� */
		{
			count++;
			Pack_sum.distance += Pack_Data[i].distance;
			Pack_sum.noise += Pack_Data[i].noise;
			Pack_sum.peak += Pack_Data[i].peak;
			Pack_sum.confidence += Pack_Data[i].confidence;
			Pack_sum.intg += Pack_Data[i].intg;
			Pack_sum.reftof += Pack_Data[i].reftof;
		}
	}
	if(count !=0)
	{
		lidar_distance = Pack_sum.distance/count;
		noise = Pack_sum.noise/count;
		peak = Pack_sum.peak/count;
		confidence = Pack_sum.confidence/count;
		intg = Pack_sum.intg/count;
		reftof = Pack_sum.reftof/count;
		Pack_sum.distance = 0;
		Pack_sum.noise = 0;
		Pack_sum.peak = 0;
		Pack_sum.confidence = 0;
		Pack_sum.intg = 0;
		Pack_sum.reftof = 0;
		count = 0;
	}
}

//void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) //���ڽ��ջص�����
//{
//	uint8_t temp_data;
//	
//	static uint8_t state = 0;			//״̬λ	
//	static uint8_t crc = 0;				//У���
//	static uint8_t cnt = 0;				//����һ֡12����ļ���
//	static uint8_t PACK_FLAG = 0;  		//�����־λ
//	static uint8_t data_len  = 0;  		//���ݳ���
//	static uint32_t timestamp = 0; 		//ʱ���
//	static uint8_t state_flag = 1; 		//ת�����ݽ��ձ�־λ
//	
//	if(huart -> Instance == USART6)
//	{
//		temp_data = USART6_Receive_buf[0];	
//		if(state < 4)/* ��ʼ����֤ ǰ4�����ݾ�Ϊ0xAA */
//		{                                          
//			if(temp_data == HEADER) state++;
//			else state = 0;
//		}
//		else if(state < 10 && state > 3)
//		{
//			switch(state)
//			{
//				case 4:   
//					if(temp_data == device_address)/* �豸��ַ��֤ */
//					{							
//						state++;
//						crc = crc + temp_data;									
//						break;        
//					} 
//					else state = 0,crc = 0;
//					
//				case 5:   
//					if(temp_data == PACK_GET_DISTANCE)/* ��ȡ������������ */
//					{  
//						PACK_FLAG = PACK_GET_DISTANCE;
//						state++;	
//						crc = crc + temp_data;	
//						break;									
//					}		 
//					else if(temp_data == PACK_RESET_SYSTEM)/* ��λ���� */
//					{
//						PACK_FLAG = PACK_RESET_SYSTEM;
//						state++; 
//						crc = crc + temp_data;	
//						break;	
//					}
//					else if(temp_data == PACK_STOP)/* ֹͣ�������ݴ������� */
//					{ 
//						PACK_FLAG = PACK_STOP;
//						state++; 
//						crc = crc + temp_data;	
//						break;
//					}
//					else if(temp_data == PACK_ACK)/* Ӧ�������� */
//					{  
//						PACK_FLAG = PACK_ACK;
//						state++;
//						crc = crc + temp_data;	
//						break;
//					}			 				 
//					else if(temp_data == PACK_VERSION)/* ��ȡ��������Ϣ���� */
//					{ 
//						PACK_FLAG = PACK_VERSION,
//						state++,
//						crc = crc + temp_data;	   	     
//						break;
//					}
//					else state = 0, crc = 0;
//					
//				case 6: 
//					if(temp_data == chunk_offset)/* ƫ�Ƶ�ַ */
//					{  
//						state++;
//						crc = crc + temp_data;
//						break; 	  
//					}	
//					else state = 0, crc = 0;
//					
//				case 7: 
//					if(temp_data == chunk_offset)
//					{  
//						state++;
//						crc = crc + temp_data;
//						break;
//					}
//					else state = 0, crc = 0;
//					
//				case 8: 
//					data_len = (uint16_t)temp_data;/* ���ݳ��ȵͰ�λ */
//					state++; 
//					crc = crc + temp_data;
//					break;
//				
//				case 9: 
//					data_len = data_len + ((uint16_t)temp_data<<8);/* ���ݳ��ȸ߰�λ */
//					state++;
//					crc = crc + temp_data;
//					break; 
//				default: break;
//			}
//		}
//		else if(state == 10) state_flag = 0;/*��switch������ʱstateΪ10����temp_data��Ϊ���볤�ȸ߰�λ���ݣ�������һ���ж�*/
//		if(PACK_FLAG == PACK_GET_DISTANCE && state_flag == 0)/* ��ȡһ֡���ݲ�У�� */
//		{
//			if(state>9)
//			{
//				if(state<190)
//				{
//					static uint8_t state_num;
//					state_num = (state - 10) % 15;
//					switch(state_num)
//					{
//						case 0: 
//							Pack_Data[cnt].distance = (uint16_t)temp_data;/* �������ݵͰ�λ */
//							crc = crc + temp_data;
//							state++;
//							break;        
//						case 1: 
//							Pack_Data[cnt].distance = ((uint16_t)temp_data<<8) + Pack_Data[cnt].distance;/* �������� */
//							crc = crc + temp_data;
//							state++;
//							break; 
//						case 2:
//							Pack_Data[cnt].noise = (uint16_t)temp_data;/* ���������Ͱ�λ */
//							crc = crc + temp_data;
//							state++;
//							break; 
//						case 3:
//							Pack_Data[cnt].noise = ((uint16_t)temp_data<<8) + Pack_Data[cnt].noise;				 /* �������� */
//							crc = crc + temp_data;
//							state++;
//							break; 
//						case 4:
//							Pack_Data[cnt].peak = (uint32_t)temp_data;				 										 /* ����ǿ����Ϣ�Ͱ�λ */
//							crc = crc + temp_data;
//							state++;
//							break; 
//						case 5:
//							Pack_Data[cnt].peak = ((uint32_t)temp_data<<8) + Pack_Data[cnt].peak;
//							crc = crc + temp_data;
//							state++;
//							break; 
//						case 6:
//							Pack_Data[cnt].peak = ((uint32_t)temp_data<<16) + Pack_Data[cnt].peak;	
//							crc = crc + temp_data;
//							state++;
//							break; 
//						case 7:
//							Pack_Data[cnt].peak = ((uint32_t)temp_data<<24) + Pack_Data[cnt].peak;				    /* ����ǿ����Ϣ */
//							crc = crc + temp_data;
//							state++;
//							break; 
//						case 8:
//							Pack_Data[cnt].confidence = temp_data;/* ���Ŷ� */
//							crc = crc + temp_data;
//							state++;
//							break; 
//						case 9:
//							Pack_Data[cnt].intg = (uint32_t)temp_data;															/* ���ִ����Ͱ�λ */
//							crc = crc + temp_data;
//							state++;
//							break; 
//						case 10:
//							Pack_Data[cnt].intg = ((uint32_t)temp_data<<8) + Pack_Data[cnt].intg;
//							crc = crc + temp_data;
//							state++;
//							break; 
//						case 11:
//							Pack_Data[cnt].intg = ((uint32_t)temp_data<<16) + Pack_Data[cnt].intg;
//							crc = crc + temp_data;
//							state++;
//							break; 
//						case 12:
//							Pack_Data[cnt].intg = ((uint32_t)temp_data<<24) + Pack_Data[cnt].intg;				  	 /* ���ִ��� */
//							crc = crc + temp_data;
//							state++;
//							break; 
//						case 13:
//							Pack_Data[cnt].reftof = (int16_t)temp_data;				 								 /* �¶ȱ���ֵ�Ͱ�λ */
//							crc = crc + temp_data;
//							state++;
//							break; 
//						case 14:
//							Pack_Data[cnt].reftof = ((int16_t)temp_data<<8) +Pack_Data[cnt].reftof;/* �¶ȱ���ֵ */
//							crc = crc + temp_data;
//							state++;
//							cnt++;/* ������һ�������� */
//							break; 
//						default: break;
//					}
//				}
//				/* ʱ��� */
//				if(state == 190) timestamp = temp_data, state++, crc = crc + temp_data;
//				else if(state == 191) timestamp = ((uint32_t)temp_data<<8) + timestamp, state++, crc = crc + temp_data; 
//				else if(state == 192) timestamp = ((uint32_t)temp_data<<16) + timestamp, state++, crc = crc + temp_data;
//				else if(state == 193) timestamp = ((uint32_t)temp_data<<24) + timestamp, state++, crc = crc + temp_data; 
//				else if(state==194)
//				{
//					if(temp_data == crc)/* У��ɹ� */
//					{
//						data_process();  	 /* ���ݴ����������һ֮֡��ɽ������ݴ��� */
//						receive_cnt++;	 	 /* ������յ���ȷ���ݵĴ��� */
//					}
//					lidar_distance = Pack_Data[0].distance;
//					crc = 0;
//					state = 0;
//					state_flag = 1;
//					cnt = 0;/* ��λ*/
//				}
//					
//			}
//		}
//		HAL_UART_Receive_IT(&huart6,USART6_Receive_buf,sizeof(USART6_Receive_buf));//���ڻص�����ִ�����֮����Ҫ�ٴο��������жϵȴ���һ�ν����жϵķ���
//	}
//}
