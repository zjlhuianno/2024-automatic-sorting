#include "CAN_Setting.h"

CAN_TxHeaderTypeDef  classis_tx_message;
uint8_t              chassis_can_send_data[2];
motor_measure_t 		 motor_chassis[7];
float 							 actual_value;
int 								 count = 0;
uint8_t 						 rx_data[8];//用于存储接收到的数据。


//CAN过滤器初始化。
void CAN_filter_init(void)
{
	CAN_FilterTypeDef can_filter_st;
	can_filter_st.FilterActivation = ENABLE;//激活过滤器。
	can_filter_st.FilterMode = CAN_FILTERMODE_IDMASK;//设为掩码模式。
	can_filter_st.FilterScale = CAN_FILTERSCALE_32BIT;//设为32位宽。
	//上面选择32位掩码模式，则下面的CAN_FxR1用作32位宽的验证码，CAN_FxR2用作32位宽的屏蔽码。
	can_filter_st.FilterIdHigh = 0x0000;//对应CAN_FxR1的高16位。
	can_filter_st.FilterIdLow = 0x0000;//对应CAN_FxR1的低16位。
	can_filter_st.FilterMaskIdHigh = 0x0000;//对应CAN_FxR2的高16位。
	can_filter_st.FilterMaskIdLow = 0x0000;//对应CAN_FxR2的低16位。
	can_filter_st.FilterBank = 0;//CAN1的筛选器编号为0~14，这里选0即可。
	can_filter_st.FilterFIFOAssignment = CAN_RX_FIFO0;//把接收到的报文放入FIFO0中。
	HAL_CAN_ConfigFilter(&hcan1, &can_filter_st);
	
	HAL_CAN_Start(&hcan1);
	HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING);

}

//CAN向电机发送控制数据。
void CAN_cmd_classis(int16_t motor)
{
	uint32_t send_mail_box;//用于存储CAN发送所使用的邮箱号。
	
	//仲裁段。
	classis_tx_message.IDE = CAN_ID_STD;//发送者的ID格式为标准格式。
	classis_tx_message.StdId = 0x200;//发送者的ID。
	classis_tx_message.RTR = CAN_RTR_DATA;//传送帧类型为数据帧。
	
	//控制段。
	classis_tx_message.DLC = 0x02;//该数据帧的长度为2个字节。
	
	//数据段。
	chassis_can_send_data[0] = motor >> 8;
	chassis_can_send_data[1] = motor;
	
	HAL_CAN_AddTxMessage(&hcan1, &classis_tx_message, chassis_can_send_data, &send_mail_box);//CAN发送数据。
}

//CAN接收中断回调。
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
	CAN_RxHeaderTypeDef rx_header;//用于存储接收到的发送者的ID等。
	
	HAL_CAN_GetRxMessage(&hcan1, CAN_RX_FIFO0, &rx_header, rx_data);//CAN接收数据。
	
	switch (rx_header.StdId)//判断接收者是否为目标电调。
	{
		case 0x201:
		case 0x202:
		case 0x203:
		case 0x204:
		{
			static uint8_t i = 0;
			i = rx_header.StdId - 0x201;//找到每个电机所返回数据包的数组的下标。
			get_motor_measure(&motor_chassis[i], rx_data);//解析接收到的数据。
			count++;
			actual_value = motor_chassis[i].speed_rpm;//PID更新目前的电流值。
			break;
		}

		default:
		{
				break;
		}
	}
}
