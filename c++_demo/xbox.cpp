#include "../c++_demo/NetConfig/kmboxNet.h"
#include "xbox.h"

soft_xbox_t xbox360;					//软件手柄数据数据
client_xbox tx;							//发送的内容
client_xbox rx;							//接收的内容
extern SOCKADDR_IN addrSrv;

unsigned int xbox_mac = 0;				//MAC参数需要正确。否则盒子不会理会。

/*盒子应答处理*/
int XboxAskRxHandle(client_xbox* rx, client_xbox* tx)		 //接收的内容
{	int ret = 0;
	if (rx->head.cmd != tx->head.cmd)
		ret = err_net_cmd;//命令码错误
	if (rx->head.indexpts != tx->head.indexpts)
		ret = err_net_pts;//时间戳错误
	ReleaseMutex(hMutex_busy());
	return ret;				//没有错误返回0
}
/*
复位所有按键为松开状态。摇杆居中。
*/
int Xbox_Free()
{	int err;
	if (GetSocketFd() <= 0)		return err_creat_socket;
	WaitForSingleObject(hMutex_busy(), INFINITE);
	tx.head.indexpts++;				//指令统计值
	tx.head.mac = xbox_mac;
	tx.head.cmd = cmd_xbox_send;	//指令
	tx.head.rand = rand();			//随机混淆值
	int length = sizeof(cmd_head_t) + sizeof(soft_xbox_t);
	memset(&xbox360, 0, sizeof(soft_xbox_t));
	memset(&(tx.soft_xbox), 0, sizeof(soft_xbox_t));
	sendto(GetSocketFd(), (const char*)&tx, length, 0, (struct sockaddr*)GetSocketAddress(), sizeof(SOCKADDR_IN));
	SOCKADDR_IN sclient;
	int clen = sizeof(sclient);
	err = recvfrom(GetSocketFd(), (char*)&rx, 1024, 0, (struct sockaddr*)&sclient, &clen);
	return XboxAskRxHandle(&rx,&tx);
}

/*
手柄上键状态设置。
0：松开
1：按下
*/
int Xbox_UP(int isdown)
{	int err;
	if (GetSocketFd() <= 0)		return err_creat_socket;
	WaitForSingleObject(hMutex_busy(), INFINITE);
	tx.head.indexpts++;				//指令统计值
	tx.head.cmd = cmd_xbox_send;	//指令
	tx.head.rand = rand();			//随机混淆值
	tx.head.mac = xbox_mac;
	int length = sizeof(cmd_head_t) + sizeof(soft_xbox_t);
	xbox360.btnUp = isdown;
	memcpy(&(tx.soft_xbox), &xbox360, sizeof(soft_xbox_t));
	sendto(GetSocketFd(), (const char*)&tx, length, 0, (struct sockaddr*)GetSocketAddress(), sizeof(SOCKADDR_IN));
	SOCKADDR_IN sclient;
	int clen = sizeof(sclient);
	err = recvfrom(GetSocketFd(), (char*)&rx, 1024, 0, (struct sockaddr*)&sclient, &clen);
	return XboxAskRxHandle(&rx, &tx);
}

/*
手柄下键状态设置。
0：松开
1：按下
*/
int Xbox_Down(int isdown)
{
	int err;
	if (GetSocketFd() <= 0)		return err_creat_socket;
	WaitForSingleObject(hMutex_busy(), INFINITE);
	tx.head.indexpts++;				//指令统计值
	tx.head.cmd = cmd_xbox_send;	//指令
	tx.head.rand = rand();			//随机混淆值
	tx.head.mac = xbox_mac;
	int length = sizeof(cmd_head_t) + sizeof(soft_xbox_t);
	xbox360.btnDown = isdown;
	memcpy(&(tx.soft_xbox), &xbox360, sizeof(soft_xbox_t));
	sendto(GetSocketFd(), (const char*)&tx, length, 0, (struct sockaddr*)GetSocketAddress(), sizeof(SOCKADDR_IN));
	SOCKADDR_IN sclient;
	int clen = sizeof(sclient);
	err = recvfrom(GetSocketFd(), (char*)&rx, 1024, 0, (struct sockaddr*)&sclient, &clen);
	return XboxAskRxHandle(&rx, &tx);
}


