from pybricks.hubs import TechnicHub
from pybricks.pupdevices import Motor
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Button, Color, Direction, Port, Side, Stop
from pybricks.robotics import DriveBase
from pybricks.tools import wait, StopWatch
import ustruct

p=PUPDevice(Port.A)

while True:
    a=p.read(0)
    outp=ustruct.unpack('6h',ustruct.pack('12b',*a))
    print(outp)