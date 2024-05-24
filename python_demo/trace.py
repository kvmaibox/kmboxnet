
import kmNet
import time
#连接盒子
ip='192.168.2.188'
port ='1282'
uuid ='AF425414'
kmNet.init(ip,port,uuid)
print('连接盒子ok')

#屏蔽鼠标
kmNet.mask_left(1)
kmNet.mask_x(1)
kmNet.mask_y(1)
kmNet.monitor(8888)
trace_delay=80      #曲线拟合步数
cmd_delay=20/1000    #相邻两条移动指令的间隔
kmNet.trace(3, trace_delay)  #贝塞尔曲线 100ms完成
'''
曲线算法0：+OK
kmNet.trace(0, trace_delay)  #贝塞尔曲线-非实时 
trace_delay <cmd_delay  :正确  
trace_delay >cmd_delay  :正确 
trace_delay =cmd_delay  :正确 

曲线算法1：+ok
kmNet.trace(1, trace_delay)  #追踪算法-非实时 
trace_delay <cmd_delay  :正确  
trace_delay >cmd_delay  :正确
trace_delay =cmd_delay  :正确 

曲线算法2：+OK
kmNet.trace(2, trace_delay)  #贝塞尔曲线-实时
trace_delay <cmd_delay  :正确  
trace_delay >cmd_delay  :正确 
trace_delay =cmd_delay  :正确 

曲线算法3：+OK
kmNet.trace(2, trace_delay)  #追踪算法-实时
trace_delay <cmd_delay  :正确  
trace_delay >cmd_delay  :正确 
trace_delay =cmd_delay  :正确 

'''
while 1:
    if kmNet.isdown_left()==1:#如果鼠标左键按下
        kmNet.move(1920,1080)
        time.sleep(cmd_delay)
        kmNet.move(-1000,-1000)
        time.sleep(cmd_delay)
        kmNet.move(1000,1000)
        time.sleep(cmd_delay)
        break

    time.sleep(0.01)

kmNet.mask_left(0)
kmNet.mask_x(0)
kmNet.mask_y(0)
print("exit")