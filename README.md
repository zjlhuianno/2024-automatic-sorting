# 2024-automatic-sorting

2024自动分拣自动分拣机器总代码。


V0.0.0
初版融合，将底盘代码和机械臂代码融合到了一起，但底盘和机械臂不能同时动。

V0.0.1
解决了机械臂和底盘不能同时动的问题，不能同时动是因为task的优先级设置不当，chassis_task的优先级设置应该比arm_task的优先级高。

V0.0.2
融合了openmv通讯，但由于openmv发送有问题，所以串口通讯有待完善。

V0.0.3
优化了机械臂的斜坡函数参数（k由0.01变为0.02），同时修改了DM电机的位置环PID参数。
