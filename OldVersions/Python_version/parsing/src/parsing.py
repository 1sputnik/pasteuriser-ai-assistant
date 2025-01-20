print("enter the file name:\n")
file_name = input()
work_file = open(file_name, 'r')

file_name = file_name.replace('.csv', '')
first_cid_file = open(file_name + '_cid_1.csv', 'w')
second_cid_file = open(file_name + '_cid_2.csv', 'w')
third_cid_file = open(file_name + '_cid_3.csv', 'w')
fourth_cid_file = open(file_name + '_cid_4.csv', 'w')
fifth_cid_file = open(file_name + '_cid_5.csv', 'w')
sixth_cid_file = open(file_name + '_cid_6.csv', 'w')

for line in work_file.readlines():
    line_parts = line.split(';')
    if line_parts[0] == '1':
        first_cid_file.write(line)
    elif line_parts[0] == '2':
        second_cid_file.write(line)
    elif line_parts[0] == '3':
        third_cid_file.write(line)
    elif line_parts[0] == '4':
        fourth_cid_file.write(line)
    elif line_parts[0] == '5':
        fifth_cid_file.write(line)
    elif line_parts[0] == '6':
        sixth_cid_file.write(line)

print("\nWork has been done!\n")