/*
手柄左键状态设置。
0：松开
1：按下
*/
int Xbox_Left(int isdown)
{
	int err;
	if (GetSocketFd() <= 0)		return err_creat_socket;
	WaitForSingleObject(hMutex_busy(), INFINITE);
	tx.head.indexpts++;				//指令统计值
	tx.head.cmd = cmd_xbox_send;	//指令
	tx.head.rand = rand();			//随机混淆值
	tx.head.mac = xbox_mac;
	int length = sizeof(cmd_head_t) + sizeof(soft_xbox_t);
	xbox360.btnLeft = isdown;
	memcpy(&(tx.soft_xbox), &xbox360, sizeof(soft_xbox_t));
	sendto(GetSocketFd(), (const char*)&tx, length, 0, (struct sockaddr*)GetSocketAddress(), sizeof(SOCKADDR_IN));
	SOCKADDR_IN sclient;
	int clen = sizeof(sclient);
	err = recvfrom(GetSocketFd(), (char*)&rx, 1024, 0, (struct sockaddr*)&sclient, &clen);
	return XboxAskRxHandle(&rx, &tx);
}

/*
手柄右键状态设置。
0：松开
1：按下
*/
int Xbox_Right(int isdown)
{
	int err;
	if (GetSocketFd() <= 0)		return err_creat_socket;
	WaitForSingleObject(hMutex_busy(), INFINITE);
	tx.head.indexpts++;				//指令统计值
	tx.head.cmd = cmd_xbox_send;	//指令
	tx.head.rand = rand();			//随机混淆值
	tx.head.mac = xbox_mac;
	int length = sizeof(cmd_head_t) + sizeof(soft_xbox_t);
	xbox360.btnRight = isdown;
	memcpy(&(tx.soft_xbox), &xbox360, sizeof(soft_xbox_t));
	sendto(GetSocketFd(), (const char*)&tx, length, 0, (struct sockaddr*)GetSocketAddress(), sizeof(SOCKADDR_IN));
	SOCKADDR_IN sclient;
	int clen = sizeof(sclient);
	err = recvfrom(GetSocketFd(), (char*)&rx, 1024, 0, (struct sockaddr*)&sclient, &clen);
	return XboxAskRxHandle(&rx, &tx);
}

/*
手柄上右键状态设置。
0：松开
1：按下
*/
int Xbox_UP_Right(int isdown)
{
	int err;
	if (GetSocketFd() <= 0)		return err_creat_socket;
	WaitForSingleObject(hMutex_busy(), INFINITE);
	tx.head.indexpts++;				//指令统计值
	tx.head.cmd = cmd_xbox_send;	//指令
	tx.head.rand = rand();			//随机混淆值
	tx.head.mac = xbox_mac;
	int length = sizeof(cmd_head_t) + sizeof(soft_xbox_t);
	xbox360.btnUp    = isdown;
	xbox360.btnRight = isdown;
	memcpy(&(tx.soft_xbox), &xbox360, sizeof(soft_xbox_t));
	sendto(GetSocketFd(), (const char*)&tx, length, 0, (struct sockaddr*)GetSocketAddress(), sizeof(SOCKADDR_IN));
	SOCKADDR_IN sclient;
	int clen = sizeof(sclient);
	err = recvfrom(GetSocketFd(), (char*)&rx, 1024, 0, (struct sockaddr*)&sclient, &clen);
	return XboxAskRxHandle(&rx, &tx);
}

