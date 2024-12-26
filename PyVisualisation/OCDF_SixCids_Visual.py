import sys
from matplotlib.pyplot import figure, show
from ZoomPan import *
from CidInfo import *


def zoom_visualisation(cid1_x=[], cid1_y=[], cid2_x=[], cid2_y=[], cid3_x=[], cid3_y=[], cid4_x=[], cid4_y=[], cid5_x=[], cid5_y=[], cid6_x=[], cid6_y=[]):
    fig = figure()

    min_x_list = []
    max_x_list = []
    min_y_list = []
    max_y_list = []
    legend_list = []
    ax_title = ""

    if len(cid1_y) > 0:
        ax_title += "1"
        min_x_list.append(cid1_x[0])
        max_x_list.append(cid1_x[len(cid1_x) - 1])
        min_y_list.append(cid1_y[0])
        max_y_list.append(cid1_y[len(cid1_y) - 1])
        legend_list.append(cids_info[int(1)])
    if len(cid2_y) > 0:
        if len(ax_title) >= 1:
            ax_title += " "
        ax_title += "2"
        min_x_list.append(cid2_x[0])
        max_x_list.append(cid2_x[len(cid2_x) - 1])
        min_y_list.append(cid2_y[0])
        max_y_list.append(cid2_y[len(cid2_y) - 1])
        legend_list.append(cids_info[int(2)])
    if len(cid3_y) > 0:
        if len(ax_title) >= 1:
            ax_title += " "
        ax_title += "3"
        min_x_list.append(cid3_x[0])
        max_x_list.append(cid3_x[len(cid3_x) - 1])
        min_y_list.append(cid3_y[0])
        max_y_list.append(cid3_y[len(cid3_y) - 1])
        legend_list.append(cids_info[int(3)])
    if len(cid4_y) > 0:
        if len(ax_title) >= 1:
            ax_title += " "
        ax_title += "4"
        min_x_list.append(cid4_x[0])
        max_x_list.append(cid4_x[len(cid4_x) - 1])
        min_y_list.append(cid4_y[0])
        max_y_list.append(cid4_y[len(cid4_y) - 1])
        legend_list.append(cids_info[int(4)])
    if len(cid5_y) > 0:
        if len(ax_title) >= 1:
            ax_title += " "
        ax_title += "5"
        min_x_list.append(cid5_x[0])
        max_x_list.append(cid5_x[len(cid5_x) - 1])
        min_y_list.append(cid5_y[0])
        max_y_list.append(cid5_y[len(cid5_y) - 1])
        legend_list.append(cids_info[int(5)])
    if len(cid6_y) > 0:
        if len(ax_title) >= 1:
            ax_title += " "
        ax_title += "6"
        min_x_list.append(cid6_x[0])
        max_x_list.append(cid6_x[len(cid6_x) - 1])
        min_y_list.append(cid6_y[0])
        max_y_list.append(cid6_y[len(cid6_y) - 1])
        legend_list.append(cids_info[int(6)])

    min_x = min(min_x_list)
    max_x = max(max_x_list)
    min_y = min(min_y_list)
    max_y = max(max_y_list)

    ax = fig.add_subplot(111, xlim=(min_x, max_x), ylim=(min_y, max_y), autoscale_on=False)

    if len(cid1_y) > 0:
        ax.plot(cid1_x, cid1_y, color='blue')
    if len(cid2_y) > 0:
        ax.plot(cid2_x, cid2_y, color='red')
    if len(cid3_y) > 0:
        ax.plot(cid3_x, cid3_y, color='green')
    if len(cid4_y) > 0:
        ax.plot(cid4_x, cid4_y, color='orange')
    if len(cid5_y) > 0:
        ax.plot(cid5_x, cid5_y, color='purple')
    if len(cid6_y) > 0:
        ax.plot(cid6_x, cid6_y, color='black')
    ax.set_title(f"Данные сидов: {ax_title}")
    ax.set_xlabel("Данные сидов")
    ax.set_xlabel("Дискретное время, сек")
    ax.legend(legend_list, loc="upper left")
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

zoom_visualisation(cid1_time, cid1_value, cid2_time, cid2_value, cid3_time, cid3_value, cid4_time, cid4_value, cid5_time, cid5_value, cid6_time, cid6_value)
