#pragma once

#define     cmd_xbox_send       0x29e92210	//xbox发送指令
HANDLE			hMutex_busy();				//获取互斥信号量
SOCKADDR_IN    *GetSocketAddress();			//获取地址
SOCKET			GetSocketFd();				//socket通信句柄

typedef struct
{
	signed short x;
	signed short y;
}game_pad_t;

typedef struct
{
	char btnUp;		//上
	char btnRight;	//下
	char btnDown;	//左
	char btnLeft;	//右
	char btnStart;	//开始
	char btnBack;	//返回
	char btnL3;		//左摇杆
	char btnR3;		//右摇杆
	char btnA;		//A键
	char btnB;		//B键
	char btnX;		//X键
	char btnY;		//Y键
	char btnLB;		//LB键
	char btnRB;		//RB键
	char btnHome;	//home键
	game_pad_t left_joystick; //左摇杆
	game_pad_t right_joystick;//右摇杆
	unsigned char LT;//左油门
	unsigned char RT;//右油门
}soft_xbox_t;



typedef struct
{
	cmd_head_t head;
	union {
		cmd_data_t      u8buff;		  //buff
		cmd_u16_t       u16buff;	  //U16
		soft_xbox_t     soft_xbox;    // 
	};
}client_xbox;


/*************************xbox相关API*****************************/
int Xbox_Free();			//释放软件控制，恢复硬件映射
int Xbox_UP(int isdown);	//手柄上键状态设置。
int Xbox_Down(int isdown);	//手柄下键状态设置。
int Xbox_Left(int isdown);	//手柄左键状态设置。
int Xbox_Right(int isdown); //手柄右键状态设置。
int Xbox_UP_Right(int isdown);//手柄上右键状态设置
int Xbox_Right_Down(int isdown);//手柄右下键状态设置
int Xbox_Left_Down(int isdown);//手柄左下键状态设置
int Xbox_Left_Up(int isdown);//手柄左上键状态设置
int Xbox_Home(int isdown);	//手柄home键状态设置。
int Xbox_Back(int isdown);	//手柄Back键状态设置。
int Xbox_Start(int isdown); //手柄Start键状态设置。
int Xbox_L3(int isdown);	//手柄左摇杆按键状态设置。
int Xbox_R3(int isdown);	//手柄右摇杆按键状态设置。
int Xbox_A(int isdown);		//手柄A键状态设置。
int Xbox_B(int isdown);		//手柄B键状态设置。
int Xbox_X(int isdown);		//手柄X键状态设置。
int Xbox_Y(int isdown);		//手柄Y键状态设置。
int Xbox_LB(int isdown);	//手柄LB键状态设置。
int Xbox_RB(int isdown);	//手柄RB键状态设置。
int Xbox_Left_joystick(short x, short y);	//手柄左摇杆坐标设置。XY取值范围[-32767,32767]
int Xbox_Right_joystick(short x, short y);	//手柄右摇杆坐标设置。XY取值范围[-32767,32767]
int Xbox_LT(int value);		//手柄LT键状态设置。取值范围[0,255]
int Xbox_RT(int value);		//手柄RT键状态设置。取值范围[0,255]