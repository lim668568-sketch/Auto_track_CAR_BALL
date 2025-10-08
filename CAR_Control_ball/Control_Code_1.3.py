# Untitled - By: 逆之苇 - Sun Sep 28 2025

import sensor
import time
from pid import PID
from pyb import UART, LED
from image import SEARCH_EX
import image
import ustruct

# 需要参数
K = 500  # 需要试验修改
#templates = ["3.pgm", "4.pgm", "5.pgm", "6.pgm"]  # 保存多个模板
length = 20  # 目标距离对应的数字
#Key = 0     # 控制指令
#last_key = 0  # 记录上一次的Key值
last_length = 0

# 目标物识别阈值
#test_threshold = (14, 100, -65, -8, 19, 127)
test_threshold =(45, 96, -25, -7, -7, 67)
size_threshold = 2000  # 最近距离设置

# PID控制器初始化
x_pid = PID(p=0.5, i=1, imax=100)  # 电机转向
h_pid = PID(p=0.05, i=0.1, imax=50)  # 小车速度

# 提示小灯
LED(1).on()

# 摄像头初始化（只做一次）
sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QQVGA)  # 320*240
sensor.skip_frames(time=2000)
sensor.set_auto_gain(False)    # 自动增益关闭
sensor.set_auto_whitebal(False)
sensor.set_vflip(True)         # 垂直翻转
sensor.set_hmirror(True)       # 水平镜像

# 串口通信初始化
uart = UART(3, 115200)
uart.init(115200, bits=8, parity=None, stop=1)

clock = time.clock()

# 最大色块寻找
def max_find(blobs):
    max_size = 0
    max_blob = None
    for blob in blobs:
        if blob[2] * blob[3] > max_size:
            max_blob = blob
            max_size = blob[2] * blob[3]
    return max_blob

def sending_data(x_output, h_output, length_data):
    """发送数据函数：同时发送转向、速度和距离数据"""
    # 限制输出范围在-127到127之间（适合8位串口传输）
    x_output = x_output+10000
    h_output = h_output+10000


    # # 转换方向和值：高 bit 表示方向(0为正，1为负)，低7 bit 表示绝对值
    # x_dir = 0x80 if x_output < 0 else 0x00
    # x_value = abs(x_output) | x_dir

    # h_dir = 0x80 if h_output < 0 else 0x00
    # h_value = abs(h_output) | h_dir

    # 打包数据，符合单片机接收格式
    # 帧结构: 0x35(帧头1)、0x20(帧头2)、x_output、h_output、length、0x55(帧尾)
    data = ustruct.pack("<BBIIBB",
                   0x35,       # 帧头1
                   0x20,       # 帧头2
                   x_output,   # 转向输出
                   h_output,   # 速度输出
                   length_data,# 距离数据
                   0x55)       # 帧尾
    uart.write(data)
    #print(data)

def constrain(value, min_val, max_val):
    """限制值在指定范围内"""
    return min(max_val, max(min_val, value))

def UartReceiveData():
    global Key
    if uart.any():  # 确保有足够的数据
        data = uart.read(3)
        if data[0] == 0xAA and data[2] == 0x55:  # 校验帧头帧尾
            Key = data[1]
            print("Key updated:", Key)


LED(1).off()

# ======================= 主循环 =======================
while True:
    clock.tick()
    #UartReceiveData()  # 处理串口数据
    img = sensor.snapshot()
    # # 模式切换时才修改相机参数
    # if Key != last_key:
    #     if Key == 2:  # 数字识别模式
    #         sensor.set_pixformat(sensor.GRAYSCALE)
    #         sensor.set_framesize(sensor.QQVGA)
    #         sensor.skip_frames(time=5)  # 短暂等待
    #     elif Key == 3:  # 物体跟踪模式
    #         sensor.set_pixformat(sensor.RGB565)
    #         sensor.set_framesize(sensor.QVGA)
    #         sensor.skip_frames(time=5)  # 短暂等待
    #     last_key = Key  # 更新上次Key值

    # if Key == 1:  # 停止模式
    #     sending_data(0, 0,length)

    # elif Key == 2:  # 数字识别模式
    #     img = sensor.snapshot()
    #     for t in templates:
    #         try:
    #             template = image.Image(t)
    #             r = img.find_template(template, 0.70, step=4, search=SEARCH_EX)
    #             if r:
    #                 img.draw_rectangle(r)
    #                 print(f"识别到: {t}")
    #                 # 更新距离
    #                 if t == "3.pgm":
    #                     length = 30
    #                 elif t == "4.pgm":
    #                     length = 40
    #                 elif t == "5.pgm":
    #                     length = 50
    #                 elif t == "6.pgm":
    #                     length = 60
    #                 break  # 识别到一个即退出
    #         except Exception as e:
    #             print(f"模板匹配错误: {e}")

    #     # 仅当识别到新距离时才发送（避免重复发送）
    #     if length != last_length:
    #         last_length = length
    #         # 发送6字节帧：数据1/2填0（无效），数据3填距离
    #     sending_data(0, 0, length)
    #     print("当前目标距离:", length)

    #elif Key == 3:  # 物体跟踪模式
    #img = sensor.snapshot()
    blobs = img.find_blobs([test_threshold])

    if blobs and length > 0:  # 确保有目标且已设置长度
                max_blob = max_find(blobs)
        # if max_blob:
                # 计算目标尺寸
                Lm = (max_blob[2] + max_blob[3]) / 2  # 直径像素
                target_Lm = K / length if length != 0 else Lm  # 目标直径像素
                target_area = max(target_Lm * target_Lm * 3.1415926, size_threshold)
                # current_length=abs(K/Lm)

                # PID计算
                x_error = (int)(max_blob[5] - img.width() / 2)  # 水平偏差
                h_error = max_blob[2] * max_blob[3]* 3.1415926 - target_area  # 面积偏差
                # print(f"target_Lm: {target_Lm}, Lm: {Lm}")
                # print(f"x_error: {x_error}, h_error: {h_error}")

                # 绘制目标
                img.draw_rectangle(max_blob[0:4])
                img.draw_cross(max_blob[5], max_blob[6])

                # 计算输出并发送
                #x_output = (int)(x_pid.get_pid(x_error, 1))
                h_output = (int)(h_pid.get_pid(h_error, 1))
                sending_data(x_error, h_output,0)
                print(f"x_error: {x_error}, h_output: {h_output}")
    else:
            # 未识别到目标或未设置长度时停止
            #sending_data(0, 0)
            # 可选：添加原地旋转寻找目标的逻辑
             sending_data(0, 0,0)  # 小幅度旋转

    #time.sleep_ms(10)  # 微小延迟，稳定系统

