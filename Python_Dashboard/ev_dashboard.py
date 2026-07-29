import sys
from PySide6.QtCore import Qt, QTimer
from PySide6.QtWidgets import (
    QApplication,
    QLabel,
    QMainWindow,
    QWidget,
    QVBoxLayout,
    QHBoxLayout,
    QGridLayout,
    QProgressBar,
    QFrame
)
from PySide6.QtGui import QFont

import pyqtgraph as pg


class MetricCard(QFrame):

    def __init__(self, title, value="0", unit=""):
        super().__init__()

        self.setStyleSheet("""
        QFrame{
            background:#171717;
            border:2px solid #252525;
            border-radius:15px;
        }
        """)

        layout = QVBoxLayout()

        titleLabel = QLabel(title)
        titleLabel.setStyleSheet("color:#9E9E9E;")
        titleLabel.setFont(QFont("Segoe UI",11))

        self.valueLabel = QLabel(value)
        self.valueLabel.setStyleSheet("color:#00E676;")
        self.valueLabel.setFont(QFont("Segoe UI",24,QFont.Bold))

        unitLabel = QLabel(unit)
        unitLabel.setStyleSheet("color:white;")

        layout.addWidget(titleLabel)
        layout.addWidget(self.valueLabel)
        layout.addWidget(unitLabel)

        self.setLayout(layout)

    def setValue(self,value):
        self.valueLabel.setText(str(value))


class Dashboard(QMainWindow):

    def __init__(self):
        super().__init__()

        self.setWindowTitle("EV ADAS Dashboard")
        self.resize(1550,850)

        self.setStyleSheet("""
        QMainWindow{
            background:#101010;
        }

        QLabel{
            color:white;
        }

        QProgressBar{
            border-radius:10px;
            background:#222;
            color:white;
            text-align:center;
            height:25px;
        }

        QProgressBar::chunk{
            background:#00E676;
            border-radius:10px;
        }
        """)

        central=QWidget()
        self.setCentralWidget(central)

        root=QHBoxLayout(central)

        ####################################################
        # LEFT
        ####################################################

        left=QVBoxLayout()

        speedTitle=QLabel("SPEED")
        speedTitle.setAlignment(Qt.AlignCenter)
        speedTitle.setFont(QFont("Segoe UI",16,QFont.Bold))

        self.speedValue=QLabel("0 km/h")
        self.speedValue.setAlignment(Qt.AlignCenter)
        self.speedValue.setFont(QFont("Segoe UI",36,QFont.Bold))
        self.speedValue.setStyleSheet("color:#00E676")

        self.graph=pg.PlotWidget()

        self.graph.setBackground("#171717")

        self.graph.showGrid(x=True,y=True)

        self.curve=self.graph.plot(
            pen=pg.mkPen("#00E676",width=3)
        )

        self.history=[]

        left.addWidget(speedTitle)
        left.addWidget(self.speedValue)
        left.addWidget(self.graph)

        ####################################################
        # CENTER
        ####################################################

        center=QVBoxLayout()

        title=QLabel("EV ADAS Dashboard")
        title.setAlignment(Qt.AlignCenter)
        title.setFont(QFont("Segoe UI",22,QFont.Bold))
        title.setStyleSheet("color:#00E676")

        center.addWidget(title)

        self.batteryLabel=QLabel("Battery")
        self.batteryLabel.setAlignment(Qt.AlignCenter)
        self.batteryLabel.setFont(QFont("Segoe UI",18))

        center.addWidget(self.batteryLabel)

        self.battery=QProgressBar()
        self.battery.setValue(97)

        center.addWidget(self.battery)

        grid=QGridLayout()

        self.torque=MetricCard("Torque","150","Nm")
        self.accel=MetricCard("Accelerator","0","%")
        self.brake=MetricCard("Brake","0","%")
        self.temp=MetricCard("Motor Temp","25","°C")
        self.alarm=MetricCard("Alarm","NONE","")
        self.fault=MetricCard("Fault","0x00","")

        grid.addWidget(self.torque,0,0)
        grid.addWidget(self.accel,0,1)
        grid.addWidget(self.brake,1,0)
        grid.addWidget(self.temp,1,1)
        grid.addWidget(self.alarm,2,0)
        grid.addWidget(self.fault,2,1)

        center.addLayout(grid)

        ####################################################
        # RIGHT
        ####################################################

        right=QVBoxLayout()

        adas=QLabel("ADAS Bird Eye View")
        adas.setAlignment(Qt.AlignCenter)
        adas.setFont(QFont("Segoe UI",18,QFont.Bold))

        right.addWidget(adas)

        car=QFrame()

        car.setMinimumSize(300,500)

        car.setStyleSheet("""
        background:#171717;
        border:2px solid #333;
        border-radius:15px;
        """)

        layout=QVBoxLayout(car)

        carLabel=QLabel("CAR VIEW")
        carLabel.setAlignment(Qt.AlignCenter)
        carLabel.setFont(QFont("Segoe UI",20,QFont.Bold))

        distance=QLabel("Front Distance\n240 cm")
        distance.setAlignment(Qt.AlignCenter)

        layout.addStretch()
        layout.addWidget(distance)
        layout.addStretch()
        layout.addWidget(carLabel)
        layout.addStretch()

        right.addWidget(car)

        ####################################################

        root.addLayout(left,3)
        root.addLayout(center,3)
        root.addLayout(right,2)

        ####################################################
        # DEMO TIMER
        ####################################################

        self.speed=0

        self.timer=QTimer()
        self.timer.timeout.connect(self.demo)
        self.timer.start(200)

    def demo(self):

        self.speed=(self.speed+2)%120

        self.speedValue.setText(f"{self.speed} km/h")

        self.history.append(self.speed)

        if len(self.history)>60:
            self.history.pop(0)

        self.curve.setData(self.history)

        self.accel.setValue(self.speed)

        self.brake.setValue(max(0,100-self.speed))

        self.torque.setValue(120+self.speed)

        self.temp.setValue(25+self.speed//20)

        self.battery.setValue(97-self.speed//30)


app=QApplication(sys.argv)

window=Dashboard()

window.show()

sys.exit(app.exec())