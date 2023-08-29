// kmNetLib.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include "NetConfig/kmboxNet.h"
#include "NetConfig/HidTable.h"
#include "picture.h"



int main()
{
	//连接测试  必须连接正常才能操作其他API 
	//kmNet_init((char*)"192.168.2.188", (char*)"12545", (char*)"F101383B"); //连接盒子
	kmNet_init((char*)"192.168.2.188", (char*)"512", (char*)"C362383B"); //连接盒子
	kmNet_lcd_picture_bottom(gImage_128x80); //下半部分显示吃鸡图片

#if 0
	long startime = GetTickCount();
	kmNet_lcd_color(0);		    //显示黑色
	printf("\t整屏刷新 =%ld ms\r\n", GetTickCount() - startime);
	startime = GetTickCount();
	kmNet_lcd_picture_bottom(gImage_128x80); //下半部分显示128x80图片
	printf("\t半屏刷新 =%ld ms\r\n", GetTickCount() - startime);
	//kmNet_lcd_picture();		  //整屏显示128x160图片
#endif 


#if 0
	kmNet_init((char*)"192.168.2.166", (char*)"1234", (char*)"F101383B"); //连接盒子
	printf("修改盒子IP地址\r\n");
	kmNet_setconfig((char*)"192.168.2.166", 1234);
	printf("修改后需要重启才能生效\r\n");
	kmNet_reboot();
#endif

#if 0    //监听物理键鼠功能测试
	kmNet_monitor(1); //开启键鼠监控功能
	for (;;)
	{
		//鼠标按键检测
		if (kmNet_monitor_mouse_left() == 1) //鼠标左键按下了
		{
			printf("鼠标左键按下\r\n");
			do { Sleep(1); } while (kmNet_monitor_mouse_left() == 1); //等待左键松开
			printf("鼠标左键松开\r\n");
		}

		if (kmNet_monitor_mouse_middle() == 1) //鼠标中键按下了
		{
			printf("鼠标中键按下\r\n");
			do { Sleep(1); } while (kmNet_monitor_mouse_middle() == 1); //等待左键松开
			printf("鼠标中键松开\r\n");
		}


		if (kmNet_monitor_mouse_right() == 1) //鼠标中键按下了
		{
			printf("鼠标右键按下\r\n");
			do { Sleep(1); } while (kmNet_monitor_mouse_right() == 1); //等待左键松开
			printf("鼠标右键松开\r\n");
		}

		if (kmNet_monitor_mouse_side1() == 1) //鼠标中键按下了
		{
			printf("鼠标侧键1按下\r\n");
			do { Sleep(1); } while (kmNet_monitor_mouse_side1() == 1); //等待左键松开
			printf("鼠标侧键1松开\r\n");
		}

		if (kmNet_monitor_mouse_side2() == 1) //鼠标中键按下了
		{
			printf("鼠标侧键2按下\r\n");
			do { Sleep(1); } while (kmNet_monitor_mouse_side2() == 1); //等待左键松开
			printf("鼠标侧键2松开\r\n");
		}

		if (kmNet_monitor_keyboard(KEY_A) == 1) //鼠标中键按下了
		{
			printf("键盘A键按下\r\n");
			do { Sleep(1); } while (kmNet_monitor_keyboard(KEY_A) == 1); //等待左键松开
			printf("键盘A键松开\r\n");
}

	}

#endif 



#if 0  //模拟人工轨迹测试
	kmNet_monitor(1); //开启键鼠监控功能
	for (;;)
	{	/*打开画图工具按下鼠标左键看移动轨迹*/
		if (kmNet_monitor_mouse_left()) //如果鼠标左键按下
		{
			long startime = GetTickCount();
			int ret = kmNet_mouse_move_auto(400, 300, 200);						//预设200ms内完成
			printf("\t耗时=%ld ms ret=%d\r\n", GetTickCount() - startime, ret);	//实际耗时
			while (kmNet_monitor_mouse_left()) Sleep(1);						//等待左键松开
		}
		Sleep(1);//主动休眠避免CPU占用过高
	}
#endif 
	
#if 0 //屏蔽鼠标测试
	printf("物理键鼠屏蔽测试：\r\n");
	kmNet_monitor(1);			//开启键鼠监控功能
	kmNet_mask_mouse_left(1);	//屏蔽鼠标左键
	kmNet_mask_mouse_middle(1);	//屏蔽鼠标中键
	kmNet_mask_mouse_right(1);	//屏蔽鼠标右键
	kmNet_mask_mouse_side1(1);	//屏蔽鼠标侧键1
	kmNet_mask_mouse_side2(1);	//屏蔽鼠标侧键2
	kmNet_mask_mouse_x(1);		//屏蔽鼠标x方向
	kmNet_mask_mouse_y(1);		//屏蔽鼠标y方向  --开启以上代码后鼠标基本上就没啥用了
	kmNet_mask_keyboard(KEY_A);	//屏蔽键盘A键
	int timeout = 1000;
	for (;;)// 10秒后退出
	{
		if (kmNet_monitor_mouse_left()) {				//如果物理鼠标左键按下
			printf("物理鼠标左键按下\r\n");
			while (kmNet_monitor_mouse_left()) Sleep(1);//等待左键松开
		}
		if (kmNet_monitor_mouse_middle()) {				  //如果物理鼠标中键按下
			printf("物理鼠标中键按下\r\n");
			while (kmNet_monitor_mouse_middle()) Sleep(1);//等待中键松开
		}
		if (kmNet_monitor_mouse_right()) {				  //如果物理鼠标右键按下
			printf("物理鼠标右键按下\r\n");
			while (kmNet_monitor_mouse_right()) Sleep(1);//等待右键松开
		}
		if (kmNet_monitor_mouse_side1()) {				  //如果物理鼠标侧键1按下
			printf("物理鼠标右键按下\r\n");
			while (kmNet_monitor_mouse_side1()) Sleep(1);//等待侧键1松开
		}

		if (kmNet_monitor_mouse_side2()) {				  //如果物理鼠标侧键2按下
			printf("物理鼠标右键按下\r\n");
			while (kmNet_monitor_mouse_side2()) Sleep(1);//等待侧键2松开
		}

		if (kmNet_monitor_keyboard(KEY_A))
		{
			printf("键盘A键按下\r\n");
			while (kmNet_monitor_keyboard(KEY_A)) Sleep(1);//等待侧键2松开
			printf("键盘A键松开\r\n");
		}
		Sleep(1);//主动休眠避免CPU占用过高
		if (timeout == 0) break;
		timeout--;

	}
	printf("解除所有屏蔽\r\n");
	kmNet_unmask_all();//解除所有屏蔽。此时键盘鼠标都能正常使用
#endif 


#if 0
	 kmNet_mouse_left(0);			//松开
	 kmNet_mouse_all(1,0,0,0);		//按下
	 kmNet_mouse_all(0, 0, 0, 0);	//松开
	 kmNet_mouse_right(1);			//按下
	//键盘按键测试
	 kmNet_keydown(4);//a按下
	 kmNet_reboot();  //重启盒子
	 kmNet_mouse_right(0);			//松开
	 kmNet_mouse_all(2, 0, 0, 0);	//按下
	 kmNet_mouse_all(0, 0, 0, 0);	//松开
	 //鼠标中键测试
	 kmNet_mouse_middle(1);			//按下
	 kmNet_mouse_middle(0);			//松开
	 kmNet_mouse_all(4, 0, 0, 0);	//按下
	 kmNet_mouse_all(0, 0, 0, 0);	//松开

#endif 



#if 1
	printf("调用速度测试,请等待约10s左右...\r\n");
	int cnt = 10000;
	int ret;
	long startime = GetTickCount();
	while (cnt>0)
	{
		ret = kmNet_mouse_move(0, -100); cnt--;
		if (ret)
			printf("tx error %d  ret0=%d\r\n", cnt,ret);
		ret = kmNet_mouse_move(0, 100);  cnt--;
		if(ret)
			printf("tx error %d  ret1=%d\r\n", cnt, ret);
	}
    printf("\t10000次调用耗时=%ld ms\r\n", GetTickCount() - startime);
#endif 


}
