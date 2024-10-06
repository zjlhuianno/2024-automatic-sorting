#include "usart_receive.h"
#include "configs.h"
#include "usart.h"

//���¶���openmv����ر�����
//***********************************************************//
uint8_t rxBuffer_main,rx_buffer[4];
uint8_t mode = 2;
uint8_t state = 0;
uint16_t color = 0, shape = 0, color_circle = 0;

// HAL��ص�������

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	
    if (huart->Instance == USART1)
    {
        // ������յ�������
				if(mode == 1)
				{
//						// �����յ���������ӵ�������
//						rx_buffer[rx_index++] = rxBuffer_main;
//						// ����Ƿ���յ����㹻������
//						if (rx_index == 4) {
//							// ��������
//							color = (rx_buffer[1] << 8) | rx_buffer[0];
//							shape = (rx_buffer[3] << 8) | rx_buffer[2];
//							// ����������׼����һ�ν���
//							rx_index = 0;
//						}
				}
				else if(mode == 2)
				{ 		
							uint8_t com_data;
							switch(state)
							{
									case 0:
											if(rx_buffer[0] == 0x2C) {
													state = 1;
											}
											break;
									case 1:
											if(rx_buffer[1] == 0x12) {
													state = 2;
											}
											break;
									default:
											if(state < sizeof(rx_buffer)) {
													com_data = rx_buffer[state];
													if(com_data == 0x5B) {
															// ���ݰ�������ϣ�����rx_buffer�е�����
															// ��������Ӵ������ݵĴ���
															color_circle = rx_buffer[2];
															state = 0;
													} else {
															// ����������һ���ֽ�
															state++;
															HAL_UART_Receive_IT(&huart1, &rx_buffer[state], 1);
													}
											}
											break;
							}
				
				}
				else if(mode == 3)
				{
				
				}

//        // ���������жϽ��գ�׼��������һ������
//        HAL_UART_Receive_IT(&huart1, &rxBuffer_main, sizeof(rxBuffer_main));
    }
}


