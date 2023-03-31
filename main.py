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

# Create two separate figure objects and their respective axes
fig1, axes1 = plt.subplots(3, 1)
fig2, axes2 = plt.subplots(3, 1)

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

        # Keep only the last 20 values in each list
        attx = attx[-20:]
        atty = atty[-20:]
        attz = attz[-20:]
        rttx = rttx[-20:]
        rtty = rtty[-20:]
        rttz = rttz[-20:]

    # Clear the previous plot and plot the new data
    axes1[0].clear()
    axes1[0].plot(attx)
    axes1[0].set_title("Accelerometer X")

    axes1[1].clear()
    axes1[1].plot(atty)
    axes1[1].set_title("Accelerometer Y")

    axes1[2].clear()
    axes1[2].plot(attz)
    axes1[2].set_title("Accelerometer Z")

    axes2[0].clear()
    axes2[0].plot(rttx)
    axes2[0].set_title("Rotation X")

    axes2[1].clear()
    axes2[1].plot(rtty)
    axes2[1].set_title("Rotation Y")

    axes2[2].clear()
    axes2[2].plot(rttz)
    axes2[2].set_title("Rotation Z")

# Call the animate() function for each figure using FuncAnimation()
ani1 = animation.FuncAnimation(fig1, animate, fargs=(attx, atty, attz, rttx, rtty, rttz), interval=10)
ani2 = animation.FuncAnimation(fig2, animate, fargs=(attx, atty, attz, rttx, rtty, rttz), interval=10)

# Display the plots in separate windows
plt.show(block=False)
fig1.canvas.manager.window.move(0, 0)
fig2.canvas.manager.window.move(800, 0)