/*
手柄右下键状态设置。
0：松开
1：按下
*/
int Xbox_Right_Down(int isdown)
{
	int err;
	if (GetSocketFd() <= 0)		return err_creat_socket;
	WaitForSingleObject(hMutex_busy(), INFINITE);
	tx.head.indexpts++;				//指令统计值
	tx.head.cmd = cmd_xbox_send;	//指令
	tx.head.rand = rand();			//随机混淆值
	tx.head.mac = xbox_mac;
	int length = sizeof(cmd_head_t) + sizeof(soft_xbox_t);
	xbox360.btnDown = isdown;
	xbox360.btnRight = isdown;
	memcpy(&(tx.soft_xbox), &xbox360, sizeof(soft_xbox_t));
	sendto(GetSocketFd(), (const char*)&tx, length, 0, (struct sockaddr*)GetSocketAddress(), sizeof(SOCKADDR_IN));
	SOCKADDR_IN sclient;
	int clen = sizeof(sclient);
	err = recvfrom(GetSocketFd(), (char*)&rx, 1024, 0, (struct sockaddr*)&sclient, &clen);
	return XboxAskRxHandle(&rx, &tx);
}

/*
手柄下左键状态设置。
0：松开
1：按下
*/
int Xbox_Left_Down(int isdown)
{
	int err;
	if (GetSocketFd() <= 0)		return err_creat_socket;
	WaitForSingleObject(hMutex_busy(), INFINITE);
	tx.head.indexpts++;				//指令统计值
	tx.head.cmd = cmd_xbox_send;	//指令
	tx.head.rand = rand();			//随机混淆值
	tx.head.mac = xbox_mac;
	int length = sizeof(cmd_head_t) + sizeof(soft_xbox_t);
	xbox360.btnDown = isdown;
	xbox360.btnLeft = isdown;
	memcpy(&(tx.soft_xbox), &xbox360, sizeof(soft_xbox_t));
	sendto(GetSocketFd(), (const char*)&tx, length, 0, (struct sockaddr*)GetSocketAddress(), sizeof(SOCKADDR_IN));
	SOCKADDR_IN sclient;
	int clen = sizeof(sclient);
	err = recvfrom(GetSocketFd(), (char*)&rx, 1024, 0, (struct sockaddr*)&sclient, &clen);
	return XboxAskRxHandle(&rx, &tx);
}


/*
手柄左上键状态设置。
0：松开
1：按下
*/
int Xbox_Left_Up(int isdown)
{
	int err;
	if (GetSocketFd() <= 0)		return err_creat_socket;
	WaitForSingleObject(hMutex_busy(), INFINITE);
	tx.head.indexpts++;				//指令统计值
	tx.head.cmd = cmd_xbox_send;	//指令
	tx.head.rand = rand();			//随机混淆值
	tx.head.mac = xbox_mac;
	int length = sizeof(cmd_head_t) + sizeof(soft_xbox_t);
	xbox360.btnUp = isdown;
	xbox360.btnLeft = isdown;
	memcpy(&(tx.soft_xbox), &xbox360, sizeof(soft_xbox_t));
	sendto(GetSocketFd(), (const char*)&tx, length, 0, (struct sockaddr*)GetSocketAddress(), sizeof(SOCKADDR_IN));
	SOCKADDR_IN sclient;
	int clen = sizeof(sclient);
	err = recvfrom(GetSocketFd(), (char*)&rx, 1024, 0, (struct sockaddr*)&sclient, &clen);
	return XboxAskRxHandle(&rx, &tx);
}



/*
手柄home键状态设置。
0：松开
1：按下
*/
int Xbox_Home(int isdown)
{
	int err;
	if (GetSocketFd() <= 0)		return err_creat_socket;
	WaitForSingleObject(hMutex_busy(), INFINITE);
	tx.head.indexpts++;				//指令统计值
	tx.head.cmd = cmd_xbox_send;	//指令
	tx.head.rand = rand();			//随机混淆值
	tx.head.mac = xbox_mac;
	int length = sizeof(cmd_head_t) + sizeof(soft_xbox_t);
	xbox360.btnHome = isdown;
	memcpy(&(tx.soft_xbox), &xbox360, sizeof(soft_xbox_t));
	sendto(GetSocketFd(), (const char*)&tx, length, 0, (struct sockaddr*)GetSocketAddress(), sizeof(SOCKADDR_IN));
	SOCKADDR_IN sclient;
	int clen = sizeof(sclient);
	err = recvfrom(GetSocketFd(), (char*)&rx, 1024, 0, (struct sockaddr*)&sclient, &clen);
	return XboxAskRxHandle(&rx, &tx);
}


