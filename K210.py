import sensor, image, time, lcd
from machine import UART
import struct


lcd.init()                          # Init lcd display
lcd.clear(lcd.RED)                  # Clear lcd screen.

sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.skip_frames(time = 2000)
clock = time.clock()


#设置颜色阈值
#更换场景，如果识别效果不好需要重新调阈值
red1 = (32, 75, 28, 89, 93, -6)
red2 = (70, 19, 66, 23, 68, -35)
blue = (41, 8, -47, 97, -24, -107)
yellow = (38, 89, -22, 25, 78, 29)
white = (33, 84, -34, 61, -72, -2)

#识别形状，识别颜色，同时还要控制图像在屏幕的中心位置。
#将数据通过串口返回到单片机上面.


#寻找图像中的最大色块
#高度增加无法识别可以减小max_size
def find_max(blobs):
    max_blob = None
    max_size = 200
    for blob in blobs:
        if blob[2]*blob[3]>max_size:#blob[2]*blob[3]，计算包含色块的最小矩形面积大小
            max_blob=blob
            max_size=blob[2]*blob[3]
    return max_blob


#默认情况下不进行识别
mode = 1

while(True):
    clock.tick()
    img = sensor.snapshot()
    img.bilateral(3, color_sigma=0.1, space_sigma=1)

    #判断正方形和矩形时，如果倾斜角度较小，小于15度能正常识别，倾斜角度大于15度，识别错误
    if mode == 1:
        #blue_blobs = img.find_blobs([blue],x_stride=10, y_stride=10, pixels_threshold=50)
        red_blobs = img.find_blobs([red1], x_stride=10, y_stride=10, pixels_threshold=250)
        if red_blobs:
            y = find_max(red_blobs)
            img.draw_cross(y[5], y[6],size=2,color=(255,255,255))
            img.draw_string(y[0], (y[1]-10), "red", color=(255,255,0))
            density = y.density()
            width = y.w()
            height = y.h()
            size = height/width
            if density < 0.65 and density > 0.4:
                print("红色", "圆环", size, density, y.area())
                for c in img.find_circles(threshold = 3500, x_margin = 15, y_margin = 10, r_margin = 10):
                    img.draw_circle(c.x(), c.y(), c.r(), color = (255, 255, 255))
            elif density > 0.65:
                img.draw_rectangle((y[0],y[1],y[2],y[3]),color=(255,255,255))
                if size > 0.9 and size < 1.1:
                   print("红色", "正方形", size, density, y.area())
                if size > 1.1 or size < 0.7:
                   print("红色", "矩形", size, density, y.area())
        else:
            blue_blobs = img.find_blobs([blue], x_stride=10, y_stride=10, pixels_threshold=250 )
            if blue_blobs:
                #for r in blue_blobs:
                r = find_max(blue_blobs)
                img.draw_cross(r[5], r[6],size=2,color=(255,255,255))
                img.draw_string(r[0], (r[1]-10), "blue", color=(255,255,0))
                density = r.density()
                width = r.w()
                height = r.h()
                size = height/width
                if density < 0.6 and density > 0.4:
                    print("蓝色", "圆环", size, density)
                    for c in img.find_circles(threshold = 3500, x_margin = 20, y_margin = 15, r_margin = 10):
                        img.draw_circle(c.x(), c.y(), c.r(), color = (255, 255, 255))
                elif density > 0.6:
                    img.draw_rectangle((r[0],r[1],r[2],r[3]),color=(255,255,255))
                    if size > 0.9 and size < 1.1:
                       print("蓝色", "正方形", size, density)
                    if size > 1.1 or size < 0.7:
                       print("蓝色", "矩形", size)

    #识别小球颜色
    if mode == 2:
        red_blobs = img.find_blobs([red1], x_stride=10, y_stride=10, pixels_threshold=250)
        blue_blobs = img.find_blobs([blue], x_stride=10, y_stride=10, pixels_threshold=250 )
        yellow_blobs = img.find_blobs([yellow], x_stride=10, y_stride=10, pixels_threshold=250 )
        white_blobs = img.find_blobs([white], x_stride=10, y_stride=10, pixels_threshold=250 )
        if red_blobs:
            y = find_max(red_blobs)
            img.draw_rectangle((y[0],y[1],y[2],y[3]),color=(255,255,255))
            print("红色")
        elif blue_blobs:
            y = find_max(blue_blobs)
            img.draw_rectangle((y[0],y[1],y[2],y[3]),color=(255,255,255))
            print("蓝色")
        elif yellow_blobs:
            y = find_max(yellow_blobs)
            img.draw_rectangle((y[0],y[1],y[2],y[3]),color=(255,255,255))
            print("黄色")
        elif white_blobs:
            y = find_max(white_blobs)
            img.draw_rectangle((y[0],y[1],y[2],y[3]),color=(255,255,255))
            print("白色")

    #识别二维码
    if mode == 3:
        for code in img.find_qrcodes():
            img.draw_rectangle(code.rect(), color = (255, 0, 0))
            print(code.payload())

    lcd.display(img)                # Display image on lcd.