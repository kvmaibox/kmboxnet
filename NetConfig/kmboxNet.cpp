#include "kmboxNet.h"
#include "HidTable.h"
#include <time.h>
SOCKET sockClientfd=0;					//网络通信句柄
client_tx tx;							//发送的内容
client_tx rx;							//接收的内容
SOCKADDR_IN addrSrv;
soft_mouse_t    softmouse;				//软件鼠标数据
soft_keyboard_t softkeyboard;			//软件键盘数据
static int monitor_run=0;				//物理键鼠监控是否运行
static int mask_keyboard_mouse_flag=0;	//键鼠屏蔽状态
#define monitor_begin 1
#define monitor_ok    2
#define monitor_exit  0

#pragma pack(1)
typedef struct {
	unsigned char report_id;
	unsigned char buttons;		// 8 buttons available
	short x;					// -32767 to 32767
	short y;					// -32767 to 32767
	short wheel;				// -32767 to 32767
}standard_mouse_report_t;

typedef struct {
	unsigned char report_id;
	unsigned char buttons;      // 8 buttons控制键
	unsigned char data[10];     //常规按键
}standard_keyboard_report_t;
#pragma pack()

standard_mouse_report_t		hw_mouse;   //硬件鼠标消息
standard_keyboard_report_t	hw_keyboard;//硬件键盘消息

//生成一个A到B之间的随机数
int myrand(int a, int b)
{	int min = a < b ? a : b;
	int max = a > b ? a : b;
	return ((rand() % (max - min)) + min);
}

int StrToHex( char* pbSrc, int nLen)
{	char h1, h2;
	unsigned char s1, s2;
	int i;
	char pbDest[16] = { 0 };
	for (i = 0; i < nLen; i++)	{
		h1 = pbSrc[2 * i];
		h2 = pbSrc[2 * i + 1];
		s1 = toupper(h1) - 0x30;
		if (s1 > 9)
			s1 -= 7;
		s2 = toupper(h2) - 0x30;
		if (s2 > 9)
			s2 -= 7;
		pbDest[i] = s1 * 16 + s2;
	}
	return pbDest[0]<<24| pbDest[1]<<16| pbDest[2]<<8| pbDest[3];
}

int NetRxReturnHandle(client_tx *rx,client_tx *tx)		 //接收的内容
{
	if (rx->head.cmd != tx->head.cmd)
		return  err_net_cmd;//命令码错误
	if (rx->head.indexpts != tx->head.indexpts)
		return  err_net_pts;//时间戳错误
	return  rx->head.rand;//真正的返回值


}

/*
连接kmboxNet盒子输入参数分别是盒子
ip  ：盒子的IP地址 （显示屏上会有显示,例如：192.168.2.88）
port: 通信端口号   （显示屏上会有显示，例如：6234）
mac : 盒子的mac地址（显示屏幕上有显示，例如：12345）
返回值:参见枚举类型
*/
int kmNet_init(char* ip, char* port, char* mac)
{	WORD wVersionRequested;WSADATA wsaData;	int err;
	wVersionRequested = MAKEWORD(1, 1);
	err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0) 		return err_creat_socket;
	if (LOBYTE(wsaData.wVersion) != 1 || HIBYTE(wsaData.wVersion) != 1) {
		WSACleanup(); sockClientfd = -1;
		return err_net_version;
	}
	srand((unsigned)time(NULL));
	sockClientfd = socket(AF_INET, SOCK_DGRAM, 0);
	addrSrv.sin_addr.S_un.S_addr = inet_addr(ip);
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(atoi(port));//端口UUID[1]>>16高16位
	tx.head.mac		 = StrToHex(mac, 4); //盒子的mac 固定 UUID[1]
	tx.head.rand	 = rand();
	tx.head.indexpts = 0;		   //指令统计值
	tx.head.cmd		 = cmd_connect;//指令
	memset(&softmouse, 0, sizeof(softmouse));		//软件数据清零
	memset(&softkeyboard, 0, sizeof(softkeyboard));
	err=sendto(sockClientfd, (const char *)&tx, sizeof(cmd_head_t), 0, (struct sockaddr *) & addrSrv, sizeof(addrSrv));
	Sleep(20);//第一次连接可能时间比较久
	int clen = sizeof(addrSrv);
	err=recvfrom(sockClientfd, (char*)&rx, 1024, 0, (struct sockaddr*)&addrSrv, &clen);
	if (err < 0)
		return err_net_rx_timeout;
	return NetRxReturnHandle(&rx, &tx);
}

