#ifndef __ARM_POSTURE_CALCULATE_H__
#define __ARM_POSTURE_CALCULATE_H__

//��е�۸����ؽ�֮��ĳ��ȡ�//��λ��cm��
#define L0 20.0f		//��е�۸�����O�㣩	��	DM4340��A�㣩			�ľ��롣
#define L1 16.5f		//DM4340��A�㣩			��	DM4310��B�㣩			�ľ��롣
#define L2 18.0f		//DM4310��B�㣩			��	�ؽڶ����C�㣩		�ľ��롣
#define L3 16.5f		//�ؽڶ����C�㣩		��	��еצ�����D�㣩	�ľ��롣
#define L4 7.50f		//��еצ�����D�㣩	��	Ŀ��㣨P�㣩			�ľ��롣

#define PI 3.141592654

void arms_js(float* data_ref,float x1,float y1,float alphas,float l1,float l2,float l3);

//void Arm_Pos_Calc(float *Target_Point, float *Angle);

#endif

