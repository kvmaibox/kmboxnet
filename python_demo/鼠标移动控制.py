'''

按住鼠标左键右下移动
按住鼠标右键左上移动

'''
import kmNet
import time
#连接盒子
ip='192.168.2.188'
port ='1282'
uuid ='AF425414'
kmNet.init(ip,port,uuid)
print('连接盒子ok')
kmNet.monitor(12345)  # 开启物理键鼠监控功能。使用端口号12345接收物理键鼠数据
kmNet.mask_left(1)
kmNet.mask_right(1)
print('按下鼠标左键斜向下移动，按下鼠标右键斜向上移动，按下鼠标中键退出测试')
while 1:
    if kmNet.isdown_left() == 1:#如果鼠标左键按下
        print('left down')
        kmNet.move(1, 1)
    if kmNet.isdown_right() == 1:#如果鼠标右键按下
        print('right down')
        kmNet.move(-1, -1)
    if kmNet.isdown_middle() == 1:#如果鼠标中键按下
        print('exit')
        break
    time.sleep(0.01)  # 10ms移动一次

kmNet.monitor(0)  #关闭监控
kmNet.unmask_all()#解除所有屏蔽