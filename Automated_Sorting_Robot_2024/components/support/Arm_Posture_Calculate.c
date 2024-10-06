#include "Arm_Posture_Calculate.h"
#include "stdlib.h"
#include "math.h"

//角度转弧度。
float dtor(float d)
{
    return (3.14159f*d)/180.0f;
}

//弧度转角度。
float rtod(float r)
{
    return (r*180.0f)/(3.14159f);
}

//计算在三角形中，以d1,d2为邻边，l1为对边的内角。
float tri_cos(float d1,float d2,float l1)
{
    if(d1+d2<=l1 || fabs(d1-d2)>=l1)
    {
        return 361;
    }
    else
    {
        return ((pow(d1,2)+pow(d2,2)-pow(l1,2))/(2*d1*d2));
    }
}

//两点之间的距离。
float re_dis(float x1,float y1,float x2,float y2)
{
    return pow(pow(x1-x2,2)+pow(y1-y2,2),0.5);
}

void arms_js(float* data_ref,float x1,float y1,float alphas,float l1,float l2,float l3)
{
	//单位cm。
	//l1：夹爪到机械臂舵机。
	//l2：机械臂舵机到DM4310。
	//l3：DM4310到DM4340。
	
	float temps1=0.0f,x2=0.0f,y2=0.0f,x3=0.0f,y3=0.0f,x4=0.0f,y4=0.0f,bd=0.0f,ad=0.0f,ab=0.0f,bc=0.0f,co=0.0f,ob=0.0f,ao=0.0f,alpha_s=0.0f,alpha1=0.0f,alpha2=0.0f,alpha3=0.0f,alpha4=0.0f,alpha5=0.0f,alpha6=0.0f,alpha7=0.0f,kbc=0.0f,kbc_f=0.0f,flag=0;
	float real_alpha1 = 0.0f, real_alpha2 = 0.0f, real_alpha3 = 0.0f;
	ab=l1;
	bc=l2;
	co=l3;
	alpha_s=dtor(alphas);
	x2=x1-l1*cos(alpha_s);
	y2=y1-l1*sin(alpha_s);
	ob= re_dis(x2,y2,0,0);
	temps1= tri_cos(ob,co,bc);
	if(temps1==361)
	{
		flag=1;
	}
	else
	{
		alpha4= acos(temps1);
	}
	ao= re_dis(x1,y1,0,0);
	temps1= tri_cos(ob,ao,ab);
	if(temps1==361)
	{                                                  
		flag=1;
	}
	else
	{                                                                                         
		alpha5= acos(temps1);
	}
	alpha6= atan(y1/x1);
	alpha3=alpha4+alpha5+alpha6;
	temps1= tri_cos(bc,co,ob);
	if(temps1==361)
	{
		flag=1;
	}
	else
	{
		alpha2= acos(temps1);
	}
	alpha7=dtor(270)-alpha4-alpha5-alpha6;
	x3=0-co*sin(alpha7);
	y3=0-co*cos(alpha7);
	kbc=(y2-y3)/(x2-x3);
	kbc_f=1/kbc;
	x4=x2-(y2/kbc_f);
	bd= re_dis(x2,y2,x4,y4);
	ad= re_dis(x1,y1,x4,y4);
	temps1= tri_cos(ab,bd,ad);


	if(temps1==361)
	{
		flag=1;
	}
	else
	{
		alpha1= dtor(180)-acos(temps1);
	}
	alpha1= rtod(alpha1);
	alpha2= rtod(alpha2);
	alpha3= rtod(alpha3);
	
	real_alpha1 = alpha1;//机械臂舵机转过的角度。
	real_alpha2 = alpha2-(180.0f-137.9f);//DM4310转过的角度。
	real_alpha3 = 55.0f+180.0f-alpha3;//DM4340转过的角度。
	
	//角度限制。
	if(real_alpha1>100)
	{
		real_alpha1=100;
	}
	else if(real_alpha1<0)
	{
		real_alpha1=0;
	}
	
	if(real_alpha2>260)
	{
		real_alpha2=260;
	}
	else if(real_alpha2<0)
	{
		real_alpha2=0;
	}
	
	if(real_alpha3>220)
	{
		real_alpha3=220;
	}
	else if(real_alpha3<0)
	{
		real_alpha3=0;
	}
	
	data_ref[0]=real_alpha1;
	data_ref[1]=real_alpha2;
	data_ref[2]=real_alpha3;
	data_ref[3]=flag;//当flag为1时，计算出问题。

}
