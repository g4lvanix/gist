#!/usr/bin/env python

import os,urllib
from datetime import datetime,timedelta
import ephem

base_url = "http://celestrak.com/NORAD/elements/"
tlefile = "amateur.txt"

def downloadTLE():
    webFile = urllib.urlopen(url)
    localFile = open(url.split('/')[-1], 'w')
    localFile.write(webFile.read())
    webFile.close()
    localFile.close()

def updateTLE():
    url = base_url + tlefile
    if os.access(tlefile,os.R_OK):
        # check if the TLE data is older than 12h
        t = os.path.gmtime(tlefile)
        d = datetime.fromtimestamp(t)
        thres = datetime.now() - timedelta(hours=12)

        if d < thres:
            os.remove(tlefile)
            downloadTLE()

    else:
        downloadTLE()

def parseTLE(fname):
    f = open(fname,'r')
    lines = f.readlines()
    tles = []
    line1 = ""
    name = ""
    for l in lines:
        if l.startswith("1"):
            line1 = l
        elif l.startswith("2 " + line1[2:7]):
            tles.append([name,line1,l])
        else:
            name = l

    return tles

def calcPasses(fname,obs):

    tledata = parseTLE(fname)

    for t in tledata:
        s = ephem.readtle(t[0],t[1],t[2])
        s.compute(obs)

        try:
            nextPass = obs.next_pass(s)
        except:
            break

        print "\n########################################\n"
        print t[0]
        print "Next Pass:"
        print "UTC"
        print "Rise:    ",nextPass[0]
        print "Max. Alt:",nextPass[2]
        print "Set:     ",nextPass[4]

        print "\nLocal time"
        print "Rise:    ",ephem.localtime(nextPass[0])
        print "Max. Alt:",ephem.localtime(nextPass[2])
        print "Set:     ",ephem.localtime(nextPass[4])

# Set up the location of the observing antenna
antloc = ephem.Observer()
antloc.lat = "51.0504"
antloc.lon = "13.7373"
antloc.pressure = 0
antloc.horizon = '-0:34'
antloc.date = datetime.utcnow()

# make sure our TLEs aren't completely out of date
updateTLE()

# check out when we can see stuff
calcPasses(tlefile,antloc)
