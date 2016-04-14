#!/usr/bin/env python3

gga = "GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,"

k = 0

for c in gga:
	k ^= ord(c)

print(hex(k))	
