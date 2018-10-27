#!/usr/bin/python
import sys
import datetime
import matplotlib.pyplot as plt

plt.title('Convex Hull')
plt.grid(True)

no_of_args = len(sys.argv)

if(no_of_args < 2):
	sys.stderr.write("ERROR!, Expected atleast one argument, output.txt file\n \
		Usage - This program takes atleast one argument, The text file containing \
		space separated coordinates in a line. You can also pass a input file containing \
		list of all coordinates. You can also pass '-s' flag to directly save the generated plot.\n \
		Example - demo.py output.txt input.txt(OPTIONAL) -s(OPTIONAL)\n")
	sys.exit(1)

out_x = []
out_y = []
with open(sys.argv[1], "r") as f:
	for line in f:
		try:
			a,b = map(float, line.split())
			out_x.append(a)
			out_y.append(b)
		except:
			sys.stderr.write("ERROR! - File is malformed. It should have two space separated \
				real numbers per line.")
			sys.exit(1)
	f.close()
plt.scatter(out_x, out_y)
plt.plot(out_x, out_y,color='magenta')

if((no_of_args == 3 and sys.argv[2] != '-s') or (no_of_args > 3)):
	inp_x = []
	inp_y = []
	with open(sys.argv[2], "r") as f:
		lines_2_to_end = f.readlines()[1:]
		for line in lines_2_to_end:
			try:
				a,b = map(float, line.split())
				inp_x.append(a)
				inp_y.append(b)
			except:
				sys.stderr.write("ERROR! - File is malformed. It should have two space separated \
					real numbers per line., with first line containing no. of values.")
				sys.exit(1)
		f.close()
		plt.scatter(inp_x, inp_y)

if(sys.argv[no_of_args-1] == '-s'):
	filename = datetime.datetime.now()
	filename = "plot-"+str(filename)+".jpg"
	plt.savefig(filename)
else:
	plt.show()