#include "Includes.h"

//�Ƕ�ת���ȡ�
float dtor(float d)
{
    return (3.14159f*d)/180.0f;
}

//����ת�Ƕȡ�
float rtod(float r)
{
    return (r*180.0f)/(3.14159f);
}

//�������������У���d1,d2Ϊ�ڱߣ�l1Ϊ�Աߵ��ڽǡ�
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

//����֮��ľ��롣
float re_dis(float x1,float y1,float x2,float y2)
{
    return pow(pow(x1-x2,2)+pow(y1-y2,2),0.5);
}

void arms_js(float* data_ref,float x1,float y1,float alphas,float l1,float l2,float l3)//��λcm��
{
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
	
	real_alpha1 = alpha1;//��е�۶��ת���ĽǶȡ�
	real_alpha2 = 33.0f+90+(180-alpha2);//DM4310ת���ĽǶȡ�
	real_alpha3 = 55.0f+180-alpha3;//DM4340ת���ĽǶȡ�
	
	//�Ƕ����ơ�
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
	data_ref[3]=flag;//��flagΪ1ʱ����������⡣
	
	

}

////��������ƽ����
//float Distance_Square(float *P1, float *P2)
//{
//	return ( (P1[0] - P2[0])*(P1[0] - P2[0]) + (P1[1] - P2[1])*(P1[1] - P2[1]) );
//}

////��е����̬���㡣
//void Arm_Pos_Calc(float *Target_Point, float *Angle)
//{
//	//Target_Point��y,z���ꡣ
//	float Y_P, Z_P;
//	Y_P = Target_Point[0];
//	Z_P = Target_Point[1];
//	
//	//�����ó�����������洢��Angle[3]�С�
//	float Alpha = 0.0f;	//�� = ��OAB ����DM4340  ���ơ�
//	float Beta  = 0.0f;	//�� = ��ABC ����DM4310  ���ơ�
//	float Gamma = 0.0f;	//�� = ��B'CD���ɹؽڶ�����ơ���B'Ϊ����BC�ϵ�һ�㡣��
//	
//	//����Beta��
//	float Cos_Beta;//Beta������ֵ��
//	float A[2], C[2];//A,C��������ꡣ
//	
//	A[0] = 0.0f;
//	A[1] = L0;
//	C[0] = Y_P - L3;
//	C[1] = Z_P + L4;
//	
//	Cos_Beta = (L1 * L1 + L2 * L2 - Distance_Square(A, C)) / (2 * L1 * L2);
//	
//	Beta = acos(Cos_Beta);
//	
//	//����Alpha��
//	float Cos_Alpha;//Alpha������ֵ��
//	float a, b, c, m, n;//�洢��������ʱ������
//	
//	a = L2 / L1;
//	b = (Y_P - L3) / L1;
//	c = (L0 - L4 - Z_P) / L1;
//	m = cos(Beta);
//	n = sin(Beta);
//	
//	Cos_Alpha = (c - a * c * m + a * b * n) / (a * a * (m * m - n * n) - 2 * a * m + 1);
//	
//	Alpha = acos(Cos_Alpha);
//	
//	//����Gamma��
//	Gamma = 3 * PI / 2 - Alpha - Beta;
//	
//	//�������Ƕ����δ�Ž�Angle�����ڡ�
//	Angle[0] = Alpha;
//	Angle[1] = Beta;
//	Angle[3] = Gamma;
//}
