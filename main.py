import serial
import matplotlib.pyplot as plt
import numpy as np
import math
import matplotlib.animation as animation
from matplotlib.collections import LineCollection
from matplotlib.colors import ListedColormap

ser = serial.Serial()
ser.port = 'COM3'
ser.baudrate = 115200
ser.setDTR(False)
ser.setRTS(False)

ser.open()

attx = []
atty = []
attz = []
rttx = []
rtty = []
rttz = []
figure, axis = plt.subplots(3, 2)
def animate(i, attx, atty, attz, rttx, rtty, rttz):
    b = ser.readline()
    str1 = b.decode('UTF-8')
    lst = str1.split('|')
    if len(lst) > 0:
        att = lst[0].split(',')
        rtt = lst[1].split(',')

        attx.append(float(att[0]))
        atty.append(float(att[1]))
        attz.append(float(att[2]))
        rttx.append(float(rtt[0]))
        rtty.append(float(rtt[1]))
        rttz.append(float(rtt[2]))

#เอาค่าแค่20ตัวนับจากหลังมาหน้า
        attx = attx[-20:]
        atty = atty[-20:]
        attz = attz[-20:]
        rttx = rttx[-20:]
        rtty = rtty[-20:]
        rttz = rttz[-20:]

        # print(att)
        # print(rtt)
        # print(lst[2])



    # ax.clear()

    # For Sine Function
    axis[0, 0].clear()
    axis[0, 0].plot(attx)
    axis[0, 0].set_title("Accelerometer X")

    # For Cosine Function
    axis[1, 0].clear()
    axis[1, 0].plot(atty)
    axis[1, 0].set_title("Accelerometer Y")

    # # For Cosine Function
    axis[2, 0].clear()
    axis[2, 0].plot(attz)
    axis[2, 0].set_title("Accelerometer Z")

    # For Cosine Function
    axis[0, 1].clear()
    axis[0, 1].plot(rttx)
    axis[0, 1].set_title("Rotation X")

    # For Cosine Function
    axis[1, 1].clear()
    axis[1, 1].plot(rtty)
    axis[1, 1].set_title("Rotation y")

    axis[2, 1].clear()
    axis[2, 1].plot(rttz)
    axis[2, 1].set_title("Rotation z")

ani = animation.FuncAnimation(figure, animate, fargs=(attx, atty, attz, rttx, rtty, rttz), interval=10)
plt.show()
# print("Attx: ", attx)
# print("Atty: ", atty)
