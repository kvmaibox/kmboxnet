"""
按住鼠标侧键1 自动瞄准距离屏幕中间最近的目标
"""
import math
import kmNet  #导入kmnet模块
from myReadVideoThread import VideoCapture #以线程的方式读取采集卡数据。让推理图像永远是最新的。避免累积延迟
import sys
from pathlib import Path
import cv2
import torch
import time
video_num =0  #采集卡编号为0 ，1，2...

FILE = Path(__file__).absolute()
sys.path.append(FILE.parents[0].as_posix())  # add yolov5/ to path
from models.experimental import attempt_load
from utils.general import is_ascii, non_max_suppression,scale_boxes, set_logging
from utils.plots import Annotator, colors
from utils.torch_utils import select_device


'''
偏转角度换算为鼠标坐标
'''
def fov(ax,ay,px,py,Cx,Cy):
    dx=(ax-px/2)*3
    dy=(ay-py/2)*2.25
    Rx=Cx/2/math.pi
    Ry=Cy/2/math.pi
    mx =math.atan2(dx,Rx)*Rx
    my =(math.atan2(dy,math.sqrt(dx * dx + Rx * Rx)))*Ry
    return mx,my



def show_relsul_to_lcd(x,y,ex,ey,frame):
#  居中显示瞄准的目标
    pic = cv2.resize(frame, (128, 160))  # 全屏显示必须配置分辨率是128x160
    pic = pic.flatten()     # 格式化
    kmNet.lcd_picture(pic)  # 给盒子屏幕显示

@torch.no_grad()
def run(weights='yolov5s.pt',  # model.pt path(s)
        conf_thres=0.25,  # confidence threshold
        iou_thres=0.45,  # NMS IOU threshold
        max_det=1000,  # maximum detections per image
        device='',  # cuda device, i.e. 0 or 0,1,2,3 or cpu
        classes=0,  # filter by class: --class 0, or --class 0 2 3
        agnostic_nms=False,  # class-agnostic NMS
        line_thickness=1,  # bounding box thickness (pixels)
        half=False,  # use FP16 half-precision inference
        ):
    # Initialize
    set_logging()
    device = select_device(device)
    print(device)
    half &= device.type != 'cpu'  # half precision only supported on CUDA

    model = attempt_load(weights, device=device)  # load FP32 model
    names = model.module.names if hasattr(model, 'module') else model.names  # get class names
    ascii = is_ascii(names)  # names are ascii (use PIL for UTF-8)
    cap = VideoCapture(video_num)#开始采集图像

    global xyxy        #单一目标
    global time_aim    #上一次自瞄的时间
    aim_process=0
    while True:
        # 获取一帧
        timestamp_t0 = time.time()
        frame = cap.read()#获取最新一帧图像
        img = torch.from_numpy(frame).to(device)
        img = img.half() if half else img.float()  # uint8 to fp16/32
        img = img / 255.0  # 0 - 255 to 0.0 - 1.0
        if len(img.shape) == 3:
            img = img[None]  # expand for batch dim
        img = img.transpose(2, 3)
        img = img.transpose(1, 2)
        timestamp_t1 = time.time()
        # Inference
        pred = model(img, augment=False, visualize=False)[0]
        timestamp_t2 = time.time()
        # NMS
        pred = non_max_suppression(pred, conf_thres, iou_thres, classes, agnostic_nms, max_det=max_det)
        timestamp_t3 = time.time()

        # Process predictions 处理预测结果
        for i, det in enumerate(pred):  # detections per image
            s = ''
            result = ''
            annotator = Annotator(frame, line_width=line_thickness, pil=not ascii)
            if len(det):    #如果预测框的数量大于0
                #将预测框的坐标从归一化坐标转换为原始坐标,im.shape[2:]为图片的宽和高，det[:, :4]为预测框的坐标， frame.shape为图片的宽和高
                det[:, :4] = scale_boxes(img.shape[2:], det[:, :4], frame.shape).round()
                # Print results
                for c in det[:, -1].unique():
                    n = (det[:, -1] == c).sum()  # detections per class
                    s += str(n.item()) + ' ' + str(names[int(c)]) + ' '  # add to string
                #print('CLASS:' + s)

                # Write results
                for *xyxy, conf, cls in reversed(det):
                    c = int(cls)  # integer class
                    label = f'{names[c]} {conf:.2f}'
                    annotator.box_label(xyxy, label, color=colors(c, True))
                    #print("\t"+names[int(c)],"(",int(xyxy[0]),int(xyxy[1]),int(xyxy[2]),int(xyxy[3]),")")
        timestamp_t4 = time.time()

        if kmNet.isdown_side1()==1 and aim_process==0:#如果侧键按下了表示此时需要自瞄 aim_process控制自瞄间隔
            '''
            这里加入坐标转换算法。鼠标移动算法。过滤算法，跟踪算法等
            本demo为简单起见，假设屏幕上只有一个目标。控制鼠标移动到目标高度80%的点（头部）。 如有多个目标请自行过滤。
            '''
            #step1:计算比例尺 输入图像分辨率是1920x1080 采集输出分辨率是640x480
            sx=int(xyxy[0])#真实物理屏幕x起始位置
            sy=int(xyxy[1])#真实物理屏幕y起始位置
            ex=int(xyxy[2])#真实物理屏幕x结束位置
            ey=int(xyxy[3])#真实物理屏幕y结束位置
            #step2:以屏幕正中心为准星。计算鼠标移动到矩形框80%处需要的偏移量
            dx=sx+(ex-sx)/2# 矩形框x的中点
            dy=sy+(ey-sy)*0.2#矩形框从上往下高度的20%处
            mx,my=fov(dx,dy,640,480,5140,5140)
            print("\t鼠标移动坐标",mx,my)
            kmNet.move(int(mx),int(my))#这只是演示。别这样写。一帧锁死肯定会键鼠数据异常的。自己加入轨迹算法。
            aim_process=1
            time_aim=time.time()
            # 显示命中的目标 ----下面可以屏蔽，加快自瞄速度。显示是最浪费时间的操作
            pic = cv2.resize(frame, (128, 160))  # 全屏显示必须配置分辨率是128x160
            pic = pic.flatten()     # 格式化
            kmNet.lcd_picture(pic)  # 给盒子屏幕显示


        if aim_process==1 and kmNet.isdown_side1()==1:#自瞄键按下还没松开，不要切换目标
            if (time.time()-time_aim)*1000>=100:
                aim_process=0

        if aim_process==1 and kmNet.isdown_side1()==0:#自瞄键松开了，复位状态机。等待下一次自瞄按下
            aim_process=0
            pass




        cv2.imshow('frame', frame)
        timestamp_t5 = time.time()
        cv2.waitKey(0)



def main():
    kmNet.init("192.168.2.188","32770","8147E04E") #连接盒子---连不上先检查网络是否通畅
    kmNet.monitor(6666) #打开物理键鼠监控功能 监听端口号6666---监听端口可任意，不与系统其他端口冲突即可
    kmNet.mask_side1(1) #屏蔽鼠标侧键1 ---侧键按下消息不会发送到游戏机。但AI可以检测到侧键按下。用作开启辅瞄开关
    kmNet.trace(0,80)   #采用硬件曲线修正算法。80ms内完成自瞄移动
    run()#开始采集推导


if __name__ == "__main__":
    main()