/*
鼠标移动x,y个单位。一次性移动。无轨迹模拟，速度最快.
自己写轨迹移动时使用此函数。
返回值：0正常执行，其他值异常。
*/
int kmNet_mouse_move(short x, short y)
{
	int err;
	if (sockClientfd <= 0)		return err_creat_socket;
	tx.head.indexpts++;				//指令统计值
	tx.head.cmd = cmd_mouse_move;	//指令
	tx.head.rand = rand();			//随机混淆值
	softmouse.x = x;
	softmouse.y = y;
	memcpy(&tx.cmd_mouse, &softmouse, sizeof(soft_mouse_t));
	int length = sizeof(cmd_head_t) + sizeof(soft_mouse_t);
	sendto(sockClientfd, (const char*)&tx, length, 0, (struct sockaddr*)&addrSrv, sizeof(addrSrv));
	softmouse.x = 0;
	softmouse.y = 0;
	SOCKADDR_IN sclient;
	int clen = sizeof(sclient);
	err = recvfrom(sockClientfd, (char*)&rx, 1024, 0, (struct sockaddr*)&sclient, &clen);
	if (err < 0)
		return err_net_rx_timeout;
	return NetRxReturnHandle(&rx, &tx);
}



/*
鼠标左键控制
isdown :0松开 ，1按下
返回值：0正常执行，其他值异常。
*/
int kmNet_mouse_left(int isdown)
{
	int err;
	if (sockClientfd <= 0)		return err_creat_socket;
	tx.head.indexpts++;				//指令统计值
	tx.head.cmd = cmd_mouse_left;	//指令
	tx.head.rand = rand();			//随机混淆值
	softmouse.button = (isdown ? (softmouse.button|0x01) :(softmouse.button&(~0x01)));
	memcpy(&tx.cmd_mouse, &softmouse, sizeof(soft_mouse_t));
	int length = sizeof(cmd_head_t) + sizeof(soft_mouse_t);
	sendto(sockClientfd, (const char*)&tx, length, 0, (struct sockaddr*)&addrSrv, sizeof(addrSrv));
	SOCKADDR_IN sclient;
	int clen = sizeof(sclient);
	err = recvfrom(sockClientfd, (char*)&rx, 1024, 0, (struct sockaddr*)&sclient, &clen);
	if (err < 0)
		return err_net_rx_timeout;
	return NetRxReturnHandle(&rx, &tx);
}

/*
鼠标中键控制
isdown :0松开 ，1按下
返回值：0正常执行，其他值异常。
*/
int kmNet_mouse_middle(int isdown)
{
	int err;
	if (sockClientfd <= 0)		return err_creat_socket;
	tx.head.indexpts++;				//指令统计值
	tx.head.cmd = cmd_mouse_middle;	//指令
	tx.head.rand = rand();			//随机混淆值
	softmouse.button = (isdown ? (softmouse.button | 0x04) : (softmouse.button & (~0x04)));
	memcpy(&tx.cmd_mouse, &softmouse, sizeof(soft_mouse_t));
	int length = sizeof(cmd_head_t) + sizeof(soft_mouse_t);
	sendto(sockClientfd, (const char*)&tx, length, 0, (struct sockaddr*)&addrSrv, sizeof(addrSrv));
	SOCKADDR_IN sclient;
	int clen = sizeof(sclient);
	err = recvfrom(sockClientfd, (char*)&rx, 1024, 0, (struct sockaddr*)&sclient, &clen);
	if (err < 0)
		return err_net_rx_timeout;
	return NetRxReturnHandle(&rx, &tx);
}

/*
鼠标右键控制
isdown :0松开 ，1按下
返回值：0正常执行，其他值异常。
*/
int kmNet_mouse_right(int isdown)
{	int err;
	if (sockClientfd <= 0)		return err_creat_socket;
	tx.head.indexpts++;				//指令统计值
	tx.head.cmd = cmd_mouse_right;	//指令
	tx.head.rand = rand();			//随机混淆值
	softmouse.button = (isdown ? (softmouse.button | 0x02) : (softmouse.button & (~0x02)));
	memcpy(&tx.cmd_mouse, &softmouse, sizeof(soft_mouse_t));
	int length = sizeof(cmd_head_t) + sizeof(soft_mouse_t);
	sendto(sockClientfd, (const char*)&tx, length, 0, (struct sockaddr*)&addrSrv, sizeof(addrSrv));
	SOCKADDR_IN sclient;
	int clen = sizeof(sclient);
	err = recvfrom(sockClientfd, (char*)&rx, 1024, 0, (struct sockaddr*)&sclient, &clen);
	if (err < 0)
		return err_net_rx_timeout;
	return NetRxReturnHandle(&rx, &tx);
}

//鼠标滚轮控制
int kmNet_mouse_wheel(int wheel)
{
	int err;
	if (sockClientfd <= 0)		return err_creat_socket;
	tx.head.indexpts++;				//指令统计值
	tx.head.cmd = cmd_mouse_wheel;	//指令
	tx.head.rand = rand();			//随机混淆值
	softmouse.wheel = wheel;
	memcpy(&tx.cmd_mouse, &softmouse, sizeof(soft_mouse_t));
	int length = sizeof(cmd_head_t) + sizeof(soft_mouse_t);
	softmouse.wheel = 0;
	sendto(sockClientfd, (const char*)&tx, length, 0, (struct sockaddr*)&addrSrv, sizeof(addrSrv));
	SOCKADDR_IN sclient;
	int clen = sizeof(sclient);
	err = recvfrom(sockClientfd, (char*)&rx, 1024, 0, (struct sockaddr*)&sclient, &clen);
	if (err < 0)
		return err_net_rx_timeout;
	return NetRxReturnHandle(&rx, &tx);
}


