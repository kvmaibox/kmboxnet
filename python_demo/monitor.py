'''

监控盒子上的键盘鼠标

注意，只有接盒子上的键盘鼠标才能被监控。

'''
import kmNet
import time
#连接盒子
ip='192.168.2.188'
port ='1282'
uuid ='AF425414'
kmNet.init(ip,port,uuid)
print('连接盒子ok')
kmNet.monitor(5001) #开启键鼠监控
while 1:
    time.sleep(0.1) #每间隔10ms检测一次
    if kmNet.isdown_left()==1:
        print('鼠标左键按下')

    if kmNet.isdown_right()==1:
        print('鼠标右键按下')

    if kmNet.isdown_middle()==1:
        print("鼠标中键按下")

    if kmNet.isdown_side1()==1:
        print("鼠标侧键1按下")

    if kmNet.isdown_side2() == 1:
        print("鼠标侧键2按下")

    if kmNet.isdown_keyboard(4) == 1:# 4是a键
        print("键盘a键按下")

    if kmNet.isdown_keyboard(224) == 1:# 224是左ctrl键
        print("键盘左ctrl键按下")





