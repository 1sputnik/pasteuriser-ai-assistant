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


class OneCIDDataFormat {
public:
	short cid;
	int time;
	float value;

	OneCIDDataFormat();
	OneCIDDataFormat(const OneCIDDataFormat& data);
	OneCIDDataFormat(short cid, int time, float value);

	friend std::ostream& operator<<(std::ostream& os, const OneCIDDataFormat& data);
	friend std::istream& operator>>(std::istream& is, const OneCIDDataFormat& data);

	void operator=(const OneCIDDataFormat& data);
};

class TableDataFormat {
public:
	int time;

	float cid_1_value;
	float cid_2_value;
	float cid_3_value;
	float cid_4_value;
	float cid_5_value;
	float cid_6_value;

	TableDataFormat();
	TableDataFormat(const TableDataFormat& data);
	TableDataFormat(int time, float cid_1_value, float cid_2_value, float cid_3_value, float cid_4_value, float cid_5_value, float cid_6_value);

	friend std::ostream& operator<<(std::ostream& os, const TableDataFormat& data);
	friend std::istream& operator>>(std::istream& is, const TableDataFormat& data);

	void operator=(const TableDataFormat& data);
};

//void load_data(TableDataFormat*& data, string file_name);