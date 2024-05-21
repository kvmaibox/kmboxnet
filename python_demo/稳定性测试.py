
import kmNet
import time
import keyboard
#连接盒子
ip='192.168.2.188'
port ='1282'
uuid ='AF425414'
kmNet.init(ip,port,uuid)
print('连接盒子ok')

'''
稳定性测试：
软件给盒子发指定按键是否等于软件读取到的值来判断

'''
print(keyboard.is_pressed('a'))
print("鼠标中键退出循环按键检测")
time.sleep(2)
kmNet.monitor(8888)  # 开启物理键鼠监控功能。使用端口号8888接收物理键鼠数据
error=0
last=0
total=0
delay=0.002#1.5ms的系统反应时间。
while 1:
    total += 1
    kmNet.keydown(4)#a键按下
    time.sleep(delay)#此延时必须加，是系统延迟。如果不加此延迟。下面的检测还没有读到a按下的消息。就会报错
    if keyboard.is_pressed('a')==False: #软件检测a键没有按下则错误+1
        error =error+1
        print('a down error')
    total += 1
    kmNet.keydown(6)#c键按下
    time.sleep(delay)
    if keyboard.is_pressed('c')==False: #软件检测a键没有按下则错误+1
        print('c down error')
        error =error+1
    total += 1
    kmNet.keyup(6)#松开C
    time.sleep(delay)
    if keyboard.is_pressed('c'): #软件检测C键按下则错误+1
        print('c up error')
        error =error+1

    total += 1
    kmNet.keyup(4)#松开A
    time.sleep(delay)
    if keyboard.is_pressed('a'): #软件检测a键按下则错误+1
        print('a up error')
        error =error+1

    if error!=last: #如果有新的错误则打印错误次数
        last=error
        print("调用",total,"次，错误",error,"次")
    if kmNet.isdown_middle():
        print("调用",total,"次，错误",error,"次")
        break

