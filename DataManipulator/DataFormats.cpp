#include "DataFormats.h"

vector<string> split_string(string str, char descriptor) {
	vector<string> splited_string;
	string buffer_string = "";
	for (size_t i = 0; i < str.length(); i++) {
		if (str[i] == descriptor) {
			splited_string.push_back(buffer_string);
			buffer_string = "";
		}
		else {
			buffer_string += str[i];
		}
	}
	if (buffer_string.size() != 0) {
		splited_string.push_back(buffer_string);
	}
	return splited_string;
}
bool is_double_numeric(string str) {
	vector<char> numerics{ '1', '2', '3', '4', '5', '6', '7', '8', '9', '.', '0'};
	for (size_t i = 0; i < str.size(); i++) {
		if (find(numerics.begin(), numerics.end(), str[i]) == numerics.end())
			return false;
	}
	return true;
}
bool is_int_numeric(string str) {
	vector<char> numerics{ '1', '2', '3', '4', '5', '6', '7', '8', '9', '0' };
	for (size_t i = 0; i < str.size(); i++) {
		if (find(numerics.begin(), numerics.end(), str[i]) == numerics.end())
			return false;
	}
	return true;
}
bool have_promlems_with_opening_file(std::ifstream& file, string file_name) {
	try {
		file.open(file_name);
	}
	catch (...) {
		if (file.is_open())
			file.close();
		return true;
	}
	if (!file.is_open()) {
		return true;
	}
	return false;
}
bool have_promlems_with_reading_data(std::ifstream& file, string& str) {
	try {
		std::getline(file, str);
	}
	catch (...) {
		file.close();
		return true;
	}
	if (str == "") {
		file.close();
		return true;
	}
	return false;
}
size_t check_quantity_data_lines_in_file(std::ifstream& file, string file_name) {
	file.open(file_name);
	string buffer_string;
	size_t new_size = 0;
	while (!file.eof()) {
		std::getline(file, buffer_string);
		if (buffer_string.length() > 2)
			new_size++;
	}
	file.close();
	return new_size;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////// OCDF ////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

OneCIDDataFormat::OneCIDDataFormat() {
	this->cid = 0;
	this->time = 0;
	this->value = 0;
}
OneCIDDataFormat::OneCIDDataFormat(const OneCIDDataFormat& data) {
	this->cid = data.cid;
	this->time = data.time;
	this->value = data.value;
}
OneCIDDataFormat::OneCIDDataFormat(short cid, llint time, double value) {
	this->cid = cid;
	this->time = time;
	this->value = value;
}
std::ostream& operator<<(std::ostream& os, const OneCIDDataFormat& data) {
	os << data.cid << ";" << data.time << ";" << data.value;
	return os;
}
std::istream& operator>>(std::istream& is, OneCIDDataFormat& data) {
	char buffer_char;
	is >> data.cid >> buffer_char >> data.time >> buffer_char >> data.value;
	return is;
}
void OneCIDDataFormat::operator=(OneCIDDataFormat& data) {
	this->cid = data.cid;
	this->time = data.time;
	this->value = data.value;
}
vector<OneCIDDataFormat> load_data(vector<OneCIDDataFormat> data, string file_name, size_t size) {
	std::ifstream load_file;
	OneCIDDataFormat temp_data;
	data.clear();
	if (size == 0) {
		size = check_quantity_data_lines_in_file(load_file, file_name);
	}
	data.resize(size);
	load_file.open(file_name);
	for (size_t i = 0; !load_file.eof() && i < size; i++) {
		load_file >> temp_data;
		data.at(i) = temp_data;
	}
	load_file.close();
	return data;
}
//void dump_data(OneCIDDataFormat*& data, string file_name);
//
//void dump_data(DataVector& data, string file_name) {
//	std::ofstream dump_file;
//	dump_file.open(file_name);
//	for (int i = 0; i < data.get_size(); i++)
//		dump_file << data[i];
//	dump_file.close();
//}
bool check_OCDF_in_file(string file_name) {
	std::ifstream file;
	if (have_promlems_with_opening_file(file, file_name)){
		return false;
	}
	string test_str;
	if (have_promlems_with_reading_data(file, test_str)) {
		return false;
	}
	file.close();
	vector<string> splited_test_str = split_string(test_str, ';');
	if (splited_test_str.size() != 3) {
		return false;
	}
	for (size_t i = 0; i < splited_test_str.size(); i++) {
		if (!is_double_numeric(splited_test_str[i])) {
			return false;
		}
	}
	return true;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////// TDF ////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

TableDataFormat::TableDataFormat() {
	this->time = 0;
	this->cid_1_value = 0;
	this->cid_2_value = 0;
	this->cid_3_value = 0;
	this->cid_4_value = 0;
	this->cid_5_value = 0;
	this->cid_6_value = 0;
}
TableDataFormat::TableDataFormat(const TableDataFormat& data) {
	this->time = data.time;
	this->cid_1_value = data.cid_1_value;
	this->cid_2_value = data.cid_2_value;
	this->cid_3_value = data.cid_3_value;
	this->cid_4_value = data.cid_4_value;
	this->cid_5_value = data.cid_5_value;
	this->cid_6_value = data.cid_6_value;
}
TableDataFormat::TableDataFormat(llint time, double cid_1_value, double cid_2_value, double cid_3_value, double cid_4_value, double cid_5_value, double cid_6_value) {
	this->time = time;
	this->cid_1_value = cid_1_value;
	this->cid_2_value = cid_2_value;
	this->cid_3_value = cid_3_value;
	this->cid_4_value = cid_4_value;
	this->cid_5_value = cid_5_value;
	this->cid_6_value = cid_6_value;
}
std::ostream& operator<<(std::ostream& os, const TableDataFormat& data) {
	os << data.time << ";" << data.cid_1_value << ";" << data.cid_2_value << ";" << data.cid_3_value << ";" << data.cid_4_value << ";" << data.cid_5_value << ";" << data.cid_6_value;
	return os;
}
std::istream& operator>>(std::istream& is, TableDataFormat& data) {
	char buffer_char;
	is >> data.time >> buffer_char >> data.cid_1_value >> buffer_char >> data.cid_2_value >> buffer_char >> data.cid_3_value
		>> buffer_char >> data.cid_4_value >> buffer_char >> data.cid_5_value >> buffer_char >> data.cid_6_value;
	return is;
}
void TableDataFormat::operator=(TableDataFormat& data) {
	this->time = data.time;
	this->cid_1_value = data.cid_1_value;
	this->cid_2_value = data.cid_2_value;
	this->cid_3_value = data.cid_3_value;
	this->cid_4_value = data.cid_4_value;
	this->cid_5_value = data.cid_5_value;
	this->cid_6_value = data.cid_6_value;
}
