import sys
from matplotlib.pyplot import figure, show
from ZoomPan import *
from CidInfo import *


def zoom_visualisation(f_cid=str(), s_cid=str(), f_real_x=[], f_real_y=[], s_real_x=[], s_real_y=[], predict_x=[], predict_y=[]):
    fig = figure()
    legend_list = []
    legend_list.append(cids_info[int(int(f_cid))])
    legend_list.append(cids_info[int(int(s_cid))])
    legend_list.append("Данные прогноза")

    ax = fig.add_subplot(111, xlim=(f_real_x[0], f_real_x[len(f_real_x) - 1]), ylim=(min(f_real_y), max(f_real_y)), autoscale_on=False)

    ax.plot(f_real_x, f_real_y, color='blue')
    ax.plot(s_real_x, s_real_y, color='purple')
    ax.plot(predict_x, predict_y, color='green')
    ax.set_title(f"Результаты работы нейронной сети")
    ax.set_xlabel("Дискретное время, сек")
    ax.legend(legend_list, loc="upper left")
    ax.grid(True)

    scale = 1.1
    zp = ZoomPan()
    figZoom = zp.zoom_factory(ax, base_scale=scale)
    figPan = zp.pan_factory(ax)
    show()

# for data storage
f_cid = ""
s_cid = ""
first_real_value = []
first_real_time = []
second_real_value = []
second_real_time = []
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
f_cid = '0'
for line in real_work_file.readlines():
    line = line.replace("\n", "")
    line_parts = line.split(';')
    if f_cid == '0':
        f_cid = line_parts[0]
    if f_cid == line_parts[0]:
        first_real_time.append(int(line_parts[1]))
        first_real_value.append(float(line_parts[2]))
    else:
        s_cid = line_parts[0]
        second_real_time.append(int(line_parts[1]))
        second_real_value.append(float(line_parts[2]))
real_work_file.close()
predict_work_file = open(predict_file_name, 'r')
for line in predict_work_file.readlines():
    line = line.replace("\n", "")
    line_parts = line.split(';')
    predict_time.append(int(line_parts[1]))
    predict_value.append(float(line_parts[2]))
predict_work_file.close()

zoom_visualisation(f_cid, s_cid, first_real_time, first_real_value, second_real_time, second_real_value, predict_time, predict_value)
