#include "configs.h"


/*
����
chassis_flag ==
0 ���õ����ٶ�Ϊ��
1 б�е�ʮ��ǰ��Ȼ��ǰ�У�ͬʱ�Ҷ�ʶ��ͨ���㷨ʹ��ͣ��Բ�̻�ǰ,��ת90��ͣ��,ǰ�ߵ�Բ�̻�����ǰ
2 ���ߵ��Ҷ�ȫ��ͣ�£�Ȼ��б�ߵ�����
3 ���ƣ�ת180�ȣ�ǰ�У�ʶ��ʮ��ͣ�£�б�Ƶ����ߣ��������Ƶ��Ҷ�©ȱ�����ܻ����ؼ��
4 �ߵ�ʮ�֣�Ȼ���ߵ���׮ǰ
5 ��׮�ߴ��270�ȣ������ԣ�ԽСԽ��
6 �ߵ�ʮ�֣������򴦵���
7 �ߵ�ʮ�֣��ؼ�
*/
uint8_t chassis_flag = 0;

/*����ֹͣ
0 ��
1 ͣ
*/
uint8_t chassis_stop_flag = 0;

/*ʮ�ֱ�־
0 
1 ��ʼʶ��ʮ��
*/
uint8_t cross_flag = 0;

/*
��е��
arm_flag ==
0 ����״̬
1 ��׼Բ�̻�
2 ��׼��׮���İ���
3 ��׼��׮��Ե
4 ��׼̨����Ʒ
5 ��׼���ﴦ
*/
uint8_t arm_flag = 0;

/*
����צ
servo_claw_flag == 
0 �պϣ���ֵ��ʧ�ܣ���ֵ�е㵣���ջ�
1 ��
2 ��
*/
uint8_t servo_claw_flag = 0;

/*
������
servo_sort_flag ==
0 �ӷ���
1 ����
*/
uint8_t servo_sort_flag = 0;

/*
Բ�����
servo_circle_flag == 
0 �ս�
1 ��
*/
uint8_t servo_circle_flag =0;

/*
������
servo_release_flag = 
0 ����
1 ����
*/
uint8_t servo_release_flag = 0;

/*�����ǳ�ʼ
ins_init_flag = 
0 ��ʼ����
1 ��ʼ�����
*/
uint8_t ins_init_flag = 0;

/*��̼�����
odometryResetFlag = 
0 
1 ����
*/
uint8_t odometryResetFlag = 0;


/*���ϱ�־λ
0
1 ���б��϶���
*/
uint8_t avoidance_flag = 0;
