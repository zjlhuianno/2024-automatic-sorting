#ifndef __ARM_POSTURE_CALCULATE_H__
#define __ARM_POSTURE_CALCULATE_H__

//机械臂各个关节之间的长度。//单位：cm。
#define L0 20.0f		//机械臂根部（O点）	到	DM4340（A点）			的距离。
#define L1 16.5f		//DM4340（A点）			到	DM4310（B点）			的距离。
#define L2 18.0f		//DM4310（B点）			到	关节舵机（C点）		的距离。
#define L3 16.5f		//关节舵机（C点）		到	机械爪舵机（D点）	的距离。
#define L4 7.50f		//机械爪舵机（D点）	到	目标点（P点）			的距离。

#define PI 3.141592654

void arms_js(float* data_ref,float x1,float y1,float alphas,float l1,float l2,float l3);

//void Arm_Pos_Calc(float *Target_Point, float *Angle);

#endif

