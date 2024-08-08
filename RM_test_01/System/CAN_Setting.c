#include "CAN_Setting.h"

CAN_TxHeaderTypeDef  classis_tx_message;
uint8_t              chassis_can_send_data[2];
motor_measure_t 		 motor_chassis[7];
float 							 actual_value;
int 								 count = 0;
uint8_t 						 rx_data[8];//���ڴ洢���յ������ݡ�


//CAN��������ʼ����
void CAN_filter_init(void)
{
	CAN_FilterTypeDef can_filter_st;
	can_filter_st.FilterActivation = ENABLE;//�����������
	can_filter_st.FilterMode = CAN_FILTERMODE_IDMASK;//��Ϊ����ģʽ��
	can_filter_st.FilterScale = CAN_FILTERSCALE_32BIT;//��Ϊ32λ��
	//����ѡ��32λ����ģʽ���������CAN_FxR1����32λ�����֤�룬CAN_FxR2����32λ��������롣
	can_filter_st.FilterIdHigh = 0x0000;//��ӦCAN_FxR1�ĸ�16λ��
	can_filter_st.FilterIdLow = 0x0000;//��ӦCAN_FxR1�ĵ�16λ��
	can_filter_st.FilterMaskIdHigh = 0x0000;//��ӦCAN_FxR2�ĸ�16λ��
	can_filter_st.FilterMaskIdLow = 0x0000;//��ӦCAN_FxR2�ĵ�16λ��
	can_filter_st.FilterBank = 0;//CAN1��ɸѡ�����Ϊ0~14������ѡ0���ɡ�
	can_filter_st.FilterFIFOAssignment = CAN_RX_FIFO0;//�ѽ��յ��ı��ķ���FIFO0�С�
	HAL_CAN_ConfigFilter(&hcan1, &can_filter_st);
	
	HAL_CAN_Start(&hcan1);
	HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING);

}

//CAN�������Ϳ������ݡ�
void CAN_cmd_classis(int16_t motor)
{
	uint32_t send_mail_box;//���ڴ洢CAN������ʹ�õ�����š�
	
	//�ٲöΡ�
	classis_tx_message.IDE = CAN_ID_STD;//�����ߵ�ID��ʽΪ��׼��ʽ��
	classis_tx_message.StdId = 0x200;//�����ߵ�ID��
	classis_tx_message.RTR = CAN_RTR_DATA;//����֡����Ϊ����֡��
	
	//���ƶΡ�
	classis_tx_message.DLC = 0x02;//������֡�ĳ���Ϊ2���ֽڡ�
	
	//���ݶΡ�
	chassis_can_send_data[0] = motor >> 8;
	chassis_can_send_data[1] = motor;
	
	HAL_CAN_AddTxMessage(&hcan1, &classis_tx_message, chassis_can_send_data, &send_mail_box);//CAN�������ݡ�
}

//CAN�����жϻص���
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
	CAN_RxHeaderTypeDef rx_header;//���ڴ洢���յ��ķ����ߵ�ID�ȡ�
	
	HAL_CAN_GetRxMessage(&hcan1, CAN_RX_FIFO0, &rx_header, rx_data);//CAN�������ݡ�
	
	switch (rx_header.StdId)//�жϽ������Ƿ�ΪĿ������
	{
		case 0x201:
		case 0x202:
		case 0x203:
		case 0x204:
		{
			static uint8_t i = 0;
			i = rx_header.StdId - 0x201;//�ҵ�ÿ��������������ݰ���������±ꡣ
			get_motor_measure(&motor_chassis[i], rx_data);//�������յ������ݡ�
			count++;
			actual_value = motor_chassis[i].speed_rpm;//PID����Ŀǰ�ĵ���ֵ��
			break;
		}

		default:
		{
				break;
		}
	}
}
