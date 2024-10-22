#include "main.h"
#include "cmsis_os.h"
#include "configs.h"
#include "usart.h"

#define HEADER 0xAA				/* ��ʼ�� */
#define device_address 0x00     /* �豸��ַ */
#define chunk_offset 0x00       /* ƫ�Ƶ�ַ���� */
#define PACK_GET_DISTANCE 0x02 	/* ��ȡ������������ */
#define PACK_RESET_SYSTEM 0x0D 	/* ��λ���� */
#define PACK_STOP 0x0F 			/* ֹͣ�������ݴ������� */
#define PACK_ACK 0x10           /* Ӧ�������� */
#define PACK_VERSION 0x14       /* ��ȡ��������Ϣ���� */

typedef struct{
	int16_t distance;  			/* �������ݣ�����Ŀ����뵥λ mm */
	uint16_t noise;		 		/* ������������ǰ���������µ��ⲿ����������Խ��˵������Խ�� */
	uint32_t peak;				/* ����ǿ����Ϣ������Ŀ�귴��صĹ�ǿ�� */
	uint8_t confidence;			/* ���Ŷȣ��ɻ��������ͽ���ǿ����Ϣ�ںϺ�Ĳ�����Ŀ��Ŷ� */
	uint32_t intg;     			/* ���ִ�������ǰ�����������Ļ��ִ��� */
	int16_t reftof;   			/* �¶ȱ���ֵ������оƬ�ڲ��¶ȱ仯����ֵ��ֻ��һ���¶ȱ仯���޷�����ʵ�¶ȶ�Ӧ */
}LidarPointTypedef;


void Lidar_task(void const * argument);