/*
鼠标全报告控制函数
*/
int kmNet_mouse_all(int button, int x, int y, int wheel)
{
	int err;
	if (sockClientfd <= 0)		return err_creat_socket;
	tx.head.indexpts++;				//指令统计值
	tx.head.cmd = cmd_mouse_wheel;	//指令
	tx.head.rand = rand();			//随机混淆值
	softmouse.button = button;
	softmouse.x = x;
	softmouse.y = y;
	softmouse.wheel = wheel;
	memcpy(&tx.cmd_mouse, &softmouse, sizeof(soft_mouse_t));
	int length = sizeof(cmd_head_t) + sizeof(soft_mouse_t);
	softmouse.x = 0;
	softmouse.y = 0;
	softmouse.wheel = 0;
	sendto(sockClientfd, (const char*)&tx, length, 0, (struct sockaddr*)&addrSrv, sizeof(addrSrv));
	SOCKADDR_IN sclient;
	int clen = sizeof(sclient);
	err = recvfrom(sockClientfd, (char*)&rx, 1024, 0, (struct sockaddr*)&sclient, &clen);
	if (err < 0)
		return err_net_rx_timeout;
	return NetRxReturnHandle(&rx, &tx);
}

/*
鼠标移动x,y个单位。模拟人为移动x,y个单位。不会出现键鼠异常的检测.
没有写移动曲线的推荐用此函数。此函数不会出现跳跃现象，按照最小步进逼近
目标点。耗时比kmNet_mouse_move高。
ms是设置移动需要多少毫秒.注意ms给的值不要太小，太小一样会出现键鼠数据异常。
尽量像人操作。实际用时会比ms小。
*/
int kmNet_mouse_move_auto(int x, int y,int ms)
{
	int err;
	if (sockClientfd <= 0)		return err_creat_socket;
	tx.head.indexpts++;				 //指令统计值
	tx.head.cmd = cmd_mouse_automove;//指令
	tx.head.rand = ms;			     //随机混淆值
	softmouse.x = x;
	softmouse.y = y;
	memcpy(&tx.cmd_mouse, &softmouse, sizeof(soft_mouse_t));
	int length = sizeof(cmd_head_t) + sizeof(soft_mouse_t);
	softmouse.x = 0;				//清零
	softmouse.y = 0;				//清零
	sendto(sockClientfd, (const char*)&tx, length, 0, (struct sockaddr*)&addrSrv, sizeof(addrSrv));
	SOCKADDR_IN sclient;
	int clen = sizeof(sclient);
	err = recvfrom(sockClientfd, (char*)&rx, 1024, 0, (struct sockaddr*)&sclient, &clen);
	if (err < 0)
		return err_net_rx_timeout;
	return NetRxReturnHandle(&rx, &tx);
}



/*
二阶贝塞尔曲线控制 
x,y 	:目标点坐标
ms		:拟合此过程用时（单位ms）
x1,y1	:控制点p1点坐标
x2,y2	:控制点p2点坐标
*/
int kmNet_mouse_move_beizer(int x, int y, int ms,int x1,int y1,int x2,int y2)
{
	int err;
	if (sockClientfd <= 0)		return err_creat_socket;
	tx.head.indexpts++;			 //指令统计值
	tx.head.cmd = cmd_bazerMove; //指令
	tx.head.rand = ms;			 //随机混淆值
	softmouse.x = x;
	softmouse.y = y;
	softmouse.point[0] = x1;
	softmouse.point[1] = y1;
	softmouse.point[2] = x2;
	softmouse.point[3] = y2;
	memcpy(&tx.cmd_mouse, &softmouse, sizeof(soft_mouse_t));
	int length = sizeof(cmd_head_t) + sizeof(soft_mouse_t);
	softmouse.x = 0;
	softmouse.y = 0;
	sendto(sockClientfd, (const char*)&tx, length, 0, (struct sockaddr*)&addrSrv, sizeof(addrSrv));
	SOCKADDR_IN sclient;
	int clen = sizeof(sclient);
	err = recvfrom(sockClientfd, (char*)&rx, 1024, 0, (struct sockaddr*)&sclient, &clen);
	if (err < 0)
		return err_net_rx_timeout;
	return NetRxReturnHandle(&rx, &tx);
}



