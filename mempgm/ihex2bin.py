#!/usr/bin/env python3

from ctypes import c_uint8

class IHEXFile:
	def __init__(self, fname):
		self.hexfile = open(fname, 'r')	
		self.hexlines = list(self.hexfile)	# list holds single hex lines out of file
		self.hexfile.close()
		self.recinf = list()
	
	def analyzeLine(self, line):
		recdat = list()	# holds data bytes from record
		
		# slice single line of IHEX file to retrieve record information
		reclen = int(self.hexlines[line][1:3], 16)
		recaddr = int(self.hexlines[line][3:7], 16)
		rectyp = int(self.hexlines[line][7:9], 16)
		
		for n in range(reclen):
			recdat.append(int(self.hexlines[line][9+(n*2):11+(n*2)], 16))	
		
		# generate list of data records with corresponding information 
		# for internal use 
		self.recinf.append((rectyp, recaddr, reclen, recdat))
	
	def analyzeFile(self):
		for n in range(len(self.hexlines)):
			self.analyzeLine(n)
	
	# these are the beans
	def makeMemoryMap(self, fname, nbytes):
		print("Making memory map!")
		
		membaseaddr = 0
		memrecaddr = 0
		memcurraddr = 0

		self.binfile = open(fname, 'wb')
		
		# iterate over the ihex records and generate a stream of bytes
		for record in self.recinf:
						
			# data record
			if record[0] == 0x00:
				memrecaddr = membaseaddr + record[1]
#				self.binfile.write(bytearray(record[3]))
				
				if memcurraddr < nbytes:
				
					if memrecaddr == memcurraddr:					
						self.binfile.write(bytearray(record[3]))
						memcurraddr += record[2]
			
					else: 
						while memcurraddr < memrecaddr:
							self.binfile.write(bytearray([0xFF]))
							memcurraddr += 1
				
						self.binfile.write(bytearray(record[3]))	
						memcurraddr += record[2]
				
			# end of file
			elif record[0] == 0x01:
				print("End of IHEX file reached!")
				self.binfile.close()
				
			# extended linear address 
			elif record[0] == 0x04:
				membaseaddr = (record[3][1]<<24) + (record[3][0]<<16)
				
			else:
				print("Invalid record type!")	

if __name__ == "__main__":

	infile = input("Path to IHEX: ")
	outfile = input("Path to BIN: ")
	nbytes = int(input("Memory size in bytes: "))

	hf = IHEXFile(infile)
	hf.analyzeFile()
	hf.makeMemoryMap(outfile, nbytes)
