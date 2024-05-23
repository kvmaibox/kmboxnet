
import kmNet
import time
#连接盒子
ip='192.168.2.188'
port ='1282'
uuid ='AF425414'
kmNet.init(ip,port,uuid)
print('连接盒子ok')

#屏蔽鼠标左键
kmNet.mask_left(1)
kmNet.mask_right(1)
kmNet.mask_middle(1)
kmNet.mask_x(1)
kmNet.mask_y(1)
kmNet.monitor(8888)
trace_delay=100#曲线拟合步数
cmd_delay=100/1000    #相邻两条移动指令的间隔
kmNet.trace(2, trace_delay)  #贝塞尔曲线 100ms完成
'''
曲线算法0：
trace_delay <cmd_delay  :正确  
trace_delay >cmd_delay  :不正确，移动过程中来了新的轨迹点， 
曲线算法1：
trace_delay <cmd_delay  :正确  
trace_delay >cmd_delay  :不正确，移动过程中来了新的轨迹点， 
曲线算法2：(能保证准确移动，但是会将所有点逐个执行。如果参数不合理会出现累计延迟。)
trace_delay <cmd_delay  :正确  
trace_delay >cmd_delay  :正确 

'''
while 1:
    if kmNet.isdown_left()==1:#如果鼠标左键按下
        kmNet.move(1920,1080)
        time.sleep(cmd_delay)
        kmNet.move(-1000,-1000)
        time.sleep(cmd_delay)
        kmNet.move(1000,1000)
        break

    time.sleep(0.01)

print("exit")