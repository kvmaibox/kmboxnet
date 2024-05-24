'''

鼠标操作api测试  enc类型函数测试

注意，只有接盒子上的键盘鼠标才能被屏蔽。
enc_move
enc_left
enc_right
enc_middle
enc_wheel

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
kmNet.mask_wheel(1)#屏蔽物理鼠标滚轮5
while 1:
    time.sleep(0.001) #每间隔10ms检测一次
    if kmNet.isdown_left()==1:
        print('鼠标左键按下')
        kmNet.enc_left(1)   #加密类型数据包
        kmNet.mask_x(0)     #解除屏蔽物理鼠标x轴移动
        print('解除鼠标x轴移动屏蔽')
    else:
        kmNet.enc_left(0)

    if kmNet.isdown_right()==1:
        print('鼠标右键按下')
        kmNet.enc_right(1)
        kmNet.mask_y(0)     #解除屏蔽物理鼠标y轴移动
        print('解除鼠标y轴移动屏蔽')
    else:
        kmNet.enc_right(0)

    if kmNet.isdown_middle()==1:
        print("鼠标中键按下")
        kmNet.enc_middle(1)
        kmNet.mask_wheel(0)   #解除屏3 蔽物理鼠标滚轮
    else:
        kmNet.enc_middle(0)

    if kmNet.isdown_side1()==1:
        print("鼠标侧键1按下")
        kmNet.enc_move(10,10)
        kmNet.mask_wheel(0)#解除滚轮移动


    if kmNet.isdown_side2() == 1:
        print("鼠标侧键2按下")
        kmNet.enc_wheel(10)
        break

kmNet.unmask_all()
print('exit')








