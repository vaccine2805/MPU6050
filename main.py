from PyQt5 import QtWidgets, QtCore
from pyqtgraph import PlotWidget, plot
import pyqtgraph as pg
import sys  # We need sys so that we can pass argv to QApplication
import os
from random import randint
import serial
import datetime

class MainWindow(QtWidgets.QMainWindow):

    def __init__(self, *args, **kwargs):
        super(MainWindow, self).__init__(*args, **kwargs)

        self.ser = serial.Serial()
        self.ser.port = 'COM3'
        self.ser.baudrate = 115200
        self.ser.setDTR(False)
        self.ser.setRTS(False)

        self.ser.open()

        self.attx = []
        self.atty = []
        self.attz = []
        self.rttx = []
        self.rtty = []
        self.rttz = []

        self.graphWidget = pg.PlotWidget(self)
        self.graphWidget.setTitle("Accelerometer X")
        # self.graphWidget.setYRange(-1,1)
        self.graphWidget.showGrid(x=True, y=True)
        self.graphWidget2 = pg.PlotWidget(self)
        self.graphWidget2.setTitle("Accelerometer Y")
        self.graphWidget2.showGrid(x=True, y=True)
        # self.graphWidget2.setYRange(-1, 1)
        self.graphWidget3 = pg.PlotWidget(self)
        self.graphWidget3.setTitle("Accelerometer Z")
        self.graphWidget3.showGrid(x=True, y=True)
        self.graphWidget4 = pg.PlotWidget(self)
        self.graphWidget4.setTitle("Rotation X")
        self.graphWidget4.showGrid(x=True, y=True)
        self.graphWidget5 = pg.PlotWidget(self)
        self.graphWidget5.setTitle("Rotation Y")
        self.graphWidget5.showGrid(x=True, y=True)
        self.graphWidget6 = pg.PlotWidget(self)
        self.graphWidget6.setTitle("Rotation Z")
        self.graphWidget6.showGrid(x=True, y=True)


        self.x = list(range(20))  # 100 time points
        self.attx = [randint(0, 0) for _ in range(20)]  # 100 data points
        self.atty = [randint(0, 0) for _ in range(20)]  # 100 data points
        self.attz = [randint(0, 0) for _ in range(20)]  # 100 data points
        self.rttx = [randint(0, 0) for _ in range(20)]  # 100 data points
        self.rtty = [randint(0, 0) for _ in range(20)]  # 100 data points
        self.rttz = [randint(0, 0) for _ in range(20)]  # 100 data points

        self.graphWidget.setBackground('black')
        self.graphWidget2.setBackground('black')
        self.graphWidget3.setBackground('black')
        self.graphWidget4.setBackground('black')
        self.graphWidget5.setBackground('black')
        self.graphWidget6.setBackground('black')

        pen = pg.mkPen(color=(255, 0, 0))
        pen2 = pg.mkPen(color=(0, 255, 0))
        pen3 = pg.mkPen(color=(0, 0, 255))
        pen4 = pg.mkPen(color=(0, 102, 102))
        pen5 = pg.mkPen(color=(204, 0, 204))
        pen6 = pg.mkPen(color=(255, 128, 0))

        self.data_line = self.graphWidget.plot(self.x, self.attx, pen=pen)
        self.data_line2 = self.graphWidget2.plot(self.x, self.atty, pen=pen2)
        self.data_line3 = self.graphWidget3.plot(self.x, self.attz, pen=pen3)
        self.data_line4 = self.graphWidget4.plot(self.x, self.rttx, pen=pen4)
        self.data_line5 = self.graphWidget5.plot(self.x, self.rtty, pen=pen5)
        self.data_line6 = self.graphWidget6.plot(self.x, self.rttz, pen=pen6)


        layout = QtWidgets.QGridLayout()
        layout.addWidget(self.graphWidget, 0, 0)
        layout.addWidget(self.graphWidget2, 1, 0)
        layout.addWidget(self.graphWidget3, 2, 0)
        layout.addWidget(self.graphWidget4, 0, 1)
        layout.addWidget(self.graphWidget5, 1, 1)
        layout.addWidget(self.graphWidget6, 2, 1)

        centralWidget = QtWidgets.QWidget()
        centralWidget.setLayout(layout)
        self.setCentralWidget(centralWidget)

        self.resize(800, 600)

        self.timer = QtCore.QTimer()
        self.timer.setInterval(50)
        self.timer.timeout.connect(self.update_plot_data)
        self.timer.start()



    def update_plot_data(self):

        b = self.ser.readline()
        str1 = b.decode('UTF-8')
        lst = str1.split('|')
        if len(lst) > 0:
            att = lst[0].split(',')
            rtt = lst[1].split(',')

            # Keep only the last 20 values in each list
            self.attx = self.attx[1:]
            self.atty = self.atty[1:]
            self.attz = self.attz[1:]
            self.rttx = self.rttx[1:]
            self.rtty = self.rtty[1:]
            self.rttz = self.rttz[1:]

            self.attx.append(float(att[0]))
            self.atty.append(float(att[1]))
            self.attz.append(float(att[2]))
            self.rttx.append(float(rtt[0]))
            self.rtty.append(float(rtt[1]))
            self.rttz.append(float(rtt[2]))

            self.x = self.x[1:]  # Remove the first y element.
            self.x.append(self.x[-1] + 1)  # Add a new value 1 higher than the last.


        self.data_line.setData(self.x, self.attx)  # Update the data.
        self.data_line2.setData(self.x, self.atty)  # Update the data.
        self.data_line3.setData(self.x, self.attz)  # Update the data.
        self.data_line4.setData(self.x, self.rttx)  # Update the data.
        self.data_line5.setData(self.x, self.rtty)  # Update the data.
        self.data_line6.setData(self.x, self.rttz)  # Update the data.

app = QtWidgets.QApplication(sys.argv)
w = MainWindow()
w.show()
sys.exit(app.exec_())