/*
手柄Back键状态设置。
0：松开
1：按下
*/
int Xbox_Back(int isdown)
{
	int err;
	if (GetSocketFd() <= 0)		return err_creat_socket;
	WaitForSingleObject(hMutex_busy(), INFINITE);
	tx.head.indexpts++;				//指令统计值
	tx.head.cmd = cmd_xbox_send;	//指令
	tx.head.rand = rand();			//随机混淆值
	tx.head.mac = xbox_mac;
	int length = sizeof(cmd_head_t) + sizeof(soft_xbox_t);
	xbox360.btnBack = isdown;
	memcpy(&(tx.soft_xbox), &xbox360, sizeof(soft_xbox_t));
	sendto(GetSocketFd(), (const char*)&tx, length, 0, (struct sockaddr*)GetSocketAddress(), sizeof(SOCKADDR_IN));
	SOCKADDR_IN sclient;
	int clen = sizeof(sclient);
	err = recvfrom(GetSocketFd(), (char*)&rx, 1024, 0, (struct sockaddr*)&sclient, &clen);
	return XboxAskRxHandle(&rx, &tx);
}



/*
手柄Start键状态设置。
0：松开
1：按下
*/
int Xbox_Start(int isdown)
{
	int err;
	if (GetSocketFd() <= 0)		return err_creat_socket;
	WaitForSingleObject(hMutex_busy(), INFINITE);
	tx.head.indexpts++;				//指令统计值
	tx.head.cmd = cmd_xbox_send;	//指令
	tx.head.rand = rand();			//随机混淆值
	tx.head.mac = xbox_mac;
	int length = sizeof(cmd_head_t) + sizeof(soft_xbox_t);
	xbox360.btnStart = isdown;
	memcpy(&(tx.soft_xbox), &xbox360, sizeof(soft_xbox_t));
	sendto(GetSocketFd(), (const char*)&tx, length, 0, (struct sockaddr*)GetSocketAddress(), sizeof(SOCKADDR_IN));
	SOCKADDR_IN sclient;
	int clen = sizeof(sclient);
	err = recvfrom(GetSocketFd(), (char*)&rx, 1024, 0, (struct sockaddr*)&sclient, &clen);
	return XboxAskRxHandle(&rx, &tx);
}


/*
手柄左摇杆按键状态设置。
0：松开
1：按下
*/
int Xbox_L3(int isdown)
{
	int err;
	if (GetSocketFd() <= 0)		return err_creat_socket;
	WaitForSingleObject(hMutex_busy(), INFINITE);
	tx.head.indexpts++;				//指令统计值
	tx.head.cmd = cmd_xbox_send;	//指令
	tx.head.rand = rand();			//随机混淆值
	tx.head.mac = xbox_mac;
	int length = sizeof(cmd_head_t) + sizeof(soft_xbox_t);
	xbox360.btnL3 = isdown;
	memcpy(&(tx.soft_xbox), &xbox360, sizeof(soft_xbox_t));
	sendto(GetSocketFd(), (const char*)&tx, length, 0, (struct sockaddr*)GetSocketAddress(), sizeof(SOCKADDR_IN));
	SOCKADDR_IN sclient;
	int clen = sizeof(sclient);
	err = recvfrom(GetSocketFd(), (char*)&rx, 1024, 0, (struct sockaddr*)&sclient, &clen);
	return XboxAskRxHandle(&rx, &tx);
}

