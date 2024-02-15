#include "DataReader.h"

vector<OneCIDDataFormat> load_OCDF_data(string file_name, size_t size) {
	std::ifstream load_file;
	OneCIDDataFormat temp_data;
	if (size == 0) {
		size = check_quantity_data_lines_in_file(load_file, file_name);
	}
	vector<OneCIDDataFormat> data(size);
	load_file.open(file_name);
	for (size_t i = 0; !load_file.eof() && i < size; i++) {
		//load_file >> temp_data;
		char buffer_char;
		load_file >> temp_data.cid >> buffer_char >> temp_data.time >> buffer_char >> temp_data.value;
		data.at(i) = temp_data;
	}
	load_file.close();
	return data;
}

vector<OneCIDDataFormat> binload_OCDF_data(string file_name, size_t size) {
	std::ifstream load_file;
	load_file.open(file_name, std::ios::binary);

	load_file.seekg(0, load_file.end);
	long long file_size = load_file.tellg();
	load_file.seekg(0, load_file.beg);

	/* �������� ������:
	* OCDF ������� �� ����� short, llint � double, ��...
	* sizeof(OCDF) != sizeof(short) + sizeof(long long int) + sizeof(double)
	*/

	OneCIDDataFormat temp;
	long size_of_OCDF = sizeof(temp.cid) + sizeof(temp.time) + sizeof(temp.value);
	if (file_size % size_of_OCDF != 0 || file_size == 0) {
		load_file.close();
		throw std::invalid_argument("Invalid data format");
	}

	if (size == 0 || size > file_size / size_of_OCDF) {
		size = file_size / size_of_OCDF;
	}

	vector<OneCIDDataFormat> data(size);

	for (size_t i = 0; i < data.size(); i++) {
		load_file.read((char*)&data[i].cid, sizeof(data[i].cid));
		load_file.read((char*)&data[i].time, sizeof(data[i].time));
		load_file.read((char*)&data[i].value, sizeof(data[i].value));
	}

	load_file.close();
	return data;
}

vector<OCDF> read_OCDF_file(string special_msg) {
	vector<OCDF> data;

	while (true) {
		system("cls");

		std::cout << special_msg;

		string load_file_path;
		std::cout << "������� ��� ����� ��� �������� �� ���� ������: ";
		std::getline(std::cin, load_file_path);
		for (size_t k = load_file_path.find('\"'); k != load_file_path.npos; k = load_file_path.find('\"', k))
			load_file_path.erase(k, 1);


		long long data_size;
		std::cout << "\n������� ������ ���������� ��������� (������� 0, ���� ���� ��������� ��� ������): ";
		if (!enter_int_numeric(data_size))
			continue;

		try {
			if (extractLastNChars(load_file_path, 4) == ".bin") {
				data = binload_OCDF_data(load_file_path, data_size);
				break;
			}
			else if (check_OCDF_in_file(load_file_path)) {
				data = load_OCDF_data(load_file_path, data_size);
				break;
			}
			else {
				check_data_format_warining();
			}
		}
		catch (...) {
			load_data_warning();
			data.clear();
		}
	}

	return data;
}