int kmNet_keydown(int vk_key)
{	int i;
	if (vk_key >= KEY_LEFTCONTROL && vk_key <= KEY_RIGHT_GUI)//控制键
	{
		switch (vk_key)
		{
			case KEY_LEFTCONTROL: softkeyboard.ctrl |= BIT0; break;
			case KEY_LEFTSHIFT:   softkeyboard.ctrl |= BIT1; break;
			case KEY_LEFTALT:     softkeyboard.ctrl |= BIT2; break;
			case KEY_LEFT_GUI:    softkeyboard.ctrl |= BIT3; break;
			case KEY_RIGHTCONTROL:softkeyboard.ctrl |= BIT4; break;
			case KEY_RIGHTSHIFT:  softkeyboard.ctrl |= BIT5; break;
			case KEY_RIGHTALT:    softkeyboard.ctrl |= BIT6; break;
			case KEY_RIGHT_GUI:   softkeyboard.ctrl |= BIT7; break;
		}
	}
	else
	{//常规键  
		for (i = 0; i < 10; i++)//首先检查队列中是否存在vk_key
		{	if (softkeyboard.button[i] == vk_key)
				goto KM_down_send;// 队列里面已经有vk_key 直接发送就行
		}
		//队列里面没有vk_key 
		for (i = 0; i < 10; i++)//遍历所有的数据，将vk_key添加到队列里
		{
			if (softkeyboard.button[i] == 0)
			{// 队列里面已经有vk_key 直接发送就行
				softkeyboard.button[i] = vk_key;
				goto KM_down_send;
			}
		}
		//队列已经满了 那么就剔除最开始的那个
		memcpy(&softkeyboard.button[0], &softkeyboard.button[1], 10);
		softkeyboard.button[9] = vk_key;
	}
KM_down_send:
	int err;
	if (sockClientfd <= 0)		return err_creat_socket;
	tx.head.indexpts++;				//指令统计值
	tx.head.cmd = cmd_keyboard_all;	//指令
	tx.head.rand = rand();			// 随机混淆值
	memcpy(&tx.cmd_keyboard, &softkeyboard, sizeof(soft_keyboard_t));
	int length = sizeof(cmd_head_t) + sizeof(soft_keyboard_t);
	sendto(sockClientfd, (const char*)&tx, length, 0, (struct sockaddr*)&addrSrv, sizeof(addrSrv));
	SOCKADDR_IN sclient;
	int clen = sizeof(sclient);
	err = recvfrom(sockClientfd, (char*)&rx, 1024, 0, (struct sockaddr*)&sclient, &clen);
	if (err < 0)
		return err_net_rx_timeout;
	return NetRxReturnHandle(&rx, &tx);
}




int kmNet_keyup(int vk_key)
{
	int i;
	if (vk_key >= KEY_LEFTCONTROL && vk_key <= KEY_RIGHT_GUI)//控制键
	{
		switch (vk_key)
		{
		case KEY_LEFTCONTROL: softkeyboard.ctrl &= ~BIT0; break;
		case KEY_LEFTSHIFT:   softkeyboard.ctrl &= ~BIT1; break;
		case KEY_LEFTALT:     softkeyboard.ctrl &= ~BIT2; break;
		case KEY_LEFT_GUI:    softkeyboard.ctrl &= ~BIT3; break;
		case KEY_RIGHTCONTROL:softkeyboard.ctrl &= ~BIT4; break;
		case KEY_RIGHTSHIFT:  softkeyboard.ctrl &= ~BIT5; break;
		case KEY_RIGHTALT:    softkeyboard.ctrl &= ~BIT6; break;
		case KEY_RIGHT_GUI:   softkeyboard.ctrl &= ~BIT7; break;
		}
	}
	else
	{//常规键  
		for (i = 0; i < 10; i++)//首先检查队列中是否存在vk_key
		{
			if (softkeyboard.button[i] == vk_key)// 队列里面已经有vk_key 
			{
				memcpy(&softkeyboard.button[i], &softkeyboard.button[i + 1], 10 - i);
				softkeyboard.button[9] = 0;
				goto KM_up_send;
			}
		}
	}
KM_up_send:
	int err;
	if (sockClientfd <= 0)		return err_creat_socket;
	tx.head.indexpts++;				//指令统计值
	tx.head.cmd = cmd_keyboard_all;	//指令
	tx.head.rand = rand();			// 随机混淆值
	memcpy(&tx.cmd_keyboard, &softkeyboard, sizeof(soft_keyboard_t));
	int length = sizeof(cmd_head_t) + sizeof(soft_keyboard_t);
	sendto(sockClientfd, (const char*)&tx, length, 0, (struct sockaddr*)&addrSrv, sizeof(addrSrv));
	SOCKADDR_IN sclient;
	int clen = sizeof(sclient);
	err = recvfrom(sockClientfd, (char*)&rx, 1024, 0, (struct sockaddr*)&sclient, &clen);
	if (err < 0)
		return err_net_rx_timeout;
	return NetRxReturnHandle(&rx, &tx);
}


//重启盒子
int kmNet_reboot(void)
{
	int err;
	if (sockClientfd <= 0)		return err_creat_socket;
	tx.head.indexpts++;				//指令统计值
	tx.head.cmd = cmd_reboot;		//指令
	tx.head.rand = rand();			// 随机混淆值
	int length = sizeof(cmd_head_t);
	sendto(sockClientfd, (const char*)&tx, length, 0, (struct sockaddr*)&addrSrv, sizeof(addrSrv));
	SOCKADDR_IN sclient;
	int clen = sizeof(sclient);
	err = recvfrom(sockClientfd, (char*)&rx, 1024, 0, (struct sockaddr*)&sclient, &clen);
	WSACleanup();
	sockClientfd = -1;
	if (err < 0)
		return err_net_rx_timeout;
	return NetRxReturnHandle(&rx, &tx);

}



