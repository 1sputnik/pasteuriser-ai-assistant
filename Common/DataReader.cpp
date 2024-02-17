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

	/* встретил момент:
	* OCDF состоит из полей short, llint и double, но...
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
		std::cout << "¬ведите им€ файла дл€ загрузки из него данных: ";
		std::getline(std::cin, load_file_path);
		for (size_t k = load_file_path.find('\"'); k != load_file_path.npos; k = load_file_path.find('\"', k))
			load_file_path.erase(k, 1);


		long long data_size;
		std::cout << "\n—колько данных необходимо загрузить (введите 0, если надо загрузить все данные): ";
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

vector<TableDataFormat> load_TDF_data(string file_name, size_t size) {
	std::ifstream load_file;
	TableDataFormat temp_data;
	size_t file_size;
	file_size = check_quantity_data_lines_in_file(load_file, file_name);
	if (file_size < size || size <= 0)
		size = file_size;
	vector<TableDataFormat> data(size);
	load_file.open(file_name);
	for (size_t i = 0; !load_file.eof() && i < size; i++) {
		char buffer_char;
		load_file >> temp_data.time >> buffer_char >> temp_data.cid_1_value >> buffer_char >> temp_data.cid_2_value >> buffer_char >>
			temp_data.cid_3_value >> buffer_char >> temp_data.cid_4_value >> buffer_char >> temp_data.cid_5_value >> buffer_char >> temp_data.cid_6_value;
		data.at(i) = temp_data;
	}
	load_file.close();
	return data;
}

vector<TableDataFormat> binload_TDF_data(string file_name, size_t size) {
	std::ifstream load_file;
	load_file.open(file_name, std::ios::binary);

	load_file.seekg(0, load_file.end);
	long long file_size = load_file.tellg();
	load_file.seekg(0, load_file.beg);

	/* встретил момент:
	* OCDF состоит из полей short, llint и double, но...
	* sizeof(OCDF) != sizeof(short) + sizeof(long long int) + sizeof(double)
	*/

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
		std::cout << "¬ведите им€ файла дл€ загрузки из него данных: ";
		std::getline(std::cin, load_file_path);
		for (size_t k = load_file_path.find('\"'); k != load_file_path.npos; k = load_file_path.find('\"', k))
			load_file_path.erase(k, 1);

		long long data_size;
		std::cout << "\n—колько данных необходимо загрузить (введите 0, если надо загрузить все данные): ";
		if (!enter_int_numeric(data_size))
			continue;

		try {
			if (extractLastNChars(load_file_path, 4) == ".bin") {
				data = binload_TDF_data(load_file_path, data_size);
				break;
			}
			else if (check_OCDF_in_file(load_file_path)) {
				data = load_TDF_data(load_file_path, data_size);
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