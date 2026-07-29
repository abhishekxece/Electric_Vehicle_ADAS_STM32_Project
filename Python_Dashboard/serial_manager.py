import serial
import threading
import time


class SerialManager:

    def __init__(self, port="COM3", baudrate=115200):
        self.port = port
        self.baudrate = baudrate

        self.speed = 0
        self.battery = 100
        self.torque = 0
        self.accelerator = 0
        self.brake = 0
        self.temperature = 25
        self.distance = 0
        self.alarm = "NONE"
        self.fault = "0x00"

        self.connected = False
        self.running = False

    def start(self):
        self.running = True
        threading.Thread(target=self.read_serial, daemon=True).start()

    def stop(self):
        self.running = False

    def read_serial(self):

        try:
            ser = serial.Serial(self.port, self.baudrate, timeout=1)
            self.connected = True

            while self.running:

                try:

                    line = ser.readline().decode("utf-8").strip()

                    if not line:
                        continue

                    self.parse(line)

                except Exception:
                    pass

            ser.close()

        except Exception:
            self.connected = False

    def parse(self, line):

        # Expected format:
        # 45,97,120,35,0,28,140,NONE,0x00

        data = line.split(",")

        if len(data) < 9:
            return

        self.speed = int(data[0])
        self.battery = int(data[1])
        self.torque = int(data[2])
        self.accelerator = int(data[3])
        self.brake = int(data[4])
        self.temperature = int(data[5])
        self.distance = int(data[6])
        self.alarm = data[7]
        self.fault = data[8]