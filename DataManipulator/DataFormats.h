#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include "WARNING.h"

using std::vector;
using std::string;
using std::find;

typedef long long int llint;

class OneCIDDataFormat;
typedef OneCIDDataFormat OCDF;

class TableDataFormat;
typedef TableDataFormat TDF;

bool is_double_numeric(string str);
bool is_int_numeric(string str);

class OneCIDDataFormat {
public:
	short cid;
	llint time;
	double value;

	OneCIDDataFormat();
	OneCIDDataFormat(const OneCIDDataFormat& data);
	OneCIDDataFormat(short cid, llint time, double value);

	friend std::ostream& operator<<(std::ostream& os, const OneCIDDataFormat& data);
	friend std::istream& operator>>(std::istream& is, const OneCIDDataFormat& data);

	void operator=(OneCIDDataFormat& data);
};

vector<OneCIDDataFormat> load_data(vector<OneCIDDataFormat> data, string file_name, size_t size = 0);
//void dump_data(vector<OneCIDDataFormat>& data, string file_name);

bool check_OCDF_in_file(string file_name);


class TableDataFormat {
public:
	llint time;

	double cid_1_value;
	double cid_2_value;
	double cid_3_value;
	double cid_4_value;
	double cid_5_value;
	double cid_6_value;

	TableDataFormat();
	TableDataFormat(const TableDataFormat& data);
	TableDataFormat(llint time, double cid_1_value, double cid_2_value, double cid_3_value, double cid_4_value, double cid_5_value, double cid_6_value);

	friend std::ostream& operator<<(std::ostream& os, const TableDataFormat& data);
	friend std::istream& operator>>(std::istream& is, const TableDataFormat& data);

	void operator=(TableDataFormat& data);
};

//void load_data(TableDataFormat*& data, string file_name);
//void dump_data(TableDataFormat*& data, string file_name);
