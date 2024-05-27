'''

控制屏幕显示图片

全屏显示：lcd_picture            图片大小必须是128x160
半屏显示：lcd_picture_bottom     图片大小必须是128x80
'''

import cv2
import kmNet
import time
# 获得视频的格式

print('这是一个播放视频的demo，会自动将exe目录下的test.mp4文件发送到屏幕上播放\r\n')



ip='192.168.2.188'
port ='1282'
uuid ='AF425414'

type=input("请输入显示模式,并按回车，0：全屏显示 ,1：半屏显示 ：")



videoCapture = cv2.VideoCapture('test.mp4')
# 获得码率及尺寸
fps = videoCapture.get(cv2.CAP_PROP_FPS)
size = (int(videoCapture.get(cv2.CAP_PROP_FRAME_WIDTH)),int(videoCapture.get(cv2.CAP_PROP_FRAME_HEIGHT)))
fNUMS = videoCapture.get(cv2.CAP_PROP_FRAME_COUNT)

kmNet.init(ip,port,uuid)


print(type)
# 读帧
success, frame = videoCapture.read()
if type=='0':

    while success:
        cv2.imshow('windows', frame)  # 显示
        cv2.waitKey(1)  # 延迟
        success, frame = videoCapture.read()  # 获取下一帧
        pic = cv2.resize(frame,(128,160))#全屏显示必须配置分辨率是128x160
        pic = pic.flatten()     #格式化
        kmNet.lcd_picture(pic)  #给盒子屏幕显示

elif type=='1':
    while success:
        cv2.imshow('windows', frame)  # 显示
        cv2.waitKey(1)  # 延迟
        success, frame = videoCapture.read()  # 获取下一帧
        pic = cv2.resize(frame,(128,80))#半屏显示必须配置分辨率是128x80
        pic = pic.flatten()                   #格式化
        start_time = time.time()
        kmNet.lcd_picture_bottom(pic)         #给盒子屏幕显示
        end_time = time.time()
        print('一共耗时 ', (end_time - start_time) * 1000, "ms")

videoCapture.release()




