import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import numpy as np
import csv
import math
from decimal import Decimal

fig = plt.figure()
ax = fig.gca(projection='3d')


pre_x = 0
pre_y = 0
pre_z = 0

#read the file 
with open('displacement_sample.txt','r') as csvfile:
    plots = csv.reader(csvfile, delimiter=' ')
    for row in plots: 
    	if(row[0] != "Orientation:"):
    			#use algorithm to connect vectors 
	       	ax.quiver(pre_x,  pre_y,  pre_z, float(row[0]), float(row[1]), float(row[2]), length=(math.sqrt(math.pow(float(row[0]), 2) + math.pow(float(row[1]), 2) + math.pow(float(row[2]), 2))), normalize = True)
	       	pre_x = pre_x + float(row[0])
	        pre_y = pre_y + float(row[1])
	        pre_z = pre_z + float(row[2])

plt.show()
