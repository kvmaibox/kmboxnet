
import kmNet
import time
#连接盒子
ip='192.168.2.188'
port ='1282'
uuid ='AF425414'
kmNet.init(ip,port,uuid)
print('连接盒子ok')

'''
1万次调用耗时约为10秒，如果鼠标接盒子上则共享上行带宽。动鼠标会占用软件带宽
'''
print('调用速度测试，1万次调用耗时测试')
start_time = time.time()
cnt=10000
while cnt>0:
    kmNet.move(10,10)
    kmNet.move(-10,-10)
    cnt =cnt-2
end_time = time.time()
print('一共耗时 ',(end_time-start_time)*1000,"ms")

#不动鼠标10014ms

