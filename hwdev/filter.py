#!/usr/bin/env python

import math

butterworth = [ [1.414,1.414],[1,2,1],[0.7654,1.85,1.85,0.7654],\
                [0.618,1.618,2,1.618,0.618], [0.5176,1.414,1.932,1.932,1.414,0.5176],\
                [0.445,1.247,1.802,2,1.802,1.247,0.445] ]

def calcValue(n,coeff,fc,order=7,ftype="lo",z0=50):
    v = 0.0

    if ftype == "lo":
        if n%2 == 0:
            v = coeff[order-2][n-1] / (2*math.pi*fc*z0)
        else:
            v = (coeff[order-2][n-1] * z0) / (2*math.pi*fc)

    elif ftype == "hi":
        if n%2 == 0:
            v = (coeff[order-2][n-1] * z0) / (2*math.pi*fc)
        else:
            v = coeff[order-2][n-1] / (2*math.pi*fc*z0)

    return v



if __name__ == "__main__":
    ftype = raw_input("Enter filter type (lo/hi): ")
    fc = float(input("Enter cutoff frequnecy in MHz: ")) * 10**6;
    order = int(input("Enter filter order (e.g. 7): "))
    z0 = float(input("Enter terminating impedance in Ohms: "))

    for i in range(1,order+1):
        print calcValue(i,butterworth,fc,order,ftype,z0)
