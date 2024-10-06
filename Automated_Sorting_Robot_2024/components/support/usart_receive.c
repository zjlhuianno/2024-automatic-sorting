#include "usart_receive.h"
#include "configs.h"
#include "usart.h"

//以下都是openmv的相关变量。
//***********************************************************//
uint8_t rxBuffer_main,rx_buffer[4];
uint8_t mode = 2;
uint8_t state = 0;
uint16_t color = 0, shape = 0, color_circle = 0;

// HAL库回调函数。

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	
    if (huart->Instance == USART1)
    {
        // 处理接收到的数据
				if(mode == 1)
				{
//						// 将接收到的数据添加到缓冲区
//						rx_buffer[rx_index++] = rxBuffer_main;
//						// 检查是否接收到了足够的数据
//						if (rx_index == 4) {
//							// 解析数据
//							color = (rx_buffer[1] << 8) | rx_buffer[0];
//							shape = (rx_buffer[3] << 8) | rx_buffer[2];
//							// 重置索引，准备下一次接收
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
															// 数据包接收完毕，处理rx_buffer中的数据
															// 在这里添加处理数据的代码
															color_circle = rx_buffer[2];
															state = 0;
													} else {
															// 继续接收下一个字节
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

//        // 重新启动中断接收，准备接收下一批数据
//        HAL_UART_Receive_IT(&huart1, &rxBuffer_main, sizeof(rxBuffer_main));
    }
}


