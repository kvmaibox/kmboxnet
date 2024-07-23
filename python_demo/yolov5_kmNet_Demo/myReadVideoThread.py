"""
采用线程方式取视频。永远拿最新的视频贞，避免buff缓存延迟
"""

import cv2
import queue
import threading

# 无缓存读取视频流类
class VideoCapture:
  def __init__(self, name):
    self.cap = cv2.VideoCapture(name)
    self.cap.set(cv2.CAP_PROP_FRAME_WIDTH, 640)
    self.cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 480)
    self.cap.set(cv2.CAP_PROP_FPS, 90)
    self.q = queue.Queue()
    t = threading.Thread(target=self._reader)
    t.daemon = True
    t.start()

  # 帧可用时立即读取帧，只保留最新的帧
  def _reader(self):
    while True:
      ret, frame = self.cap.read()
      if not ret:
        break
      if not self.q.empty():
        try:
          self.q.get_nowait()   # 删除上一个（未处理的）帧
        except queue.Empty:
          pass
      self.q.put(frame)

  def read(self):
    return self.q.get()

def main():
  # 测试代码
  cap = VideoCapture(0)
  while True:
    frame = cap.read()
    cv2.imshow("kmboxNvideo-thread", frame)
    if chr(cv2.waitKey(1) & 255) == 'q':
      break




if __name__ == "__main__":
  main()


