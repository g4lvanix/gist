#!/usr/env/python3

morse = {"A:":".-","B":"-...","C":"-.-.","D":"-..","E":".",
		"F":"..-.","G":"--.","H":"....","I":"..","J":".---",
		"K":"-.-","L":".-..","M":"--","N":"-.","O":"---",
		"P":".--.","Q":"--.-","R":".-.","S":"...","T":"-",
		"U":"..-","V":"...-","W":".--","X":"-..-","Y":"-.--",
		"Z":"--..","1":".----","2":"..---","3":"...--","4":"....-",
		"5":".....","6":"-....","7":"--...","8":"---..","9":"---.",
		"0":"-----"}

def call2morse(call):
	message = ""
	for c in call:
		message += morse[c] + " "
	return message

def morse2bin(code,delay,mode):
	if mode == "fskcw":
		binstring = "000"
	else:
		binstring = ""

	for c in code:
		if c == "-":
			if mode == "fskcw":
				binstring += "1110"
			elif (mode == "dfcw") or (mode == "castle"):
				binstring += "1"
		elif c == ".":
			if mode == "fskcw":
				binstring += "10"
			elif (mode == "dfcw") or (mode == "castle"):
				binstring += "0"
		else:
			if mode == "fskcw":
				binstring += "00"
			elif mode == "dfcw":
				binstring += "0"

	# add in the TX delay
	binstring += "0"*delay;
	# fill up the string with more bits to get full byte allocation
	binstring += "0"*(len(binstring)%8)
	return binstring

def packbin(binstr):
	# calculate needed number of bytes to pack message
	nbytes = int(len(binstr)/8)

	output = "const uint8_t call["+str(nbytes)+"] = {\n\t"
	# print out individual bytes in hexadecimal
	for i in range(0,nbytes*8,8):
		output += "0x{:02x}".format(int(binstr[i:i+8],2))+","

	output += "\b\n};"

	return output

if __name__ == "__main__":
	print("g4lvanix morse code packer\n")

	print("Available keying modes: ")
	print("(1) FSK-CW")
	print("(2) DFCW")
	print("(3) CASTLE\n")
	keymode = int(input("Choose keying mode: "))
	callsign = input("Enter callsign: ").upper()
	unitdelay = int(input("Delay after call (units): "))

	m = call2morse(callsign)

	print("\nCleartext message:",callsign)
	print("Morse code message:",m,"\n")

	if (keymode == 1):
		b = morse2bin(m,unitdelay,"fskcw")
	elif (keymode == 2):
		b = morse2bin(m,unitdelay,"dfcw")
	elif (keymode == 3):
		b = morse2bin(m,unitdelay,"castle")
	else:
		print("Invalid keying mode specified")

	print(packbin(b))
