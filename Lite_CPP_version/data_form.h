#pragma once
#include <fstream>
#include <iostream>
#include <string>
#include <ctime>
#include <typeinfo>
#include <algorithm>

using std::string;

struct DATA {
    __int8 cid;
    unsigned int time;
    double value;

    void operator=(const DATA& data);
    bool operator>(const DATA& data);
    bool operator<(const DATA& data);

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
    size_t size;

    void add_memory();

    int get_records_count_in_file(std::string file_name); 

    size_t get_lines_count_in_file(std::ifstream& file, char line_separator); 

    void inc_size();

public:
	DataVector();
	DataVector(std::string file_name);
    DataVector(size_t size_of);

    DATA get_max_element();
    
    DATA get_min_element();

    double get_average_value();

	void push_back(const DATA& value);

    size_t get_size();

    void resize(size_t new_size);

    size_t count_distance();
    
    void clear();
    
    DATA& operator[](size_t index);
    
    DataVector& operator=(DataVector& other);
    
    ~DataVector();
};

void load_data(DataVector& data, string file_name);
void dump_data(DataVector& data, string file_name);