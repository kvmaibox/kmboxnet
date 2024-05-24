'''

屏蔽键鼠操作

注意，只有接盒子上的键盘鼠标才能被屏蔽。

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

#首先屏蔽鼠标所有按键，以及键盘的a键和左ctrl键
kmNet.mask_left(1)#屏蔽物理鼠标左键
kmNet.mask_right(1)#屏蔽物理鼠标右键
kmNet.mask_middle(1)#屏蔽物理鼠标中键
kmNet.mask_side1(1)#屏蔽物理鼠标侧键1键
kmNet.mask_side2(1)#屏蔽物理鼠标侧键2键
kmNet.mask_x(1)#屏蔽物理鼠标x轴移动
kmNet.mask_y(1)#屏蔽物理鼠标y轴移动
kmNet.mask_wheel(1)#屏蔽物理鼠标滚轮
kmNet.mask_keyboard(4)#屏蔽键盘的a键
kmNet.mask_keyboard(224)#屏蔽键盘的左ctrl键
while 1:
    time.sleep(0.1) #每间隔10ms检测一次
    if kmNet.isdown_left()==1:
        print('鼠标左键按下')
        kmNet.mask_left(0)  #解除 屏蔽物理鼠标左键
        kmNet.mask_x(0)     #解除屏蔽物理鼠标x轴移动
        print('解除鼠标x轴移动屏蔽')

    if kmNet.isdown_right()==1:
        print('鼠标右键按下')
        kmNet.mask_right(0)  #解除屏蔽物理鼠标右键
        kmNet.mask_y(0)     #解除屏蔽物理鼠标y轴移动
        print('解除鼠标y轴移动屏蔽')

    if kmNet.isdown_middle()==1:
        print("鼠标中键按下")
        kmNet.mask_middle(0)  #解除屏蔽物理鼠标中键
        kmNet.mask_wheel(0)   #解除屏蔽物理鼠标滚轮

    if kmNet.isdown_side1()==1:
        print("鼠标侧键1按下")
        kmNet.mask_side1(0)  #解除屏蔽物理鼠标侧键1键
        kmNet.unmask_keyboard(4)  #解除屏蔽键盘的a键
        print("解除键盘a键的物理屏蔽")

    if kmNet.isdown_side2() == 1:
        print("鼠标侧键2按下")
        kmNet.mask_side2(0)  #解除屏蔽物理鼠标侧键2键
        kmNet.unmask_keyboard(224)  #解除屏蔽键盘的a键
        print("解除键盘左ctrl键的物理屏蔽")

    if kmNet.isdown_keyboard(4) == 1:# 4是a键
        print("键盘a键按下")

    if kmNet.isdown_keyboard(224) == 1:# 224是左ctrl键
        print("键盘左ctrl键按下")





