def copy_list(out_list=[], in_list=[], left_border=0, right_border=-1):
    in_list.clear()
    if right_border == -1:
        right_border = len(out_list)
    for i in range(left_border, right_border):
        in_list.append(out_list[i])


def count_appr_sum(d=[]):
    appr_sum = 0.0
    for i in range(0, len(d)):
        appr_sum += d[i]
        if i == len(d) - 1:
            appr_sum /= float(len(d))
    return appr_sum


def count_right_neighbor_approximation(data=[], time=[]):
    temp_data = []
    temp_time = []
    for iterator in range(0, len(data), 2):
        if iterator == len(data) - 1:
            temp_data.append(data[iterator])
            temp_time.append(time[iterator])
        else:
            temp_data.append((data[iterator] + data[iterator + 1]) / 2.0)
            temp_time.append((time[iterator] + time[iterator + 1]) / 2.0)
    copy_list(temp_data, data)
    copy_list(temp_time, time)


def count_up_down_approximation(data=[], time=[]):
    temp_data = []
    temp_time = []
    iterator = 0
    while iterator <= len(data) - 1:
        temp_d = data[iterator]
        temp_t = time[iterator]
        temp_data.append(temp_d)
        temp_time.append(temp_t)
        if iterator != len(data) - 1 and data[iterator] < data[iterator + 1]:
            while True:
                if iterator != len(data) - 1 and data[iterator] < data[iterator + 1]:
                    temp_d = data[iterator + 1]
                    temp_t = time[iterator + 1]
                    iterator += 1
                else:
                    temp_data.append(temp_d)
                    temp_time.append(temp_t)
                    break
        elif iterator != len(data) - 1 and data[iterator] > data[iterator + 1]:
            while True:
                if iterator != len(data) - 1 and data[iterator] > data[iterator + 1]:
                    temp_d = data[iterator + 1]
                    temp_t = time[iterator + 1]
                    iterator += 1
                else:
                    temp_data.append(temp_d)
                    temp_time.append(temp_t)
                    break
        iterator += 1
    copy_list(temp_data, data)
    copy_list(temp_time, time)


def count_critical_percent_approximation(data=[], time=[], c_p=10.0):
    temp_data = []
    temp_time = []
    iterator = 0
    temp_d = []
    temp_t = []
    critical_percent = c_p
    len_data = len(data) - 1
    while iterator <= len_data:
        temp_d.clear()
        temp_d.append(data[iterator])
        temp_t.clear()
        temp_t.append(time[iterator])
        appr_sum = data[iterator]
        appr_percent = appr_sum / critical_percent
        while True:
            if iterator != len_data:
                if iterator != len_data and data[iterator] < data[iterator + 1]:
                    if data[iterator] + appr_percent >= data[iterator + 1]:
                        temp_d.append(data[iterator + 1])
                        temp_t.append(time[iterator + 1])
                        appr_sum = count_appr_sum(temp_d)
                        appr_percent = appr_sum / critical_percent
                        iterator += 1
                        continue
                elif iterator != len_data and data[iterator] >= data[iterator + 1]:
                    if data[iterator] - appr_percent <= data[iterator + 1]:
                        temp_d.append(data[iterator + 1])
                        temp_t.append(time[iterator + 1])
                        appr_sum = count_appr_sum(temp_d)
                        appr_percent = appr_sum / critical_percent
                        iterator += 1
                        continue
                temp_data.append(appr_sum)
                temp_time.append(temp_t[len(temp_t) - 1])
                iterator += 1
                break
            else:
                temp_data.append(appr_sum)
                temp_time.append(temp_t[len(temp_t) - 1])
                iterator += 1
                break
    copy_list(temp_data, data)
    copy_list(temp_time, time)


def cut_right_border(data=[], time=[]):
    temp_data = []
    temp_time = []
    print("enter the border position, if the quantity of elements is ", len(data))
    position = int(input())
    if len(data) > position > 0:
        copy_list(data, temp_data, 0, position)
        copy_list(time, temp_time, 0, position)
    copy_list(temp_data, data)
    copy_list(temp_time, time)