//监听物理键鼠
static HANDLE handle_listen = NULL;
DWORD WINAPI ThreadListenProcess(LPVOID lpParameter)
{
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);			//创建套接字，SOCK_DGRAM指明使用 UDP 协议
	SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);	//绑定套接字
	sockaddr_in servAddr;
	memset(&servAddr, 0, sizeof(servAddr));			 //每个字节都用0填充
	servAddr.sin_family = PF_INET;					//使用IPv4地址
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);	 //自动获取IP地址
	servAddr.sin_port = htons(addrSrv.sin_port+1);  //端口
	bind(sock, (SOCKADDR*)&servAddr, sizeof(SOCKADDR));
	SOCKADDR cliAddr;  //客户端地址信息
	int nSize = sizeof(SOCKADDR);
	char buff[1024];  //缓冲区
	monitor_run = monitor_ok;
	while (monitor_run) {
		int strLen = recvfrom(sock, buff, 1024, 0, &cliAddr, &nSize); 
		memcpy(&hw_mouse, buff, sizeof(hw_mouse));							//物理鼠标状态
		memcpy(&hw_keyboard, &buff[sizeof(hw_mouse)], sizeof(hw_keyboard));	//物理键盘状态
	}
	closesocket(sock);
	return 0;

}

//使能键鼠监控
int kmNet_monitor(char enable)
{
	int err;
	if (sockClientfd <= 0)		return err_creat_socket;
	tx.head.indexpts++;				//指令统计值
	tx.head.cmd = cmd_monitor;		//指令
	if(enable)
	tx.head.rand = (addrSrv .sin_port+1)|0xaa55<<16;	// 随机混淆值
	else 
	tx.head.rand = 0;	// 随机混淆值
	int length = sizeof(cmd_head_t);
	sendto(sockClientfd, (const char*)&tx, length, 0, (struct sockaddr*)&addrSrv, sizeof(addrSrv));
	SOCKADDR_IN sclient;
	int clen = sizeof(sclient);
	err = recvfrom(sockClientfd, (char*)&rx, 1024, 0, (struct sockaddr*)&sclient, &clen);
	if (enable)//打开监听功能
	{
		do
		{	if (handle_listen == NULL)
			{
				DWORD lpThreadID;
				monitor_run = monitor_begin;
				handle_listen = CreateThread(NULL, 0, ThreadListenProcess, NULL, 0, &lpThreadID);
			}
			Sleep(10);
		} while (monitor_run!= monitor_ok); //等待监听线程启动
	}
	else {
		handle_listen == NULL;
		monitor_run = monitor_exit;
	}
	if (err < 0)
		return err_net_rx_timeout;
	return NetRxReturnHandle(&rx, &tx);
}


/*
监听物理鼠标左键状态
返回值
-1：还未开启监听功能 需要先调用kmNet_monitor（1）
0 :物理鼠标左键松开
1：物理鼠标左键按下
*/
int kmNet_monitor_mouse_left()
{
	if (monitor_run != monitor_ok) return -1;
	return (hw_mouse.buttons & 0x01)?1:0;
}


/*//监听物理鼠标中键状态
返回值
-1：还未开启监听功能 需要先调用kmNet_monitor（1）
0 :物理鼠标中键松开
1：物理鼠标中键按下
*/
int kmNet_monitor_mouse_middle()
{
	if (monitor_run != monitor_ok) return -1;
	return (hw_mouse.buttons & 0x04) ? 1 : 0;
}

/*//监听物理鼠标右键状态
返回值
-1：还未开启监听功能 需要先调用kmNet_monitor（1）
0 :物理鼠标右键松开
1：物理鼠标右键按下
*/
int kmNet_monitor_mouse_right()
{
	if (monitor_run != monitor_ok) return -1;
	return (hw_mouse.buttons & 0x02) ? 1 : 0;
}


/*//监听物理鼠标侧键1状态
返回值
-1：还未开启监听功能 需要先调用kmNet_monitor（1）
0 :物理鼠标侧键1松开
1：物理鼠标侧键1按下
*/
int kmNet_monitor_mouse_side1()
{
	if (monitor_run != monitor_ok) return -1;
	return (hw_mouse.buttons & 0x08) ? 1 : 0;
}

/*//监听物理鼠标侧键2状态
返回值
-1：还未开启监听功能 需要先调用kmNet_monitor（1）
0 :物理鼠标侧键2松开
1：物理鼠标侧键2按下
*/
int kmNet_monitor_mouse_side2()
{
	if (monitor_run != monitor_ok) return -1;
	return (hw_mouse.buttons & 0x10) ? 1 : 0;
}



