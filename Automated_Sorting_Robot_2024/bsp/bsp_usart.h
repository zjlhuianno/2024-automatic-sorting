#ifndef BSP_USART_H
#define BSP_USART_H
#include "struct_typedef.h"
#include "usart.h"

#define BUFFER_SIZE  100 
extern  volatile uint8_t rx_len1 ;  //接收一帧数据的长度
extern volatile uint8_t recv_end_flag1; //一帧数据接收完成标志
extern uint8_t rx_buffer1[100];  //接收数据缓存数组



extern UART_HandleTypeDef huart1;
extern DMA_HandleTypeDef hdma_usart1_rx;
extern DMA_HandleTypeDef hdma_usart1_tx;

extern UART_HandleTypeDef huart2;
extern DMA_HandleTypeDef hdma_usart2_rx;
extern DMA_HandleTypeDef hdma_usart2_tx;

void usart1_init(uint8_t *rx1_buf, uint8_t *rx2_buf, uint16_t dma_buf_num);
#endif
