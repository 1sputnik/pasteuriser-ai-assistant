#pragma once
#include <fstream>
#include <vector>
#include <list>
#include <algorithm>

struct DATA;

using std::string;
using std::vector;
using std::list;

typedef vector<DATA> data_vector;
typedef list<DATA> data_list;

struct DATA {
	__int8 cid;
	unsigned int time;
	double value;

	void operator=(const DATA& data);

	friend std::ostream& operator<<(std::ostream& os, const DATA& data);
	friend std::istream& operator>>(std::istream& is, const DATA& data);

	DATA();
	DATA(__int8 cid, unsigned int time, double value);
};


void load_data(data_vector& data, string file_name);
void dump_data(const data_vector& data, string file_name);

void load_data(data_list& data, string file_name);
void dump_data(const data_list& data, string file_name);


void list_to_vector(data_list& l_data, data_vector& v_data);
void vector_to_list(data_vector& v_data, data_list l_data);