//监听键盘指定按键状态
int kmNet_monitor_keyboard(unsigned char  vk_key)
{
	if (monitor_run != monitor_ok) return -1;
	if (vk_key >= KEY_LEFTCONTROL && vk_key <= KEY_RIGHT_GUI)//控制键
	{
		switch (vk_key)
		{
		case KEY_LEFTCONTROL: return  hw_keyboard.buttons & BIT0 ? 1 : 0;
		case KEY_LEFTSHIFT:   return  hw_keyboard.buttons & BIT1 ? 1 : 0;
		case KEY_LEFTALT:     return  hw_keyboard.buttons & BIT2 ? 1 : 0;
		case KEY_LEFT_GUI:    return  hw_keyboard.buttons & BIT3 ? 1 : 0;
		case KEY_RIGHTCONTROL:return  hw_keyboard.buttons & BIT4 ? 1 : 0;
		case KEY_RIGHTSHIFT:  return  hw_keyboard.buttons & BIT5 ? 1 : 0;
		case KEY_RIGHTALT:    return  hw_keyboard.buttons & BIT6 ? 1 : 0;
		case KEY_RIGHT_GUI:   return  hw_keyboard.buttons & BIT7 ? 1 : 0;
		}
	}
	else//常规键
	{
		for (int i = 0; i < 10; i++)
		{
			if (hw_keyboard.data[i] == vk_key)
			{
				return 1;
			}
		}
	}
	return 0;

}


//开启盒子内部打印信息并发送到指定端口（调试使用）
int kmNet_debug(short port, char enable)
{
	int err;
	if (sockClientfd <= 0)		return err_creat_socket;
	tx.head.indexpts++;					//指令统计值
	tx.head.cmd = cmd_debug;			//指令
	tx.head.rand = port | enable << 16;	// 随机混淆值
	int length = sizeof(cmd_head_t);
	sendto(sockClientfd, (const char*)&tx, length, 0, (struct sockaddr*)&addrSrv, sizeof(addrSrv));
	SOCKADDR_IN sclient;
	int clen = sizeof(sclient);
	err = recvfrom(sockClientfd, (char*)&rx, 1024, 0, (struct sockaddr*)&sclient, &clen);
	if (err < 0)
		return err_net_rx_timeout;
	return NetRxReturnHandle(&rx, &tx);

}

//屏蔽鼠标左键 
int kmNet_mask_mouse_left(int enable)
{
	int err;
	if (sockClientfd <= 0)		return err_creat_socket;
	tx.head.indexpts++;					//指令统计值
	tx.head.cmd = cmd_mask_mouse;		//指令
	tx.head.rand = enable? (mask_keyboard_mouse_flag |= BIT0): (mask_keyboard_mouse_flag &= ~BIT0);	// 屏蔽鼠标左键
	int length = sizeof(cmd_head_t);
	sendto(sockClientfd, (const char*)&tx, length, 0, (struct sockaddr*)&addrSrv, sizeof(addrSrv));
	SOCKADDR_IN sclient;
	int clen = sizeof(sclient);
	err = recvfrom(sockClientfd, (char*)&rx, 1024, 0, (struct sockaddr*)&sclient, &clen);
	if (err < 0)
		return err_net_rx_timeout;
	return NetRxReturnHandle(&rx, &tx);
}

//屏蔽鼠标右键 
int kmNet_mask_mouse_right(int enable)
{
	int err;
	if (sockClientfd <= 0)		return err_creat_socket;
	tx.head.indexpts++;					//指令统计值
	tx.head.cmd = cmd_mask_mouse;		//指令
	tx.head.rand = enable ? (mask_keyboard_mouse_flag |= BIT1) : (mask_keyboard_mouse_flag &= ~BIT1);	// 屏蔽鼠标左键
	int length = sizeof(cmd_head_t);
	sendto(sockClientfd, (const char*)&tx, length, 0, (struct sockaddr*)&addrSrv, sizeof(addrSrv));
	SOCKADDR_IN sclient;
	int clen = sizeof(sclient);
	err = recvfrom(sockClientfd, (char*)&rx, 1024, 0, (struct sockaddr*)&sclient, &clen);
	if (err < 0)
		return err_net_rx_timeout;
	return NetRxReturnHandle(&rx, &tx);
}


//屏蔽鼠标中键 
int kmNet_mask_mouse_middle(int enable)
{
	int err;
	if (sockClientfd <= 0)		return err_creat_socket;
	tx.head.indexpts++;					//指令统计值
	tx.head.cmd = cmd_mask_mouse;		//指令
	tx.head.rand = enable ? (mask_keyboard_mouse_flag |= BIT2) : (mask_keyboard_mouse_flag &= ~BIT2);	// 屏蔽鼠标左键
	int length = sizeof(cmd_head_t);
	sendto(sockClientfd, (const char*)&tx, length, 0, (struct sockaddr*)&addrSrv, sizeof(addrSrv));
	SOCKADDR_IN sclient;
	int clen = sizeof(sclient);
	err = recvfrom(sockClientfd, (char*)&rx, 1024, 0, (struct sockaddr*)&sclient, &clen);
	if (err < 0)
		return err_net_rx_timeout;
	return NetRxReturnHandle(&rx, &tx);
}


