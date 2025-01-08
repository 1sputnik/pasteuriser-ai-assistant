import sys
from matplotlib.pyplot import figure, show

class ZoomPan:
    def __init__(self):
        self.press = None
        self.cur_xlim = None
        self.x0 = None
        self.y0 = None
        self.x1 = None
        self.y1 = None
        self.xpress = None
        self.ypress = None

    def zoom_factory(self, ax, base_scale=2.0):
        def zoom(event):
            cur_xlim = ax.get_xlim()

            xdata = event.xdata

            if event.button == 'down':
                scale_factor = 1 / base_scale
            elif event.button == 'up':
                scale_factor = base_scale
            else:
                scale_factor = 1
                print(event.button)

            new_width = (cur_xlim[1] - cur_xlim[0]) * scale_factor
            relx = (cur_xlim[1] - xdata)/(cur_xlim[1] - cur_xlim[0])
            ax.set_xlim([xdata - new_width * (1-relx), xdata + new_width * (relx)])
            ax.figure.canvas.draw()

        # get the figure of interest
        fig = ax.get_figure()
        fig.canvas.mpl_connect('scroll_event', zoom)

        return zoom

    def pan_factory(self, ax):
        def onPress(event):
            if event.inaxes != ax: return
            self.cur_xlim = ax.get_xlim()
            self.cur_ylim = ax.get_ylim()
            self.press = self.x0, self.y0, event.xdata, event.ydata
            self.x0, self.y0, self.xpress, self.ypress = self.press

        def onRelease(event):
            self.press = None
            ax.figure.canvas.draw()

        def onMotion(event):
            if self.press is None: return
            if event.inaxes != ax: return
            dx = event.xdata - self.xpress
            dy = event.ydata - self.ypress
            self.cur_xlim -= dx
            self.cur_ylim -= dy
            ax.set_xlim(self.cur_xlim)
            ax.set_ylim(self.cur_ylim)

            ax.figure.canvas.draw()

        fig = ax.get_figure()

        fig.canvas.mpl_connect('button_press_event',onPress)
        fig.canvas.mpl_connect('button_release_event',onRelease)
        fig.canvas.mpl_connect('motion_notify_event',onMotion)

        return onMotion


def zoom_visualisation(all_x=[], all_y=[], train_x=[], train_y=[], predict_x=[],predict_y=[]):
    fig = figure()

    x_border = 10000000

    if x_border > all_x[0]:
        if x_border > len(all_x):
            x_border = len(all_x)
        ax = fig.add_subplot(111, xlim=(all_x[0], x_border), ylim=(0, 200), autoscale_on=False)

        ax.plot(all_x, all_y, color='b')
        ax.plot(train_x, train_y, color='r')
        ax.plot(predict_x, predict_y, color='g')
        ax.grid(True)

        scale = 1.1
        zp = ZoomPan()
        figZoom = zp.zoom_factory(ax, base_scale=scale)
        figPan = zp.pan_factory(ax)
        show()

# for data storage
all_target_value = []
all_time = []
train_target_value = []
train_time = []
test_target_value = []
test_time = []

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
    if int(float(line_parts[2]) > 0):
        all_time.append(int(line_parts[1]))
        all_target_value.append(float(line_parts[2]))
work_file.close()

file_name = "train.csv"
work_file = open(file_name, 'r')
for line in work_file.readlines():
    line = line.replace("\n", "")
    line_parts = line.split(';')
    if int(float(line_parts[2]) > 0):
        train_time.append(int(line_parts[1]))
        train_target_value.append(float(line_parts[2]))
work_file.close()

file_name = "test.csv"
work_file = open(file_name, 'r')
for line in work_file.readlines():
    line = line.replace("\n", "")
    line_parts = line.split(';')
    if int(float(line_parts[2]) > 0):
        test_time.append(int(line_parts[1]))
        test_target_value.append(float(line_parts[2]))
print("(python) data have been read\n")
work_file.close()

zoom_visualisation(all_time, all_target_value, train_time, train_target_value, test_time, test_target_value)
