#include "bsp_can.h"
#include "main.h"


CAN_TxHeaderTypeDef  can_tx_message;
uint8_t  can_tx_data[8];


void can_filter_init(void)
{
	CAN_FilterTypeDef can_filter_st;
	can_filter_st.FilterActivation = ENABLE;
	can_filter_st.FilterMode = CAN_FILTERMODE_IDMASK;
	can_filter_st.FilterScale = CAN_FILTERSCALE_32BIT;
	can_filter_st.FilterIdHigh = 0x0000;
	can_filter_st.FilterIdLow = 0x0000;
	can_filter_st.FilterMaskIdHigh = 0x0000;
	can_filter_st.FilterMaskIdLow = 0x0000;
	can_filter_st.FilterBank = 0;
	can_filter_st.FilterFIFOAssignment = CAN_RX_FIFO0;
	HAL_CAN_ConfigFilter(&hcan2, &can_filter_st);
	HAL_CAN_Start(&hcan2);
	HAL_CAN_ActivateNotification(&hcan2, CAN_IT_RX_FIFO0_MSG_PENDING);

}

void CAN_motor_Send(int16_t data1)
{
	uint32_t send_mail_box;
	can_tx_message.StdId =0x200;
	can_tx_message.IDE = CAN_ID_STD;
	can_tx_message.RTR = CAN_RTR_DATA;
	can_tx_message.DLC = 0x08;
	can_tx_data[0] = data1;
	can_tx_data[1] = data1>>8;
	can_tx_data[2] = 0;
	can_tx_data[3] = 0;
	can_tx_data[4] = 0;
	can_tx_data[5] = 0;
	can_tx_data[6] = 0;
	can_tx_data[7] = 0;

	HAL_CAN_AddTxMessage(&hcan2, &can_tx_message, can_tx_data, &send_mail_box);
}