/*
手柄右摇杆按键状态设置。
0：松开
1：按下
*/
int Xbox_R3(int isdown)
{
	int err;
	if (GetSocketFd() <= 0)		return err_creat_socket;
	WaitForSingleObject(hMutex_busy(), INFINITE);
	tx.head.indexpts++;				//指令统计值
	tx.head.cmd = cmd_xbox_send;	//指令
	tx.head.rand = rand();			//随机混淆值
	tx.head.mac = xbox_mac;
	int length = sizeof(cmd_head_t) + sizeof(soft_xbox_t);
	xbox360.btnR3 = isdown;
	memcpy(&(tx.soft_xbox), &xbox360, sizeof(soft_xbox_t));
	sendto(GetSocketFd(), (const char*)&tx, length, 0, (struct sockaddr*)GetSocketAddress(), sizeof(SOCKADDR_IN));
	SOCKADDR_IN sclient;
	int clen = sizeof(sclient);
	err = recvfrom(GetSocketFd(), (char*)&rx, 1024, 0, (struct sockaddr*)&sclient, &clen);
	return XboxAskRxHandle(&rx, &tx);
}



/*
手柄A键状态设置。
0：松开
1：按下
*/
int Xbox_A(int isdown)
{
	int err;
	if (GetSocketFd() <= 0)		return err_creat_socket;
	WaitForSingleObject(hMutex_busy(), INFINITE);
	tx.head.indexpts++;				//指令统计值
	tx.head.cmd = cmd_xbox_send;	//指令
	tx.head.rand = rand();			//随机混淆值
	tx.head.mac = xbox_mac;
	int length = sizeof(cmd_head_t) + sizeof(soft_xbox_t);
	xbox360.btnA = isdown;
	memcpy(&(tx.soft_xbox), &xbox360, sizeof(soft_xbox_t));
	sendto(GetSocketFd(), (const char*)&tx, length, 0, (struct sockaddr*)GetSocketAddress(), sizeof(SOCKADDR_IN));
	SOCKADDR_IN sclient;
	int clen = sizeof(sclient);
	err = recvfrom(GetSocketFd(), (char*)&rx, 1024, 0, (struct sockaddr*)&sclient, &clen);
	return XboxAskRxHandle(&rx, &tx);
}


/*
手柄B键状态设置。
0：松开
1：按下
*/
int Xbox_B(int isdown)
{
	int err;
	if (GetSocketFd() <= 0)		return err_creat_socket;
	WaitForSingleObject(hMutex_busy(), INFINITE);
	tx.head.indexpts++;				//指令统计值
	tx.head.cmd = cmd_xbox_send;	//指令
	tx.head.rand = rand();			//随机混淆值
	tx.head.mac = xbox_mac;
	int length = sizeof(cmd_head_t) + sizeof(soft_xbox_t);
	xbox360.btnB = isdown;
	memcpy(&(tx.soft_xbox), &xbox360, sizeof(soft_xbox_t));
	sendto(GetSocketFd(), (const char*)&tx, length, 0, (struct sockaddr*)GetSocketAddress(), sizeof(SOCKADDR_IN));
	SOCKADDR_IN sclient;
	int clen = sizeof(sclient);
	err = recvfrom(GetSocketFd(), (char*)&rx, 1024, 0, (struct sockaddr*)&sclient, &clen);
	return XboxAskRxHandle(&rx, &tx);
}

/*
手柄X键状态设置。
0：松开
1：按下
*/
int Xbox_X(int isdown)
{
	int err;
	if (GetSocketFd() <= 0)		return err_creat_socket;
	WaitForSingleObject(hMutex_busy(), INFINITE);
	tx.head.indexpts++;				//指令统计值
	tx.head.cmd = cmd_xbox_send;	//指令
	tx.head.rand = rand();			//随机混淆值
	tx.head.mac = xbox_mac;
	int length = sizeof(cmd_head_t) + sizeof(soft_xbox_t);
	xbox360.btnX = isdown;
	memcpy(&(tx.soft_xbox), &xbox360, sizeof(soft_xbox_t));
	sendto(GetSocketFd(), (const char*)&tx, length, 0, (struct sockaddr*)GetSocketAddress(), sizeof(SOCKADDR_IN));
	SOCKADDR_IN sclient;
	int clen = sizeof(sclient);
	err = recvfrom(GetSocketFd(), (char*)&rx, 1024, 0, (struct sockaddr*)&sclient, &clen);
	return XboxAskRxHandle(&rx, &tx);
}

