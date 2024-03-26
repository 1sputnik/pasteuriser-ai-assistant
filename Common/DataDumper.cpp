#include "DataDumper.h"

void dump_data(vector<OneCIDDataFormat>& data, string file_name) {
	std::ofstream dump_file;
	dump_file.open(file_name);
	for (int i = 0; i < data.size(); i++) {
		dump_file << data[i] << '\n';
	}
	dump_file.close();
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
