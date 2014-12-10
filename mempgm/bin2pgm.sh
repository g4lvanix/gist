#!/bin/bash

convert()
{
	FILESIZE=$(stat -c%s "$1")	# get file size of binary file in bytes
	PGMLINES=$(($FILESIZE/$2))	# calculate the number of lines the PGM image will have

	# user info output
	echo "Reading binary file: $1"
	echo "Generating PGM image with $2 bytes per line"
	echo "Reading $FILESIZE bytes from file, $PGMLINES lines"

	# write PGM file header
	echo "P2">${3}	
	echo "$2 $PGMLINES">>${3}
	echo "255">>${3}

	# dump binary data in decimal format/plain ASCII to PGM file 
	hexdump -v -e ''"$2"'/1 "%u ""\n"'  $1 >> ${3}
	exit
}

usage()
{
	echo "This script converts a binary file to an image in grayscale PGM format"
	echo "Usage: ./bin2pgm.sh [BINFILE] [NBYTES] [PGMFILE]"
	echo "BINFILE:  filename of bin file"
	echo "NBYTES:number of bytes of the bin file to be represented per line in the image"
	echo "PGMFILE: filename of the PGM image file"
	exit
}

if [ $# -eq 1 -a "$1" == "--help" ]; then
	usage
fi

convert $1 $2 $3
