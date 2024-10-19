#include "configs.h"


/*
底盘
chassis_flag ==
0 设置底盘速度为零
1 斜行到十字前，然后前行，同时灰度识别，通过算法使其停在圆盘机前,左转90度停下,前走到圆盘机白线前
2 左走到灰度全白停下，然后斜走到倒球处
3 右移，转180度，前行，识别到十字停下，斜移到白线，慢慢右移到灰度漏缺，可能会来回检测
4 走到十字，然后走到立桩前
5 绕桩走大概270度，具体试，越小越快
6 走到十字，到倒球处倒球
7 走到十字，回家
*/
uint8_t chassis_flag = 0;

/*底盘停止
0 走
1 停
*/
uint8_t chassis_stop_flag = 0;

/*十字标志
0 
1 开始识别十字
*/
uint8_t cross_flag = 0;

/*
机械臂
arm_flag ==
0 收起状态
1 对准圆盘机
2 对准立桩中心白球
3 对准立桩边缘
4 对准台阶物品
5 对准收物处
*/
uint8_t arm_flag = 0;

/*
拨夹爪
servo_claw_flag == 
0 闭合，给值或失能，给值有点担心烧坏
1 开
2 夹
*/
uint8_t servo_claw_flag = 0;

/*
分类舵机
servo_sort_flag ==
0 加分球
1 黄球
*/
uint8_t servo_sort_flag = 0;

/*
圆环舵机
servo_circle_flag == 
0 收紧
1 打开
*/
uint8_t servo_circle_flag =0;

/*
放球舵机
servo_release_flag = 
0 收起
1 放下
*/
uint8_t servo_release_flag = 0;

/*陀螺仪初始
ins_init_flag = 
0 初始化中
1 初始化完成
*/
uint8_t ins_init_flag = 0;

/*里程计清零
odometryResetFlag = 
0 
1 清零
*/
uint8_t odometryResetFlag = 0;


/*避障标志位
0
1 进行避障动作
*/
uint8_t avoidance_flag = 0;
