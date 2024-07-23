'''

键盘类api测试

需要将键盘接到kmbox上。
#
keydown ：按键按下
keyup   :按键松开
keypress:单击一次按键

#加密类型
enc_keydown ：按键按下
enc_keyup   :按键松开
enc_keypress:单击一次按键

按键盘q键退出
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


kmNet.mask_keyboard(4)#屏蔽键盘的a键
kmNet.mask_keyboard(5)#屏蔽键盘的b键
kmNet.mask_keyboard(6)#屏蔽键盘的c键
kmNet.mask_keyboard(20)#屏蔽键盘的q键
kmNet.mask_keyboard(224)#屏蔽键盘的左ctrl键


while 1:
    time.sleep(0.01) #每间隔10ms检测一次
    if kmNet.isdown_keyboard(4)==1:
        print('键盘a键按下')
        kmNet.keydown(4)  # a键按下
    else:
        kmNet.keyup(4)    # a键松开

    if kmNet.isdown_keyboard(5)==1:
        print('键盘b键按下')
        kmNet.enc_keydown(5)  # b键按下 加密函数
    else:
        kmNet.enc_keyup(5)    # b键松开 加密函数

    if kmNet.isdown_keyboard(6)==1:
        print("键盘C键按下")
        kmNet.keypress(6,10)   #c键单机一次 10是耗时。本次单击C键用时10ms


    if kmNet.isdown_keyboard(224) == 1:# 224是左ctrl键
        print("键盘左ctrl键按下")
        kmNet.enc_keypress(6, 10)  # c键单机一次 10是耗时。本次单击C键用时10ms

    if kmNet.isdown_keyboard(20) == 1:# 20是Q键
        kmNet.unmask_all()
        print("exit")
        break