def cut_left_border(data=None, time=None):
    temp_data = []
    temp_time = []
    print("enter the border position, if the quantity of elements is ", len(data))
    position = int(input())
    if len(data) > position > 0:
        copy_list(data, temp_data, position, len(data))
        copy_list(time, temp_time, position, len(time))
    copy_list(temp_data, data)
    copy_list(temp_time, time)


def cut_range(data=[], time=[]):
    temp_data = []
    temp_time = []
    print("enter the left border position, if the quantity of elements is ", len(data))
    left_position = int(input())
    print("enter the right border position:")
    right_position = int(input())
    if len(data) > left_position > 0 and len(data) > right_position > 0 and left_position < right_position:
        copy_list(data, temp_data, left_position, right_position)
        copy_list(time, temp_time, left_position, right_position)
    copy_list(temp_data, data)
    copy_list(temp_time, time)


def find_max_element_before_border(time=[], border=0, i=0):
    if i < 0:
        i = 0
    while i < len(time):
        if time[i] > border:
            i -= 1
            break
        i = i + 1
    if i == len(time):
        return -1
    else:
        return i


def right_data_per_time(data=[], time=[]):
    temp_data = []
    temp_time = []
    print("enter the length of time intervals (0 or -1 for auto): ")
    delta = int(input())
    if delta <= 0:
        delta = int((time[0] + time[len(time) - 1]) / len(time))
        if delta == -1:
            for i in range(1, len(time)):
                if (delta > (time[i] - time[i - 1])):
                    delta = int(time[i] - time[i - 1])
    temp_data.append(data[0])
    intervals = int((time[len(time) - 1] - time[0]) / delta)
    for i in range(0, intervals):
        temp_time.append(time[0] + (i * delta))
    down = 0
    up = 0
    for i in range(1, len(temp_time)):
        down = find_max_element_before_border(time, temp_time[i], down)
        up = down + 1
        if time[down] == temp_time[i]:
            temp_data.append(data[down])
            continue
        if time[up] == temp_time[i]:
            temp_data.append(data[up])
            continue
        if down == len(time) - 1:
            temp_data.append(data[down])
            break
        tega = (temp_time[i] - time[down]) / ((temp_time[i] - time[down]) + (time[up] - temp_time[i]))
        unta = 1 - tega
        temp_data.append(unta * data[down] + tega * data[up])
    copy_list(temp_data, data)
    copy_list(temp_time, time)


def right_null_start(time=[]):
    downer = time[0]
    for i in range(len(time)):
        time[i] = time[i] - downer


data = []
time = []
cid = ""

print("enter the work file name:\n")
work_file_name = input() # D:\work\data\data_1_cid_1.csv
work_file = open(work_file_name, 'r')
for line in work_file.readlines():
    line = line.replace("\n", "")
    line_parts = line.split(';')
    if int(float(line_parts[2]) > 0):
        cid = line_parts[0]
        time.append(int(line_parts[1]))
        data.append(float(line_parts[2]))
print("data have been read\n")

print("choose the mode of data processing:\n"
      "1 - \"right neighbor\" approximation\n"
      "2 - \"up-down\" approximation\n"
      "3 - \"critical percent\" approximation\n"
      "4 - cut right border of data\n"
      "5 - cut left border of data\n"
      "6 - cut range of data\n"
      "7 - right data per time\n"
      "8 - make start at 0 per time\n"
      "your action: ")
action = input()

if action == '1':
    count_right_neighbor_approximation(data, time)
elif action == '2':
    count_up_down_approximation(data, time)
elif action == '3':
    count_critical_percent_approximation(data, time)
elif action == '4':
    cut_right_border(data, time)
elif action == '5':
    cut_left_border(data, time)
elif action == '6':
    cut_range(data, time)
elif action == '7':
    right_data_per_time(data, time)
elif action == '8':
    right_null_start(time)

print("enter the result file name:\n")
result_file_name = input()
result_file = open(result_file_name, 'w')
for i in range(0, len(data)):
    line = cid + ';' + str(int(time[i])) + ';' + str(data[i]) + '\n'
    result_file.write(line)