/*
手柄Y键状态设置。
0：松开
1：按下
*/
int Xbox_Y(int isdown)
{
	int err;
	if (GetSocketFd() <= 0)		return err_creat_socket;
	WaitForSingleObject(hMutex_busy(), INFINITE);
	tx.head.indexpts++;				//指令统计值
	tx.head.cmd = cmd_xbox_send;	//指令
	tx.head.rand = rand();			//随机混淆值
	tx.head.mac = xbox_mac;
	int length = sizeof(cmd_head_t) + sizeof(soft_xbox_t);
	xbox360.btnY = isdown;
	memcpy(&(tx.soft_xbox), &xbox360, sizeof(soft_xbox_t));
	sendto(GetSocketFd(), (const char*)&tx, length, 0, (struct sockaddr*)GetSocketAddress(), sizeof(SOCKADDR_IN));
	SOCKADDR_IN sclient;
	int clen = sizeof(sclient);
	err = recvfrom(GetSocketFd(), (char*)&rx, 1024, 0, (struct sockaddr*)&sclient, &clen);
	return XboxAskRxHandle(&rx, &tx);
}

/*
手柄LB键状态设置。
0：松开
1：按下
*/
int Xbox_LB(int isdown)
{
	int err;
	if (GetSocketFd() <= 0)		return err_creat_socket;
	WaitForSingleObject(hMutex_busy(), INFINITE);
	tx.head.indexpts++;				//指令统计值
	tx.head.cmd = cmd_xbox_send;	//指令
	tx.head.rand = rand();			//随机混淆值
	tx.head.mac = xbox_mac;
	int length = sizeof(cmd_head_t) + sizeof(soft_xbox_t);
	xbox360.btnLB = isdown;
	memcpy(&(tx.soft_xbox), &xbox360, sizeof(soft_xbox_t));
	sendto(GetSocketFd(), (const char*)&tx, length, 0, (struct sockaddr*)GetSocketAddress(), sizeof(SOCKADDR_IN));
	SOCKADDR_IN sclient;
	int clen = sizeof(sclient);
	err = recvfrom(GetSocketFd(), (char*)&rx, 1024, 0, (struct sockaddr*)&sclient, &clen);
	return XboxAskRxHandle(&rx, &tx);
}


/*
手柄RB键状态设置。
0：松开
1：按下
*/
int Xbox_RB(int isdown)
{
	int err;
	if (GetSocketFd() <= 0)		return err_creat_socket;
	WaitForSingleObject(hMutex_busy(), INFINITE);
	tx.head.indexpts++;				//指令统计值
	tx.head.cmd = cmd_xbox_send;	//指令
	tx.head.rand = rand();			//随机混淆值
	tx.head.mac = xbox_mac;
	int length = sizeof(cmd_head_t) + sizeof(soft_xbox_t);
	xbox360.btnRB = isdown;
	memcpy(&(tx.soft_xbox), &xbox360, sizeof(soft_xbox_t));
	sendto(GetSocketFd(), (const char*)&tx, length, 0, (struct sockaddr*)GetSocketAddress(), sizeof(SOCKADDR_IN));
	SOCKADDR_IN sclient;
	int clen = sizeof(sclient);
	err = recvfrom(GetSocketFd(), (char*)&rx, 1024, 0, (struct sockaddr*)&sclient, &clen);
	return XboxAskRxHandle(&rx, &tx);
}



