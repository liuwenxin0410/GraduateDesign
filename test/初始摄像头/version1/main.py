THRESHOLD = (5, 70, -23, 15, -57, 0)
import sensor, image, time
from pyb import LED,UART
import car
from pid import PID
rho_pid = PID(p=0.4, i=0)
theta_pid = PID(p=0.001, i=0)
LED(1).on()
LED(2).on()
LED(3).on()
sensor.reset()
sensor.set_vflip(True)
sensor.set_hmirror(True)
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QQQVGA)
sensor.skip_frames(time = 2000)
clock = time.clock()
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
		if line.magnitude()>8:
			rho_output = rho_pid.get_pid(rho_err,1)
			theta_output = theta_pid.get_pid(theta_err,1)
			output = rho_output+theta_output
			if(output<0):
				ouput = abs(output) + 100
			OUTPUT = str(round(output))
			uart.write(OUTPUT)
			uart.write('\r\n')
			print(OUTPUT)
		pass