//屏蔽鼠标侧键键1 
int kmNet_mask_mouse_side1(int enable)
{
	int err;
	if (sockClientfd <= 0)		return err_creat_socket;
	tx.head.indexpts++;					//指令统计值
	tx.head.cmd = cmd_mask_mouse;		//指令
	tx.head.rand = enable ? (mask_keyboard_mouse_flag |= BIT3) : (mask_keyboard_mouse_flag &= ~BIT3);	// 屏蔽鼠标左键
	int length = sizeof(cmd_head_t);
	sendto(sockClientfd, (const char*)&tx, length, 0, (struct sockaddr*)&addrSrv, sizeof(addrSrv));
	SOCKADDR_IN sclient;
	int clen = sizeof(sclient);
	err = recvfrom(sockClientfd, (char*)&rx, 1024, 0, (struct sockaddr*)&sclient, &clen);
	if (err < 0)
		return err_net_rx_timeout;
	return NetRxReturnHandle(&rx, &tx);
}



//屏蔽鼠标侧键键2
int kmNet_mask_mouse_side2(int enable)
{
	int err;
	if (sockClientfd <= 0)		return err_creat_socket;
	tx.head.indexpts++;					//指令统计值
	tx.head.cmd = cmd_mask_mouse;		//指令
	tx.head.rand = enable ? (mask_keyboard_mouse_flag |= BIT4) : (mask_keyboard_mouse_flag &= ~BIT4);	// 屏蔽鼠标左键
	int length = sizeof(cmd_head_t);
	sendto(sockClientfd, (const char*)&tx, length, 0, (struct sockaddr*)&addrSrv, sizeof(addrSrv));
	SOCKADDR_IN sclient;
	int clen = sizeof(sclient);
	err = recvfrom(sockClientfd, (char*)&rx, 1024, 0, (struct sockaddr*)&sclient, &clen);
	if (err < 0)
		return err_net_rx_timeout;
	return NetRxReturnHandle(&rx, &tx);
}


//屏蔽鼠标X轴坐标
int kmNet_mask_mouse_x(int enable)
{
	int err;
	if (sockClientfd <= 0)		return err_creat_socket;
	tx.head.indexpts++;					//指令统计值
	tx.head.cmd = cmd_mask_mouse;		//指令
	tx.head.rand = enable ? (mask_keyboard_mouse_flag |= BIT5) : (mask_keyboard_mouse_flag &= ~BIT5);	// 屏蔽鼠标左键
	int length = sizeof(cmd_head_t);
	sendto(sockClientfd, (const char*)&tx, length, 0, (struct sockaddr*)&addrSrv, sizeof(addrSrv));
	SOCKADDR_IN sclient;
	int clen = sizeof(sclient);
	err = recvfrom(sockClientfd, (char*)&rx, 1024, 0, (struct sockaddr*)&sclient, &clen);
	if (err < 0)
		return err_net_rx_timeout;
	return NetRxReturnHandle(&rx, &tx);
}


//屏蔽鼠标y轴坐标
int kmNet_mask_mouse_y(int enable)
{
	int err;
	if (sockClientfd <= 0)		return err_creat_socket;
	tx.head.indexpts++;					//指令统计值
	tx.head.cmd = cmd_mask_mouse;		//指令
	tx.head.rand = enable ? (mask_keyboard_mouse_flag |= BIT6) : (mask_keyboard_mouse_flag &= ~BIT6);	// 屏蔽鼠标左键
	int length = sizeof(cmd_head_t);
	sendto(sockClientfd, (const char*)&tx, length, 0, (struct sockaddr*)&addrSrv, sizeof(addrSrv));
	SOCKADDR_IN sclient;
	int clen = sizeof(sclient);
	err = recvfrom(sockClientfd, (char*)&rx, 1024, 0, (struct sockaddr*)&sclient, &clen);
	if (err < 0)
		return err_net_rx_timeout;
	return NetRxReturnHandle(&rx, &tx);
}

//屏蔽鼠标滚轮
int kmNet_mask_mouse_wheel(int enable)
{
	int err;
	if (sockClientfd <= 0)		return err_creat_socket;
	tx.head.indexpts++;					//指令统计值
	tx.head.cmd = cmd_mask_mouse;		//指令
	tx.head.rand = enable ? (mask_keyboard_mouse_flag |= BIT7) : (mask_keyboard_mouse_flag &= ~BIT7);	// 屏蔽鼠标左键
	int length = sizeof(cmd_head_t);
	sendto(sockClientfd, (const char*)&tx, length, 0, (struct sockaddr*)&addrSrv, sizeof(addrSrv));
	SOCKADDR_IN sclient;
	int clen = sizeof(sclient);
	err = recvfrom(sockClientfd, (char*)&rx, 1024, 0, (struct sockaddr*)&sclient, &clen);
	if (err < 0)
		return err_net_rx_timeout;
	return NetRxReturnHandle(&rx, &tx);
}

 
//屏蔽键盘指定按键
int kmNet_mask_keyboard(short vkey)
{
	int err;
	if (sockClientfd <= 0)		return err_creat_socket;
	tx.head.indexpts++;					//指令统计值
	tx.head.cmd = cmd_mask_mouse;		//指令
	tx.head.rand = (mask_keyboard_mouse_flag&0xff)|(vkey<<8);	// 屏蔽键盘vkey
	int length = sizeof(cmd_head_t);
	sendto(sockClientfd, (const char*)&tx, length, 0, (struct sockaddr*)&addrSrv, sizeof(addrSrv));
	SOCKADDR_IN sclient;
	int clen = sizeof(sclient);
	err = recvfrom(sockClientfd, (char*)&rx, 1024, 0, (struct sockaddr*)&sclient, &clen);
	if (err < 0)
		return err_net_rx_timeout;
	return NetRxReturnHandle(&rx, &tx);
}

