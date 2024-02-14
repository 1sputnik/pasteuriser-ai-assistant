#include "DataFormats.h"

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
OneCIDDataFormat::OneCIDDataFormat(short cid, int time, float value) {
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
void OneCIDDataFormat::operator=(const OneCIDDataFormat& data) {
	this->cid = data.cid;
	this->time = data.time;
	this->value = data.value;
}
vector<OneCIDDataFormat> load_OCDF_data(string file_name, size_t size) {
	std::ifstream load_file;
	OneCIDDataFormat temp_data;
	if (size == 0) {
		size = check_quantity_data_lines_in_file(load_file, file_name);
	}
	vector<OneCIDDataFormat> data(size);
	load_file.open(file_name);
	for (size_t i = 0; !load_file.eof() && i < size; i++) {
		load_file >> temp_data;
		data.at(i) = temp_data;
	}
	load_file.close();
	return data;
}
void dump_data(vector<OneCIDDataFormat>& data, string file_name) {
	std::ofstream dump_file;
	dump_file.open(file_name);
	for (int i = 0; i < data.size(); i++) {
		dump_file << data[i] << '\n';
	}
	dump_file.close();
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
void bindump_data(vector<OneCIDDataFormat>& data, string file_name) {
	std::ofstream dump_file;
	dump_file.open(file_name, std::ios::binary);
	for (int i = 0; i < data.size(); i++) {
		dump_file.write((char*)&data[i].cid, sizeof(data[i].cid));
		dump_file.write((char*)&data[i].time, sizeof(data[i].time));
		dump_file.write((char*)&data[i].value, sizeof(data[i].value));
	}
	dump_file.close();
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
TableDataFormat::TableDataFormat(int time, float cid_1_value, float cid_2_value, float cid_3_value, float cid_4_value, float cid_5_value, float cid_6_value) {
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
void TableDataFormat::operator=(const TableDataFormat& data) {
	this->time = data.time;
	this->cid_1_value = data.cid_1_value;
	this->cid_2_value = data.cid_2_value;
	this->cid_3_value = data.cid_3_value;
	this->cid_4_value = data.cid_4_value;
	this->cid_5_value = data.cid_5_value;
	this->cid_6_value = data.cid_6_value;
}
void dump_data(vector<TableDataFormat>& data, string file_name) {
	std::ofstream dump_file;
	dump_file.open(file_name);
	for (int i = 0; i < data.size(); i++) {
		dump_file << data[i] << '\n';
	}
	dump_file.close();
}
void bindump_data(vector<TableDataFormat>& data, string file_name) {
	std::ofstream dump_file;
	dump_file.open(file_name, std::ios::binary);
	for (int i = 0; i < data.size(); i++) {
		dump_file.write((char*)&data[i].time, sizeof(data[i].time));
		dump_file.write((char*)&data[i].cid_1_value, sizeof(data[i].cid_1_value));
		dump_file.write((char*)&data[i].cid_2_value, sizeof(data[i].cid_2_value));
		dump_file.write((char*)&data[i].cid_3_value, sizeof(data[i].cid_3_value));
		dump_file.write((char*)&data[i].cid_4_value, sizeof(data[i].cid_4_value));
		dump_file.write((char*)&data[i].cid_5_value, sizeof(data[i].cid_5_value));
		dump_file.write((char*)&data[i].cid_6_value, sizeof(data[i].cid_6_value));
	}
	dump_file.close();
}