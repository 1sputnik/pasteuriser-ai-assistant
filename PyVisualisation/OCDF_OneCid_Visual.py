import sys
from matplotlib.pyplot import figure, show
from ZoomPan import *
from CidInfo import *


def zoom_visualisation(cid=str(), all_x=[], all_y=[]):
    fig = figure()

    ax = fig.add_subplot(111, xlim=(all_x[0], all_x[len(all_x) - 1]), ylim=(min(all_y), max(all_y)), autoscale_on=False)

    ax.plot(all_x, all_y, color='b')
    ax.set_title(f"Данные сида {cid}")
    ax.set_ylabel(f"{cids_info[int(cid)]}")
    ax.set_xlabel("Дискретное время, сек")
    ax.grid(True)

    scale = 1.1
    zp = ZoomPan()
    figZoom = zp.zoom_factory(ax, base_scale=scale)
    figPan = zp.pan_factory(ax)
    show()

# for data storage
cid_ex = ""
all_target_value = []
all_time = []

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
    cid_ex = line_parts[0]
    all_time.append(int(line_parts[1]))
    all_target_value.append(float(line_parts[2]))
work_file.close()

zoom_visualisation(cid_ex, all_time, all_target_value)