/*
手柄左摇杆坐标设置。
x:摇杆x的值
y:摇杆y的值
*/
int Xbox_Left_joystick(short x,short y)
{	int err;
	if (GetSocketFd() <= 0)		return err_creat_socket;
	WaitForSingleObject(hMutex_busy(), INFINITE);
	tx.head.indexpts++;				//指令统计值
	tx.head.cmd = cmd_xbox_send;	//指令
	tx.head.rand = rand();			//随机混淆值
	tx.head.mac = xbox_mac;
	int length = sizeof(cmd_head_t) + sizeof(soft_xbox_t);
	xbox360.left_joystick.x = x;
	xbox360.left_joystick.y = y;
	memcpy(&(tx.soft_xbox), &xbox360, sizeof(soft_xbox_t));
	sendto(GetSocketFd(), (const char*)&tx, length, 0, (struct sockaddr*)GetSocketAddress(), sizeof(SOCKADDR_IN));
	SOCKADDR_IN sclient;
	int clen = sizeof(sclient);
	err = recvfrom(GetSocketFd(), (char*)&rx, 1024, 0, (struct sockaddr*)&sclient, &clen);
	return XboxAskRxHandle(&rx, &tx);
}



/*
手柄右摇杆坐标设置。
x:摇杆x的值
y:摇杆y的值
*/
int Xbox_Right_joystick(short x, short y)
{	int err;
	if (GetSocketFd() <= 0)		return err_creat_socket;
	WaitForSingleObject(hMutex_busy(), INFINITE);
	tx.head.indexpts++;				//指令统计值
	tx.head.cmd = cmd_xbox_send;	//指令
	tx.head.rand = rand();			//随机混淆值
	tx.head.mac = xbox_mac;
	int length = sizeof(cmd_head_t) + sizeof(soft_xbox_t);
	xbox360.right_joystick.x = x;
	xbox360.right_joystick.y = y;
	memcpy(&(tx.soft_xbox), &xbox360, sizeof(soft_xbox_t));
	sendto(GetSocketFd(), (const char*)&tx, length, 0, (struct sockaddr*)GetSocketAddress(), sizeof(SOCKADDR_IN));
	SOCKADDR_IN sclient;
	int clen = sizeof(sclient);
	err = recvfrom(GetSocketFd(), (char*)&rx, 1024, 0, (struct sockaddr*)&sclient, &clen);
	return XboxAskRxHandle(&rx, &tx);
}

int Xbox_LT(int value)	//手柄LT键状态设置。取值范围[0,255]
{
	int err;
	if (GetSocketFd() <= 0)		return err_creat_socket;
	WaitForSingleObject(hMutex_busy(), INFINITE);
	tx.head.indexpts++;				//指令统计值
	tx.head.cmd = cmd_xbox_send;	//指令
	tx.head.rand = rand();			//随机混淆值
	tx.head.mac = xbox_mac;
	int length = sizeof(cmd_head_t) + sizeof(soft_xbox_t);
	xbox360.LT= value;
	memcpy(&(tx.soft_xbox), &xbox360, sizeof(soft_xbox_t));
	sendto(GetSocketFd(), (const char*)&tx, length, 0, (struct sockaddr*)GetSocketAddress(), sizeof(SOCKADDR_IN));
	SOCKADDR_IN sclient;
	int clen = sizeof(sclient);
	err = recvfrom(GetSocketFd(), (char*)&rx, 1024, 0, (struct sockaddr*)&sclient, &clen);
	return XboxAskRxHandle(&rx, &tx);

}
int Xbox_RT(int value)	//手柄LT键状态设置。取值范围[0,255]
{
	int err;
	if (GetSocketFd() <= 0)		return err_creat_socket;
	WaitForSingleObject(hMutex_busy(), INFINITE);
	tx.head.indexpts++;				//指令统计值
	tx.head.cmd = cmd_xbox_send;	//指令
	tx.head.rand = rand();			//随机混淆值
	tx.head.mac = xbox_mac;
	int length = sizeof(cmd_head_t) + sizeof(soft_xbox_t);
	xbox360.RT = value;
	memcpy(&(tx.soft_xbox), &xbox360, sizeof(soft_xbox_t));
	sendto(GetSocketFd(), (const char*)&tx, length, 0, (struct sockaddr*)GetSocketAddress(), sizeof(SOCKADDR_IN));
	SOCKADDR_IN sclient;
	int clen = sizeof(sclient);
	err = recvfrom(GetSocketFd(), (char*)&rx, 1024, 0, (struct sockaddr*)&sclient, &clen);
	return XboxAskRxHandle(&rx, &tx);

}