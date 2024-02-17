import sys
from matplotlib.pyplot import figure, show
from ZoomPan import *


def zoom_visualisation(x=[], cid1_y=[], cid2_y=[], cid3_y=[], cid4_y=[], cid5_y=[], cid6_y=[]):
    fig = figure()

    min_y = min(min(cid1_y), min(cid2_y), min(cid3_y), min(cid4_y), min(cid5_y), min(cid6_y))
    max_y = max(max(cid1_y), max(cid2_y), max(cid3_y), max(cid4_y), max(cid5_y), max(cid6_y))

    ax = fig.add_subplot(111, xlim=(min(x), max(x)), ylim=(min_y, max_y), autoscale_on=False)

    ax.plot(x, cid1_y, color='blue')
    ax.plot(x, cid2_y, color='red')
    ax.plot(x, cid3_y, color='green')
    ax.plot(x, cid4_y, color='orange')
    ax.plot(x, cid5_y, color='purple')
    ax.plot(x, cid6_y, color='black')
    ax.grid(True)

    scale = 1.1
    zp = ZoomPan()
    figZoom = zp.zoom_factory(ax, base_scale=scale)
    figPan = zp.pan_factory(ax)
    show()


# for data storage
time = []
cid1_value = []
cid2_value = []
cid3_value = []
cid4_value = []
cid5_value = []
cid6_value = []


# read data with primary data processing
file_name = ""
if len(sys.argv) > 1:
    file_name = f"{sys.argv[1]}"
else:
    print("(python) enter the file name: ")
    file_name = input()
work_file = open(file_name, 'r')
for line in work_file.readlines():
    line = line.replace("\n", "")
    line_parts = line.split(';')
    time.append(int(line_parts[0]))
    cid1_value.append(float(line_parts[1]))
    cid2_value.append(float(line_parts[2]))
    cid3_value.append(float(line_parts[3]))
    cid4_value.append(float(line_parts[4]))
    cid5_value.append(float(line_parts[5]))
    cid6_value.append(float(line_parts[6]))
work_file.close()

zoom_visualisation(time, cid1_value, cid2_value, cid3_value, cid4_value, cid5_value, cid6_value)
