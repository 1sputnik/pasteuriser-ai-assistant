from matplotlib.pyplot import figure, show
import keyboard
from ZoomPan import *


def zoom_visualisation(gx=[], gy=[]):
    fig = figure()

    ax = fig.add_subplot(111, xlim=(gx[0], gx[len(gx) - 1]), ylim=(0, 0.022), autoscale_on=False)

    ax.plot(bx, by, color='blue')
    ax.set_title(f"Кривая темпа обучения модели нейронной сети")
    ax.set_xlabel("Номер эпохи")
    ax.set_ylabel("Значение MSE")
    ax.grid(True)

    scale = 1.1
    zp = ZoomPan()
    figZoom = zp.zoom_factory(ax, base_scale=scale)
    figPan = zp.pan_factory(ax)
    show()


# for data storage
g_error_target_value = []
g_error_time = []

# read data with primary data processing
file_name = "Error.csv"
work_file = open(file_name, 'r')
for line in work_file.readlines():
    if line == '\n':
        break
    line = line.replace("\n", "")
    line_parts = line.split(';')
    g_error_time.append(int(line_parts[0]))
    g_error_target_value.append(float(line_parts[1]))
work_file.close()

zoom_visualisation(g_error_time, g_error_target_value)