//解除屏蔽所有已经设置的物理屏蔽
int kmNet_unmask_all()
{
	int err;
	if (sockClientfd <= 0)		return err_creat_socket;
	tx.head.indexpts++;					//指令统计值
	tx.head.cmd = cmd_unmask_all;		//指令
	mask_keyboard_mouse_flag = 0;
	tx.head.rand = mask_keyboard_mouse_flag;
	int length = sizeof(cmd_head_t);
	sendto(sockClientfd, (const char*)&tx, length, 0, (struct sockaddr*)&addrSrv, sizeof(addrSrv));
	SOCKADDR_IN sclient;
	int clen = sizeof(sclient);
	err = recvfrom(sockClientfd, (char*)&rx, 1024, 0, (struct sockaddr*)&sclient, &clen);
	if (err < 0)
		return err_net_rx_timeout;
	return NetRxReturnHandle(&rx, &tx);
}



//设置配置信息  改IP与端口号
int kmNet_setconfig( char *ip,unsigned short port)
{
	int err;
	if (sockClientfd <= 0)		return err_creat_socket;
	tx.head.indexpts++;					//指令统计值
	tx.head.cmd = cmd_setconfig;		//指令
	tx.head.rand = inet_addr(ip); ;
	tx.u8buff.buff[0] = port >> 8;
	tx.u8buff.buff[1] = port >> 0;
	int length = sizeof(cmd_head_t)+2;
	sendto(sockClientfd, (const char*)&tx, length, 0, (struct sockaddr*)&addrSrv, sizeof(addrSrv));
	SOCKADDR_IN sclient;
	int clen = sizeof(sclient);
	err = recvfrom(sockClientfd, (char*)&rx, 1024, 0, (struct sockaddr*)&sclient, &clen);
	if (err < 0)
		return err_net_rx_timeout;
	return NetRxReturnHandle(&rx, &tx);
}


//将整个LCD屏幕用指定颜色填充。 清屏可以用黑色
int kmNet_lcd_color(unsigned short rgb565)
{
	int err;
	if (sockClientfd <= 0)		return err_creat_socket;
	for (int y = 0; y < 40; y++)
	{
		tx.head.indexpts++;		    //指令统计值
		tx.head.cmd = cmd_showpic;	//指令
		tx.head.rand = 0|y*4;		
		memset(tx.u16buff.buff, rgb565, 512);
		int length = sizeof(cmd_head_t) + 1024;
		sendto(sockClientfd, (const char*)&tx, length, 0, (struct sockaddr*)&addrSrv, sizeof(addrSrv));
		SOCKADDR_IN sclient;
		int clen = sizeof(sclient);
		err = recvfrom(sockClientfd, (char*)&rx, length, 0, (struct sockaddr*)&sclient, &clen);
		if (err < 0)
			return err_net_rx_timeout;
	}
	return NetRxReturnHandle(&rx, &tx);

}

//在底部显示一张128x80的图片
int kmNet_lcd_picture_bottom(unsigned char *buff_128_80)
{
	int err;
	if (sockClientfd <= 0)		return err_creat_socket;
	for (int y = 0; y < 20; y++)
	{
		tx.head.indexpts++;		    //指令统计值
		tx.head.cmd = cmd_showpic;	//指令
		tx.head.rand = 80+y*4;
		memcpy(tx.u8buff.buff, &buff_128_80[y*1024], 1024);
		int length = sizeof(cmd_head_t) + 1024;
		sendto(sockClientfd, (const char*)&tx, length, 0, (struct sockaddr*)&addrSrv, sizeof(addrSrv));
		SOCKADDR_IN sclient;
		int clen = sizeof(sclient);
		err = recvfrom(sockClientfd, (char*)&rx, length, 0, (struct sockaddr*)&sclient, &clen);
		if (err < 0)
			return err_net_rx_timeout;
	}
	return NetRxReturnHandle(&rx, &tx);
}

//在底部显示一张128x160的图片
int kmNet_lcd_picture(unsigned char* buff_128_160)
{
	int err;
	if (sockClientfd <= 0)		return err_creat_socket;
	for (int y = 0; y < 40; y++)
	{
		tx.head.indexpts++;		    //指令统计值
		tx.head.cmd = cmd_showpic;	//指令
		tx.head.rand = y * 4;
		memcpy(tx.u8buff.buff, &buff_128_160[y * 1024], 1024);
		int length = sizeof(cmd_head_t) + 1024;
		sendto(sockClientfd, (const char*)&tx, length, 0, (struct sockaddr*)&addrSrv, sizeof(addrSrv));
		SOCKADDR_IN sclient;
		int clen = sizeof(sclient);
		err = recvfrom(sockClientfd, (char*)&rx, length, 0, (struct sockaddr*)&sclient, &clen);
		if (err < 0)
			return err_net_rx_timeout;
	}
	return NetRxReturnHandle(&rx, &tx);
}