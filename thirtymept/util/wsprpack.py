#!/usr/bin/env python3

# This script packs the 2 bit WSPR channel symbols generated
# by K1JT's WSPRcode.exe into bytes containing 4 symbols each
# The packing allows for more efficient memory usage on small
# microcontrollers used for ham radio MEPTs

wsprcode = "1 3 2 2 2 2 0 0 1 0 0 2 3 1 1 0 2 2 3 2 0 1 2 1 1 3 1 0 0 2\
 2 2 2 0 3 0 0 1 0 3 2 2 0 2 0 0 3 0 1 1 2 2 3 3 0 3 0 2 2 3\
 1 2 1 0 2 0 2 1 3 0 1 0 1 2 1 0 1 2 2 1 2 0 1 2 3 1 0 0 2 3\
 1 2 1 2 3 2 2 0 3 2 2 0 2 2 3 0 0 3 2 2 3 3 3 0 1 1 2 0 1 3\
 2 1 0 0 0 3 3 1 0 2 2 2 2 3 0 3 2 0 3 3 0 2 0 2 0 2 0 3 1 0\
 3 0 3 1 0 0 0 1 3 2 0 0"

nums = []

# convert the WSPRcode output to list of numbers
for c in wsprcode.split(" "):
	nums.append(int(c))

if len(nums) != 162:
	print("Error! Too many channel symbols!")

binary = []

# merge 4 channel symbols each into one 8 character (^= bit) string, save in list
# operates on the first 160 symbols, as these will pack into exactly 40 bytes
for i in range(0,len(nums)-2,4):
	s = bin(nums[i])[2:].zfill(2) + bin(nums[i+1])[2:].zfill(2) + bin(nums[i+2])[2:].zfill(2) + bin(nums[i+3])[2:].zfill(2)
	binary.append(s)

# this appends the last two symbols to the list of binary character strings
binary.append( bin(nums[160])[2:].zfill(2) + bin(nums[161])[2:].zfill(2) + "0000" )


# create a nice formatted output (C array) that can be directly copy pasted
# into a C source file
output = "const uint8_t wsprSymbols[41] = {"

for i in range(len(binary)):
	s = ""
	if (i%5 == 0):
		s += "\n\t"

	s += str(hex(int(binary[i],2))) + ","
	output += s

output += "\n};"

print(output)
