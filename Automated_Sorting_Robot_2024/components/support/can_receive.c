#include "can_receive.h"

#define get_motor_measure(ptr, data)                                    \
    {                                                                   \
        (ptr)->last_ecd = (ptr)->ecd;                                   \
        (ptr)->ecd = (uint16_t)((data)[0] << 8 | (data)[1]);            \
        (ptr)->speed_rpm = (uint16_t)((data)[2] << 8 | (data)[3]);      \
        (ptr)->given_current = (uint16_t)((data)[4] << 8 | (data)[5]);  \
        (ptr)->temperate = (data)[6];                                   \
    }
extern CAN_HandleTypeDef hcan1;
static motor_measure_t motor_chassis[4];
static CAN_TxHeaderTypeDef chassis_tx_message;
static uint8_t chassis_can_send_data[8];

//发送电机控制电流(0x201,0x202,0x203,0x204)
void CAN_cmd_chassis(int16_t motor1, int16_t motor2, int16_t motor3, int16_t motor4)
{
    uint32_t send_mail_box;
    chassis_tx_message.StdId = CAN_CHASSIS_ALL_ID;
    chassis_tx_message.IDE = CAN_ID_STD;
    chassis_tx_message.RTR = CAN_RTR_DATA;
    chassis_tx_message.DLC = 0x08;			//一个电机0x02，四个电机0x08
    chassis_can_send_data[0] = motor1 >> 8;
    chassis_can_send_data[1] = motor1;
    chassis_can_send_data[2] = motor2 >> 8;
    chassis_can_send_data[3] = motor2;
    chassis_can_send_data[4] = motor3 >> 8;
    chassis_can_send_data[5] = motor3;
    chassis_can_send_data[6] = motor4 >> 8;
    chassis_can_send_data[7] = motor4;
	
    HAL_CAN_AddTxMessage(&hcan1, &chassis_tx_message, chassis_can_send_data, &send_mail_box);

}

//hal库CAN回调函数,接收电机数据
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
	CAN_RxHeaderTypeDef rx_header;
	uint8_t rx_data[8];
	uint8_t ii;
    HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &rx_header, rx_data);
	if(hcan == &hcan1)
	{
		//根据接收到的CAN消息的标准标识符 (rx_header.StdId) 与预定义的常量 (CAN_2006_M1_ID、CAN_2006_M2_ID等) 进行比较。
		switch (rx_header.StdId)
		{
			case CAN_2006_M1_ID:
			case CAN_2006_M2_ID:
			case CAN_2006_M3_ID:
			case CAN_2006_M4_ID:
			//如果 StdId 匹配其中一个常量，它会通过减去 CAN_2006_M1_ID 来确定消息属于哪个电机 (i)
			{
				static uint8_t i = 0;
				i = rx_header.StdId - CAN_2006_M1_ID;
				get_motor_measure(&motor_chassis[i], rx_data);
				//计算转子的旋转
				if(motor_chassis[i].ecd - motor_chassis[i].last_ecd < -4086)
					motor_chassis[i].round++;
				if(motor_chassis[i].ecd - motor_chassis[i].last_ecd >  4086)
					motor_chassis[i].round--;
				motor_chassis[i].code = motor_chassis[i].ecd - motor_chassis[i].last_ecd + motor_chassis[i].round * 8192;
				//里程计清零标志位
				if(odometryResetFlag == 1)
				{
					odometryResetFlag = 0;
					for(ii = 0; ii < 4; ii ++)
					{
						motor_chassis[ii].round =0;
						motor_chassis[ii].code = 0;
					}

				}
				break;
			}
			default:
			{
				break;
			}
		}
	}
		
        

}
//API
const motor_measure_t *get_chassis_motor_measure_point(uint8_t i)
{
    return &motor_chassis[(i & 0x03)];
}
