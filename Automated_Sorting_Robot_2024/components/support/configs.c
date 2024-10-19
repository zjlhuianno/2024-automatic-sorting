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

/*�����ǳ�ʼ
ins_init_flag = 
0 ��ʼ����
1 ��ʼ�����
*/
uint8_t ins_init_flag = 0;

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

uint8_t openmv_flag_ddd = 0;

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

int arm_flag_ok=0;

//���¶���openmv����ر�����
//***********************************************************//


//���¶��ǻ�е�۵���ر�����
//***********************************************************//


//**************PID����******************//

	//�ٶȻ�PI���ơ�
	float DM4340_speed_kp=0.21300000012f;
	float DM4340_speed_ki=0.000106123481f;
	float DM4340_speed_kd=0.0f;
	float DM4340_speed_max_out=10.0f;
	float DM4340_speed_max_iout=12.0f;
	float DM4340_speed_PID[3];

	//λ�û�PD���ơ�
	float DM4340_pos_kp=0.30f;//0.06f;
	float DM4340_pos_ki=0.0f;
	float DM4340_pos_kd=0.20f;
	float DM4340_pos_max_out=28.0f;
	float DM4340_pos_max_iout=28.0f;
	float DM4340_pos_PID[3];


	//********************************//
	
	//�ٶȻ�PI���ơ�
	float DM4310_speed_kp=0.0500000007f;
	float DM4310_speed_ki=0.00800000038f;
	float DM4310_speed_kd=0.0f;
	float DM4310_speed_max_out=10.0f;
	float DM4310_speed_max_iout=10.0f;
	float DM4310_speed_PID[3];

	//λ�û�PD���ơ�
	float DM4310_pos_kp=0.40f;
	float DM4310_pos_ki=0.0f;
	float DM4310_pos_kd=13.0f;
	float DM4310_pos_max_out=35.0f;
	float DM4310_pos_max_iout=35.0f;
	float DM4310_pos_PID[3];
//**************END******************//


//�ṹ�������
DM_Motor_t DM4340_Data,DM4310_Data;//DM4340��DM4310��������ݡ�
pid_type_def DM4340_speed_pid, DM4310_speed_pid;//DM4340��DM4310������ٶȻ�PID���ݡ�
pid_type_def DM4340_pos_pid, DM4310_pos_pid;//DM4340��DM4310�����λ�û�PID���ݡ�


float DM4340_LPF_K = 0.25f;		//DM4310�ĵ�ͨ�˲�LPF(Lowpass filtering)������
float DM4310_LPF_K = 0.15f;		//DM4310�ĵ�ͨ�˲�LPF(Lowpass filtering)������

float DM4340_target_speed;//DM4340��Ŀ���ٶȡ�
float DM4340_target_pos;  //DM4340��Ŀ��λ�á�
float DM4310_target_speed;//DM4310��Ŀ���ٶȡ�
float DM4310_target_pos;  //DM4310��Ŀ��λ�á�


float arms_js_data[4] = { 0.0f, 0.0f, 0.0f, 0.0f };//��̬�����õ������ݡ�
										//arms_js_data[0]����е�۶��ת���ĽǶȡ�
										//arms_js_data[1]��DM4310ת���ĽǶȡ�
										//arms_js_data[2]��DM4340ת���ĽǶȡ�
										//arms_js_data[3]�������Ƿ���ȷ�ı�־λ��1��ʾ�������

float Servo_target_angle;		//��е�۶����Ŀ��λ�á�
int DM4310_target_pos_int;  //DM4310��Ŀ��λ�á�(int�͡�)
int DM4340_target_pos_int;  //DM4340��Ŀ��λ�á�(int�͡�)

float target_x_follow; 	//Ŀ��x�����׷��ֵ��
float target_y_follow;		//Ŀ��y�����׷��ֵ��
float target_angle_follow;//Ŀ��Ƕȵ�׷��ֵ��

float target_x;			//��е�۽���ʱ�����Ŀ��x���ꡣ
float target_y;			//��е�۽���ʱ�����Ŀ��y���ꡣ
float target_angle;	//��е�۽���ʱ�����Ŀ��Ƕȡ�

int DM4310_delta_enc = 0;//��task����һֱ��¼DM4310��������p_int��last_p_int�Ĳ�ֵ��
int last_DM4310_delta_enc = 0;//��һ�ε�DM4310_delta_enc��

//��־λ����1Ϊ�ǣ�0Ϊ�񡣣�

uint8_t flag_z=0;								//�������Ƿ��������
uint8_t force_sensing_flag = 0;	//�Ƿ�������ظ�֪��

//ֱ�ӿ��ƶ���ı�־λ��
uint8_t arm_catch_flag=0;//��צ�����̬��־λ��
uint8_t pile_approach_flag = 0;//����Ȧ����׮��־λ��
uint8_t shunt_ball_flag = 0;//���������־λ��

//��е����openmv�йصı�־λ��
uint8_t openmv_ok_flag = 0;//openmv�Ƿ�ʶ�������ݡ�
uint8_t should_push_ball_flag_openmv = 1;//�Ƿ�Ҫ����
uint8_t ball_color_flag_openmv = 0;//ʶ�𵽵������ɫ����0Ϊ������1Ϊ�����򡣣�
uint8_t no_ball_push_flag_openmv = 0;//�Ƿ�ʶ������ɲ���



