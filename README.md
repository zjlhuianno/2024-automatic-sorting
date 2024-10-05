# Openmv识别颜色和形状


---

提示：需要考虑光照，更换场地需要重新调节颜色阈值

@[TOC](文章目录)

---

# 前言

在STM32端，使用mode值来设置openmv的模式：

> mode的值是0，表示停止，不识别；（不使用openmv时，在STM32端将mode值设置为0）
>  mode的值是1，表示同时识别颜色和形状； 
>  mode的值是2，表示识别小球颜色；
> mode的值是3，表示识别二维码；

STM32端使用三个变量接收，openmv发来的数据：

> color的值是1，表示红色；
> color的值是2，表示蓝色；

> shape的值是1，表示圆环； 
> shape的值是2，表示正方形；
>  shape的值是3，表示矩形；

> color_circle的值是1，表示红色；
>  color_circle的值是2，表示蓝色；
>   color_circle的值是3，表示黄色；
> color_circle的值是4，表示白色；

openmv串口（使用USART3）：

> P4端口是TX，接单片机的RX
> P5端口是RX，接单片机的TX

---


# 一、openmv参数调节

## 1.颜色阈值

```python
#设置颜色阈值
#更换场景，如果识别效果不好需要重新调阈值
red1 = (32, 75, 28, 89, 93, -6)
red2 = (70, 19, 66, 23, 68, -35)
blue = (41, 8, -47, 97, -24, -107)
yellow = (38, 89, -22, 25, 78, 29)
white = (33, 84, -34, 61, -72, -2)
```

## 2.更改函数参数，来限制识别区域，提高精度
```python
red_blobs = img.find_blobs([red1], x_stride=10, y_stride=10, pixels_threshold=250)
```

## 3.更改参数density和size的判定范围，提高识别的精度

> 1.density表示目标区域中，像素的密度。正方形和矩形的像素密度相差不大，三角形像素密度较小，调试时根据观察到的值实时修改。
> 2.size表示矩形或者正方形的长和宽的比值。比值大的是矩形，比值小的时正方形。需要注意的时，如果更换方向，矩形的size也会很小，所以要设置两个阈值，一个极大值一个极小值。

```python
if density < 0.65 and density > 0.4:
                print("红色", "圆环", size, density, y.area())
                send_data(uart, 1, 1)
                for c in img.find_circles(threshold = 3500, x_margin = 15, y_margin = 10, r_margin = 10):
                    img.draw_circle(c.x(), c.y(), c.r(), color = (255, 255, 255))
            elif density > 0.65:
                img.draw_rectangle((y[0],y[1],y[2],y[3]),color=(255,255,255))
                if size > 0.9 and size < 1.1:
                   send_data(uart, 1, 2)
                   print("红色", "正方形", size, density, y.area())
                if size > 1.1 or size < 0.7:
                   send_data(uart, 1, 3)
                   print("红色", "矩形", size, density, y.area())
```

# 二、openmv识别方法
## 1. 每次取所识别区域的最大值

```python
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

```

> 根据实际情况，通过调节max_size的值，来提高精度

## 2.识别二维码
```python
#识别二维码
    if mode == 3:
        red_led.off()
        blue_led.off()
        for code in img.find_qrcodes():
            img.draw_rectangle(code.rect(), color = (255, 0, 0))
            print(code.payload())
```

> 直接调用函数识别即可
# 三、openmv与STM32进行通信
## 1. openmv端发送函数

```python
def send_data(uart, color, shape):
    data = bytearray([color & 0xFF, (color >> 8) & 0xFF, shape & 0xFF, (shape >> 8) & 0xFF])
    uart.write(data)  # 发送数据
```

> openmv端配置串口过程略

## 2.STM32端接收

1.变量

```c
uint8_t rxBuffer_main,rx_buffer[4];
uint8_t mode = 0;
uint8_t rx_index = 0;
uint16_t color = 0, shape = 0, color_circle = 0;
```
2.回调函数

```c
// HAL库回调函数
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART1)
    {
        // 处理接收到的数据
				if(mode == 1)
				{
						// 将接收到的数据添加到缓冲区
						rx_buffer[rx_index++] = rxBuffer_main;
						// 检查是否接收到了足够的数据
						if (rx_index == 4) {
							// 解析数据
							color = (rx_buffer[1] << 8) | rx_buffer[0];
							shape = (rx_buffer[3] << 8) | rx_buffer[2];
							// 重置索引，准备下一次接收
							rx_index = 0;
						}
				}
				else if(mode == 2)
				{
						// 将接收到的数据添加到缓冲区
						rx_buffer[rx_index++] = rxBuffer_main;
						// 检查是否接收到了足够的数据
						if (rx_index == 4) {
							// 解析数据
							color_circle = (rx_buffer[1] << 8) | rx_buffer[0];
							shape = (rx_buffer[3] << 8) | rx_buffer[2];
							// 重置索引，准备下一次接收
							rx_index = 0;
						}
				
				}
				else if(mode == 3)
				{
				
				}

        // 重新启动中断接收，准备接收下一批数据
        HAL_UART_Receive_IT(&huart1, &rxBuffer_main, sizeof(rxBuffer_main));
    }
}
```

> 注意：需要现在main.c文件中将串口使能
```c
	extern uint8_t rxBuffer_main;
	// 启动中断接收
   HAL_UART_Receive_IT(&huart1, &rxBuffer_main, sizeof(rxBuffer_main));
```
# 四、openmv与k210的区别

## 1.k210串口配置
```python
import sensor, image, time, lcd
from fpioa_manager import fm
import time
from machine import UART

lcd.init()                          # Init lcd display
lcd.clear(lcd.RED)                  # Clear lcd screen.

# need your connect hardware IO 10/11 to loopback
fm.register(8, fm.fpioa.UART1_TX, force=True)
fm.register(6, fm.fpioa.UART1_RX, force=True)

uart_A = UART(UART.UART1, 115200, 8, None, 1, timeout=1000, read_buf_len=4096)

time.sleep_ms(100) # wait uart ready

sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.skip_frames(time = 2000)
clock = time.clock()

while(True):
    clock.tick()                    # Update the FPS clock
    lcd.display(img)                # Display image on lcd.

uart_A.deinit()
del uart_A

```
## 2.k210屏幕配置

```c
import sensor, image, time, lcd

lcd.init()                          # Init lcd display
lcd.clear(lcd.RED)                  # Clear lcd screen.

while(True):
    clock.tick()                    # Update the FPS clock.
    lcd.display(img)                # Display image on lcd.
    #print(clock.fps())              # Note: CanMV Cam runs about half as fast when connected
                                    # to the IDE. The FPS should increase once disconnected.
```

> 以上代码在k210上面也能使用，但k210识别物体的速度很慢，大家择优使用就行
> 在连接电脑的情况下，k210每秒是跑1帧，但是openmv能跑到40到50帧。

# 总结

> 使用以上方法进行openmv识别并不精确，后期还会修改

> 注意事项有：
> 1.增加缓冲，等待识别精确后再向单片机发送，或者在单片机端增加缓冲
> 2.openmv在不使用时需要将mode设置成0，也就是openmv不识别。只有在使用时才可修改mode的值

