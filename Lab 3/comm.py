#!/usr/bin/python
import random, sys, locale, string 
from optparse import OptionParser
locale.setlocale(locale.LC_COLLATE, 'C')


class comm: 
	def __init__(self, file): 
		if file == '-': #script is given "-" as ONE of the files
			f = sys.stdin #read from stdin
		else:
			f = open(file, 'r')  #f1 will open the file --> has the file
		self.line = f.readlines()  #self.line1 will have all lines from file1
		f.close()	#close the file
		if not self.line[len(self.line)-1].endswith("\n"):
			self.line[len(self.line)-1] = self.line[len(self.line)-1] + "\n"
	
	def getLines(self):
   		return self.line  #returns line from file

class formatColumns: 
	#supressing portion here
    def __init__(self, supress1, supress2, supress3):
        self.supress1 = supress1  #all are true if you want to supress(1,2,3)
        self.supress2 = supress2
        self.supress3 =  supress3 
    
    def fileText(self, col, word):
    	if (col == 1):
    		return word
   
    	if (col == 2):
    		return "\t" + word

    	if (col == 3):
    		return "\t\t" + word

def main():
	version_msg = "%prog 2.0"
	usage_msg = """%prog [OPTION]... FILE1 FILE2

Compares FILE1 and FILE2. No options, outputs columns 1, 2 and 3. 
Options are -1, -2, -3, and -u. -1 outputs columns 2 and 3. -2 outputs
columns 1 and 3. -3 outputs columns 1 and 2. -12 outputs col3. -13 outputs
column 2. -23 outputs column1. -u deals with unsorted files."""

	parser = OptionParser(version=version_msg,
                          usage=usage_msg)
	#options -1,-2,-3,-u here
	parser.add_option("-1", "--supressC1", action="store_true", dest="col1", default=False, 
					  help="supresses first column containing lines unique to file1")
	parser.add_option("-2", "--supressC2", action="store_true", dest="col2", default=False,
					  help="supresses second column containing lines uniqe to file2")
	parser.add_option("-3", "--supressC3", action="store_true", dest="col3", default=False,
					  help="supresses third column containing duplicates of file1 and file2")
	parser.add_option("-u", "--notsorted", action="store_true", dest="unsorted", default=False,
					   help="compares unsorted file1 and file2")
	options, args = parser.parse_args(sys.argv[1:])  #parses the options and arguments 
	col1 = options.col1   #col1,2,3 and unsorted are assigned the options they got 
	col2 = options.col2
	col3 = options.col3
	unsorted = options.unsorted

  	#errors 
	if len(args) != 2:
		parser.error("wrong number of operands. please enter 2 files.")
	
	try:
		#get the options args 
		file1 = comm(args[0])  #first file
		file2 = comm(args[1])  #second file
		
		f1Lines = file1.getLines()  #getting the first line of f1
		f2Lines = file2.getLines() #getting the first line of f2
		
		formatGenerator = formatColumns(col1, col2, col3)
		i = 0
		j = 0
		result = "" #needed to use options 
		if unsorted: 
			#if unsorted, check file1 with file2 all the way through
			#if match then print to 3rd column and remove from file2
			#if doesn't match then print to column 1
			#after that is all done, print the rest of file2 in column 2
			for i in range(len(f1Lines)):
				for j in range(len(f2Lines)):
					if (f1Lines[i]) == (f2Lines[j]):
						if not col3: #col3 not supressed
							if col1 and col2: #but col1 and 2 are supressed
								sys.stdout.write(formatGenerator.fileText(1, f1Lines[i])) #put in col1
							elif col1 or col2: 
								sys.stdout.write(formatGenerator.fileText(2, f1Lines[i])) #put in col2
							else:
								sys.stdout.write(formatGenerator.fileText(3, f1Lines[i])) #nothing supressed put in col3
						else: 
							result += ""
							sys.stdout.write(result)
						f2Lines.remove(f1Lines[i])	
						break
				else: 
					if not col1: 
						sys.stdout.write(formatGenerator.fileText(1, f1Lines[i]))
					else:
						result += ""
						sys.stdout.write(result)	
			#print out remaining of file2 in col2
			for j in range(len(f2Lines)):
				if not col2:
					if col1: 
						sys.stdout.write(formatGenerator.fileText(1, f2Lines[j]))
					else:
						sys.stdout.write(formatGenerator.fileText(2, f2Lines[j]))

		else:  #already sorted
			while (i < len(f1Lines) and j < len(f2Lines)):
				if (f1Lines[i]) == (f2Lines[j]):
					if not col3: #col1 not supressed
						if col1 and col2: 
							sys.stdout.write(formatGenerator.fileText(1, f1Lines[i]))
						else:
							if col1 or col2: 
								sys.stdout.write(formatGenerator.fileText(2, f1Lines[i]))
							else:
								sys.stdout.write(formatGenerator.fileText(3, f1Lines[i]))
					else:
						result += ""
						sys.stdout.write(result)
					i = i + 1
					j = j + 1
				else:
					if (f1Lines[i]) < (f2Lines[j]):
						if not col1:
							sys.stdout.write(formatGenerator.fileText(1, f1Lines[i]))
						else:
							result += ""
							sys.stdout.write(result)
						i = i + 1
					else:
						if (f1Lines[i]) > (f2Lines[j]):
							if not col2:
								if col1:
									sys.stdout.write(formatGenerator.fileText(1, f2Lines[j]))
								else:
									sys.stdout.write(formatGenerator.fileText(2, f2Lines[j]))
							else:
								result += ""
								sys.stdout.write(result)
							j = j + 1
			#print remaining if there are remainings
			for k in range(i, len(f1Lines)):
				if not col1:
					sys.stdout.write(formatGenerator.fileText(1, f1Lines[k]))
				else:
					result += ""
					sys.stdout.write(result)
			for l in range(j, len(f2Lines)):
				if not col2: 
					if col1: 
						sys.stdout.write(formatGenerator.fileText(1, f2Lines[l]))
					else:
						sys.stdout.write(formatGenerator.fileText(2, f2Lines[l]))
				else: 
					result += ""
					sys.stdout.write(result)

	except IOError as err:
		errno, strerror = err.args
		parser.error("I/O error({0}): {1}".format(errno, strerror))

if __name__ == "__main__":
    main()

