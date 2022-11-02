import time, sensor, image
from image import SEARCH_EX, SEARCH_DS
sensor.reset()
sensor.set_contrast(1)
sensor.set_gainceiling(16)
sensor.set_framesize(sensor.QQVGA)
sensor.set_pixformat(sensor.GRAYSCALE)
template = image.Image("/1.pgm")
clock = time.clock()
while (True):
	clock.tick()
	img = sensor.snapshot()
	r = img.find_template(template, 0.70, step=4, search=SEARCH_EX)
	if r:
		img.draw_rectangle(r)
	print(clock.fps())