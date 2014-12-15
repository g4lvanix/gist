#!/usr/bin/env python3

import math, datetime

def hsv_color(h, s, v):
	
	c = s*v
	
	x = c*(1-abs(((h/60)%2)-1))
	
	m = v-c
	
	if h < 60:
		rgb = [c+m, x+m, m]
	elif h < 120:
		rgb = [x+m, c+m, m]
	elif h < 180:
		rgb = [m, c+m, x+m]
	elif h < 240:
		rgb = [m, x+m, c+m]
	elif h < 300:
		rgb = [x+m, m, c+m]
	elif h < 360:
		rgb = [c+m, m, x+m]
		
	return rgb

minangle = 360/3600

while 1:
	t = datetime.datetime.now()
	phi = minangle*((t.hour*60)+t.minute)
	rgb = hsv_color(phi, 1, (t.second/60))
	rgb[0] = round(rgb[0]*255)
	rgb[1] = round(rgb[1]*255)
	rgb[2] = round(rgb[2]*255)
	
	print(str(t.hour) + ":" + str(t.minute) + ":" + str(t.second))
	print("Phi: " + str(phi))
	print("R: " + str(rgb[0]) + " G: " + str(rgb[1]) + " B: " + str(rgb[2]))
