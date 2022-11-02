# Untitled - By: lenovo - 周六 10月 1 2022
THRESHOLD = (38, 100, -128, 81, -128, 127) # Grayscale threshold for dark things...
import sensor, image, time
from pyb import LED,UART
#import car
from pid import PID
#from pyb import UART,LED

rho_pid = PID(p=0.4, i=0)
theta_pid = PID(p=0.001, i=0)

LED(1).on()
LED(2).on()
LED(3).on()

sensor.reset()
sensor.set_vflip(True)
sensor.set_hmirror(True)
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QQVGA) # 80x60 (4,800 pixels) - O(N^2) max = 2,3040,000.
#sensor.set_windowing([0,20,80,40])
sensor.skip_frames(time = 2000)     # WARNING: If you use QQVGA it may take seconds
clock = time.clock()                # to process a frame sometimes.

uart = UART(1, 115200)

while(True):
    clock.tick()
    img = sensor.snapshot().binary([THRESHOLD])
    line = img.get_regression([(100,100)], robust = True)
    if (line):
        rho_err = abs(line.rho())-img.width()/2
        if line.theta()>90:
            theta_err = line.theta()-180
        else:
            theta_err = line.theta()
        img.draw_line(line.line(), color = 127)
        #print(rho_err,line.magnitude(),rho_err)
        if line.magnitude()>8:
            #if -40<b_err<40 and -30<t_err<30:
            rho_output = rho_pid.get_pid(rho_err,1)
            theta_output = theta_pid.get_pid(theta_err,1)
            output = rho_output+theta_output
            #car.run(50+output, 50-output)
            if(output<0):
                ouput = abs(output) + 100
            OUTPUT = str(round(output))
            uart.write(OUTPUT)
            uart.write('\r\n')
            print(OUTPUT)

        pass
    #print(clock.fps())
