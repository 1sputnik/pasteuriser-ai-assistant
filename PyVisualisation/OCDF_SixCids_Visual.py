import sys
from matplotlib.pyplot import figure, show
from ZoomPan import *


def zoom_visualisation(cid1_x=[], cid1_y=[], cid2_x=[], cid2_y=[], cid3_x=[], cid3_y=[], cid4_x=[], cid4_y=[], cid5_x=[], cid5_y=[], cid6_x=[], cid6_y=[]):
    fig = figure()

    min_x = min(min(cid1_x), min(cid2_x), min(cid3_x), min(cid4_x), min(cid5_x), min(cid6_x))
    max_x = max(max(cid1_x), max(cid2_x), max(cid3_x), max(cid4_x), max(cid5_x), max(cid6_x))
    min_y = min(min(cid1_y), min(cid2_y), min(cid3_y), min(cid4_y), min(cid5_y), min(cid6_y))
    max_y = max(max(cid1_y), max(cid2_y), max(cid3_y), max(cid4_y), max(cid5_y), max(cid6_y))

    ax = fig.add_subplot(111, xlim=(min_x, max_x), ylim=(min_y, max_y), autoscale_on=False)

    ax.plot(cid1_x, cid1_y, color='blue')
    ax.plot(cid2_x, cid2_y, color='red')
    ax.plot(cid3_x, cid3_y, color='green')
    ax.plot(cid4_x, cid4_y, color='orange')
    ax.plot(cid5_x, cid5_y, color='purple')
    ax.plot(cid6_x, cid6_y, color='black')
    ax.set_title(f"Данные временного ряда")
    ax.set_xlabel("Данные сидов")
    ax.set_xlabel("Дискретное время в секундах: t,sec")
    ax.grid(True)

    scale = 1.1
    zp = ZoomPan()
    figZoom = zp.zoom_factory(ax, base_scale=scale)
    figPan = zp.pan_factory(ax)
    show()


# for data storage
all_values = []
all_time = []
all_data_cid = []

cid1_value = []
cid1_time = []

cid2_value = []
cid2_time = []

cid3_value = []
cid3_time = []

cid4_value = []
cid4_time = []

cid5_value = []
cid5_time = []

cid6_value = []
cid6_time = []


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
    all_data_cid.append(int(line_parts[0]))
    all_time.append(int(line_parts[1]))
    all_values.append(float(line_parts[2]))
work_file.close()

for i in range(len(all_time) - 1):
    if all_data_cid[i] == 1:
        cid1_value.append(all_values[i])
        cid1_time.append(all_time[i])
    elif all_data_cid[i] == 2:
        cid2_value.append(all_values[i])
        cid2_time.append(all_time[i])
    elif all_data_cid[i] == 3:
        cid3_value.append(all_values[i])
        cid3_time.append(all_time[i])
    elif all_data_cid[i] == 4:
        cid4_value.append(all_values[i])
        cid4_time.append(all_time[i])
    elif all_data_cid[i] == 5:
        cid5_value.append(all_values[i])
        cid5_time.append(all_time[i])
    elif all_data_cid[i] == 6:
        cid6_value.append(all_values[i])
        cid6_time.append(all_time[i])

if len(cid1_time) == 0:
    cid1_time.append(0)
if len(cid2_time) == 0:
    cid2_time.append(0)
if len(cid3_time) == 0:
    cid3_time.append(0)
if len(cid4_time) == 0:
    cid4_time.append(0)
if len(cid5_time) == 0:
    cid5_time.append(0)
if len(cid6_time) == 0:
    cid6_time.append(0)

if len(cid1_value) == 0:
    cid1_value.append(0)
if len(cid2_value) == 0:
    cid2_value.append(0)
if len(cid3_value) == 0:
    cid3_value.append(0)
if len(cid4_value) == 0:
    cid4_value.append(0)
if len(cid5_value) == 0:
    cid5_value.append(0)
if len(cid6_value) == 0:
    cid6_value.append(0)

zoom_visualisation(cid1_time, cid1_value, cid2_time, cid2_value, cid3_time, cid3_value, cid4_time, cid4_value, cid5_time, cid5_value, cid6_time, cid6_value)
