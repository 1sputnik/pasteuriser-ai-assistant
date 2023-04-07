#pragma once
#include <fstream>
#include <iostream>
#include <string>
#include <ctime>
#include <typeinfo>

struct DATA;
class DataVector;

using std::string;

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

class DataVector
{
private:
	DATA* array;
	size_t capacity;
	size_t size_;

public:
	DataVector();
	DataVector(std::string FileName);
	DataVector(size_t size_of);
	void MemAddForFile(std::string FileName);
	void MemAdd();
	int MemFromFile(std::string FileName);
	void push_back(const DATA& value);
	DATA& operator[](size_t index);
	void inc_size();
	size_t size();
	void resize(size_t new_size);
	size_t count_distance();
	void clear();
	DataVector& operator=(DataVector& other);
	~DataVector();
};

void load_data(DataVector& data, string file_name);
void dump_data(DataVector& data, string file_name);
