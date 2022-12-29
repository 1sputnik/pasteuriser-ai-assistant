from matplotlib import pyplot
from matplotlib import style
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


def clean_visualisation(_x=[], _y=[]):
    style.use('ggplot')
    pyplot.figure(figsize=(240, 20), dpi=80)

    pyplot.plot(_x, _y, 'b', linewidth=0.5)

    pyplot.grid(True)
    pyplot.show()


def double_visualisation(_x=[], _y=[]):
    style.use('ggplot')
    pyplot.figure(figsize=(240, 10), dpi=80)

    pyplot.subplot(2, 1, 1)
    pyplot.plot(_x[:int(len(_x)/2)],
                _y[:int(len(_y)/2)],
                'b', linewidth=1)
    pyplot.grid(True)

    pyplot.subplot(2, 1, 2)
    pyplot.plot(_x[int(len(_x)/2):],
                _y[int(len(_y)/2):],
                'b', linewidth=1)
    pyplot.grid(True)

    pyplot.show()


def quad_visualisation(_x=[], _y=[]):
    style.use('ggplot')
    pyplot.figure(figsize=(240, 10), dpi=80)

    pyplot.subplot(4, 1, 1)
    pyplot.plot(_x[: int(len(_x)/4)],
                _y[: int(len(_y)/4)],
                'b', linewidth=0.5)
    pyplot.grid(True)

    pyplot.subplot(4, 1, 2)
    pyplot.plot(_x[int(len(_x)/4) : int(len(_x)/2)],
                _y[int(len(_y)/4) : int(len(_y)/2)],
                'b', linewidth=0.5)
    pyplot.grid(True)

    pyplot.subplot(4, 1, 3)
    pyplot.plot(_x[int(len(_x)/2) : int(len(_x)/4)*3],
                _y[int(len(_y)/2) : int(len(_y)/4)*3],
                'b', linewidth=0.5)
    pyplot.grid(True)

    pyplot.subplot(4, 1, 4)
    pyplot.plot(_x[int(len(_x)/4)*3 :],
                _y[int(len(_y)/4)*3 :],
                'b', linewidth=0.5)
    pyplot.grid(True)

    pyplot.show()


def zoom_visualisation(_x=[], _y=[]):
    fig = figure()

    print("enter the initial visible border of the X axis if minimum time value is:", _x[0], 
	  "and maximum value is: ", _x[len(_x) - 1])
    x_border = int(input())

    if x_border > _x[0]:
        if x_border > len(_x):
            x_border = len(_x)
        ax = fig.add_subplot(111, xlim=(_x[0], x_border), ylim=(0, 200), autoscale_on=False)

        ax.plot(_x, _y, color='b')
        ax.grid(True)

        scale = 1.1
        zp = ZoomPan()
        figZoom = zp.zoom_factory(ax, base_scale=scale)
        figPan = zp.pan_factory(ax)
        show()

# for data storage
target_value = []
time = []

# read data with primary data processing
print("enter the file name:\n")
file_name = input()
work_file = open(file_name, 'r')
for line in work_file.readlines():
    line = line.replace("\n", "")
    line_parts = line.split(';')
    if int(float(line_parts[2]) > 0):
        time.append(int(line_parts[1]))
        target_value.append(float(line_parts[2]))
print("data have been read\n")

print("choose the mode of data visualisation\n"
      "1 - with scaling (1 graph)\n"
      "2 - 2 graphs\n"
      "3 - 4 graphs\n"
      "4 - without mode (1 graph)\n"
      "mode: ")
mode_visualisation = input()

if mode_visualisation == '1':
    zoom_visualisation(time, target_value)
elif mode_visualisation == '2':
    double_visualisation(time, target_value)
elif mode_visualisation == '3':
    quad_visualisation(time, target_value)
elif mode_visualisation == '4':
    clean_visualisation(time, target_value)
