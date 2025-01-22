#include "DataReader.h"

size_t check_quantity_data_lines_in_file(std::string file_name) {
	std::ifstream file;
	file.open(file_name);
	std::string buffer_string;
	size_t new_size = 0;
	while (!file.eof()) {
		std::getline(file, buffer_string);
		if (buffer_string.length() > 4)
			new_size++;
	}
	file.close();
	return new_size;
}

vector<OneCIDDataFormat> load_OCDF_data(string file_name, size_t size) {
	OneCIDDataFormat temp_data;
	size_t temp_size = check_quantity_data_lines_in_file(file_name);
	if (size == 0 || size > temp_size) {
		size = temp_size;
	}
	vector<OneCIDDataFormat> data(size);
	std::ifstream load_file;
	load_file.open(file_name, std::ios::in);
	string line;
	vector<string> values;
	double temp_val;
	try {
		for (size_t i = 0; !load_file.eof() && i < size; i++) {
			getline(load_file, line);
			if (is_target_format_data(line)) {
				values = split_string(line, ';');
			}
			else
				throw std::invalid_argument("Invalid data format");
			if (values.size() == 3) {
				if (!string_to_integer(values[0], temp_data.time) ||
					!string_to_integer(values[1], temp_data.time) ||
					!string_to_double(values[2], temp_val))
					throw std::invalid_argument("Invalid data format");
				temp_data.value = temp_val;
				data.at(i) = temp_data;
			}
			else
				throw std::invalid_argument("Invalid data format");
		}
	}
	catch (...) {
		data.clear();
	}
	values.clear();
	load_file.close();
	return data;
}

vector<OneCIDDataFormat> binload_OCDF_data(string file_name, size_t size) {
	std::ifstream load_file;
	load_file.open(file_name, std::ios::binary | std::ios::in);

	load_file.seekg(0, load_file.end);
	long long file_size = load_file.tellg();
	load_file.seekg(0, load_file.beg);

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

	try {
		for (size_t i = 0; i < data.size(); i++) {
			load_file.read((char*)&data[i].cid, sizeof(data[i].cid));
			load_file.read((char*)&data[i].time, sizeof(data[i].time));
			load_file.read((char*)&data[i].value, sizeof(data[i].value));
		}
	}
	catch (...) {
		data.clear();
		throw std::invalid_argument("Invalid data format");
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
		std::cout << "Введите имя файла для загрузки из него данных: ";
		std::getline(std::cin, load_file_path);
		for (size_t k = load_file_path.find('\"'); k != load_file_path.npos; k = load_file_path.find('\"', k))
			load_file_path.erase(k, 1);


		int data_size;
		std::cout << "\nСколько данных необходимо загрузить (введите 0, если надо загрузить все данные): ";
		string answer;
		getline(cin, answer);
		if (!string_to_integer(answer, data_size)) {
			msg_warning("\nОшибка ввода данных! Введённое значение не является целым числом или число отрицательное!\n");
			continue;
		}

		try {
			if (extractLastNChars(load_file_path, 4) == ".bin") {
				data = binload_OCDF_data(load_file_path, data_size);
				break;
			}
			else {
				data = load_OCDF_data(load_file_path, data_size);
				break;
			}
		}
		catch (...) {
			msg_warning("\nОшибка загрузки данных! Неверный формат данных или невозможно считать данные!\n");
			data.clear();
		}
	}

	return data;
}

vector<TableDataFormat> load_TDF_data(string file_name, size_t size) {
	size_t file_size = check_quantity_data_lines_in_file(file_name);
	if (size == 0 || size > file_size) {
		size = file_size;
	}
	vector<TableDataFormat> data(size);
	TableDataFormat temp_data;
	std::ifstream load_file;
	load_file.open(file_name, std::ios::in);
	string line;
	vector<string> values;
	try {
		for (size_t i = 0; !load_file.eof() && i < size; i++) {
			getline(load_file, line);
			if (is_target_format_data(line)) {
				values = split_string(line, ';');
			}
			else
				throw std::invalid_argument("Invalid data format");
			if (values.size() == 7) {
				temp_data.time = stoi(values[0]);
				temp_data.cid_1_value = stof(values[1]);
				temp_data.cid_2_value = stof(values[2]);
				temp_data.cid_3_value = stof(values[3]);
				temp_data.cid_4_value = stof(values[4]);
				temp_data.cid_5_value = stof(values[5]);
				temp_data.cid_6_value = stof(values[6]);
				data.at(i) = temp_data;
			}
			else
				throw std::invalid_argument("Invalid data format");
		}
	}
	catch (...) {
		data.clear();
	}
	load_file.close();
	return data;
}

vector<TableDataFormat> binload_TDF_data(string file_name, size_t size) {
	std::ifstream load_file;
	load_file.open(file_name, std::ios::binary | std::ios::in);

	load_file.seekg(0, load_file.end);
	long long file_size = load_file.tellg();
	load_file.seekg(0, load_file.beg);

	TableDataFormat temp;
	long size_of_TDF = sizeof(temp.time) + 6.0 * sizeof(temp.cid_1_value);
	if (file_size % size_of_TDF != 0 || file_size == 0) {
		load_file.close();
		throw std::invalid_argument("Invalid data format");
	}

	if (size == 0 || size > file_size / size_of_TDF) {
		size = file_size / size_of_TDF;
	}

	vector<TableDataFormat> data(size);

	for (size_t i = 0; i < data.size(); i++) {
		load_file.read((char*)&data[i].time, sizeof(data[i].time));
		load_file.read((char*)&data[i].cid_1_value, sizeof(data[i].cid_1_value));
		load_file.read((char*)&data[i].cid_2_value, sizeof(data[i].cid_2_value));
		load_file.read((char*)&data[i].cid_3_value, sizeof(data[i].cid_3_value));
		load_file.read((char*)&data[i].cid_4_value, sizeof(data[i].cid_4_value));
		load_file.read((char*)&data[i].cid_5_value, sizeof(data[i].cid_5_value));
		load_file.read((char*)&data[i].cid_6_value, sizeof(data[i].cid_6_value));
	}

	load_file.close();
	return data;
}

vector<TableDataFormat> read_TDF_file(string special_msg) {
	vector<TDF> data;

	while (true) {
		system("cls");

		std::cout << special_msg;

		string load_file_path;
		std::cout << "Введите имя файла для загрузки из него данных: ";
		std::getline(std::cin, load_file_path);
		for (size_t k = load_file_path.find('\"'); k != load_file_path.npos; k = load_file_path.find('\"', k))
			load_file_path.erase(k, 1);

		int data_size;
		std::cout << "\nСколько данных необходимо загрузить (введите 0, если надо загрузить все данные): ";
		string answer;
		getline(cin, answer);
		if (!string_to_integer(answer, data_size)) {
			msg_warning("\nОшибка ввода данных! Введённое значение не является целым числом или число отрицательное!\n");
			continue;
		}

		try {
			if (extractLastNChars(load_file_path, 4) == ".bin") {
				data = binload_TDF_data(load_file_path, data_size);
				break;
			}
			else {
				data = load_TDF_data(load_file_path, data_size);
				break;
			}
		}
		catch (...) {
			msg_warning("\nОшибка загрузки данных! Неверный формат данных или невозможно считать данные!\n");
			data.clear();
		}
	}

	return data;
}