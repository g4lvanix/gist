#!/bin/bash

rm mono.raw
arecord -d 60 -D hw:0,0 -c 2 -r 192000 -f S16_LE stereo.wav
avconv -i stereo.wav -f s16le -ar 192000 -acodec pcm_s16le -ac 1 mono.raw
rm stereo.wav
