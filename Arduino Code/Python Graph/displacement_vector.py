import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import numpy as np
import csv
import math
from decimal import Decimal

fig = plt.figure()
ax = fig.gca(projection='3d')

# Make the grid
#x, y, z = np.meshgrid(np.arange(-10, 10, 1),
#                      np.arange(-10, 10, 1),
#                      np.arange(-10, 10, 1))

#ax.plot3D(xline, yline, zline, 'gray')
# Make the direction data for the arrows

pre_x = 0
pre_y = 0
pre_z = 0
with open('displacement_sample.txt','r') as csvfile:
    plots = csv.reader(csvfile, delimiter=' ')
    for row in plots: 
        ax.quiver(pre_x,  pre_y,  pre_z, float(row[0]), float(row[1]), float(row[2]), length=(math.sqrt(math.pow(float(row[0])-pre_x, 2) + math.pow(float(row[1])-pre_y, 2) +math.pow(float(row[2])-pre_z, 2))), normalize = True)
        pre_x = float(row[0])
        pre_y = float(row[1])
        pre_z = float(row[2])



plt.show()
