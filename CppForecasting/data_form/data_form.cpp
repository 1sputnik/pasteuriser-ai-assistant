#include "data_form.h"

std::ostream& operator<<(std::ostream& os, const DATA& data) {
	os << data.cid << ";" << data.time << ";" << data.value << "\n";
	return os;
}
std::istream& operator>>(std::istream& is, DATA& data) {
	char buffer_char;
	is >> data.cid >> buffer_char >> data.time >> buffer_char >> data.value;
	return is;
}

void DATA::operator=(const DATA& data) {
	this->cid = data.cid;
	this->time = data.time;
	this->value = data.value;
}
DATA::DATA() {
	this->cid = __int8();
	this->time = unsigned int();
	this->value = float();
}
DATA::DATA(__int8 cid, unsigned int time, double value) {
	this->cid = cid;
	this->time = time;
	this->value = value;
}


void load_data(data_vector& data, string file_name) {
	std::ifstream work_file;
	work_file.open(file_name);
	DATA temp_datum;
	while (!work_file.eof()) {
		work_file >> temp_datum;
		data.push_back(temp_datum);
	}
}
void dump_data(const data_vector& data, string file_name) {
	std::ofstream result_file;
	result_file.open(file_name);
	for (auto datum : data)
		result_file << datum;
	result_file.close();
}

void load_data(data_list& data, string file_name) {
	std::ifstream work_file;
	work_file.open(file_name);
	DATA temp_datum;
	while (!work_file.eof()) {
		work_file >> temp_datum;
		data.push_back(temp_datum);
	}
}
void dump_data(const data_list& data, string file_name) {
	std::ofstream result_file;
	result_file.open(file_name);
	for (auto datum : data)
		result_file << datum;
	result_file.close();
}


void list_to_vector(data_list& l_data, data_vector& v_data) {
	std::copy(l_data.begin(), l_data.end(), std::back_inserter(v_data));
	l_data.clear();
}
void vector_to_list(data_vector& v_data, data_list l_data) {
	std::copy(v_data.begin(), v_data.end(), std::back_inserter(l_data));
	v_data.clear();
}
