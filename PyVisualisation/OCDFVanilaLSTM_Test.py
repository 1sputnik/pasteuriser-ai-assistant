import sys
from matplotlib.pyplot import figure, show
from ZoomPan import *
from CidInfo import *


def zoom_visualisation(cid=str(), real_x=[], real_y=[], predict_x=[], predict_y=[]):
    fig = figure()

    ax = fig.add_subplot(111, xlim=(real_x[0], real_x[len(real_x) - 1]), ylim=(min(real_y), max(real_y)), autoscale_on=False)

    ax.plot(real_x, real_y, color='blue')
    ax.plot(predict_x, predict_y, color='green')
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
real_value = []
real_time = []
predict_value = []
predict_time = []

# read data with primary data processing
file_name = ""
if len(sys.argv) > 2:
    real_file_name = f"{sys.argv[1]}"
    predict_file_name = f"{sys.argv[2]}"
else:
    print("(python) enter the file name with real data: ")
    real_file_name = input()
    print("(python) enter the file name with predict data: ")
    predict_file_name = input()
real_work_file = open(real_file_name, 'r')
for line in real_work_file.readlines():
    line = line.replace("\n", "")
    line_parts = line.split(';')
    cid_ex = line_parts[0]
    real_time.append(int(line_parts[1]))
    real_value.append(float(line_parts[2]))
real_work_file.close()
predict_work_file = open(predict_file_name, 'r')
for line in predict_work_file.readlines():
    line = line.replace("\n", "")
    line_parts = line.split(';')
    predict_time.append(int(line_parts[1]))
    predict_value.append(float(line_parts[2]))
predict_work_file.close()

zoom_visualisation(cid_ex, real_time, real_value, predict_time, predict_value)
