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

bool DATA::operator>(const DATA& data) {
	if (this->value > data.value)
		return true;
	else
		return false;
}

bool DATA::operator<(const DATA& data) {
	if (this->value < data.value)
		return true;
	else
		return false;
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


DataVector::DataVector() : DataVector(1) {} // обычный конструктор

DataVector::DataVector(std::string file_name) : DataVector(get_records_count_in_file(file_name)) {} // конструктор, с помощью которого можно сразу выделить память под файл

DataVector::DataVector(size_t size_of) { // конструктор, с помощью которого можно сразу выделить память под необходимое количество элементов
	array = new DATA[size_of];
	capacity = size_of;
	this->size = 0;
}

void DataVector::push_back(const DATA& value) { // добавление в конец
	inc_size();
	array[size-1] = value;
}

size_t DataVector::get_size() {
	return this->size;
}

void DataVector::resize(size_t new_size) {
	while (new_size > this->capacity) 
		add_memory();
	if (new_size > this->size) {
		for (size_t i = this->size; i < new_size; i++)
			this->array[i] = DATA();
	}
	this->size = new_size;
}

size_t DataVector::count_distance() {
	return this->array[2].time - this->array[1].time;
}

void DataVector::clear() {
	delete[] array;
	size = 0;
	array = new DATA[capacity];
}

DATA& DataVector::operator[](size_t index) { // операция получения по индексу
	if (index >= size) {
		std::cerr << "Error dont have in massive DATA[" << index << "]\n";
		std::exit(1);
	}

	return array[index];
}

DataVector& DataVector::operator=(DataVector& other) {
	if (this == &other)
		return *this;

	delete[] array;
	array = new DATA[other.capacity];
	for (size_t i = 0; i < other.size; ++i) 
		array[i] = other.array[i];
	size = other.size;
	capacity = other.capacity;
	return *this;
}

DataVector::~DataVector() {
	delete[] array;
}

DATA DataVector::get_max_element() {
	DATA max_element = this->array[0];
	for (size_t i = 1; i < this->size; i++) {
		if (max_element < this->array[i])
			max_element = this->array[i];
	}
	return max_element;
}

DATA DataVector::get_min_element() {
	DATA min_element = this->array[0];
	for (size_t i = 1; i < this->size; i++) {
		if (min_element > this->array[i])
			min_element = this->array[i];
	}
	return min_element;
}

double DataVector::get_average_value() {
	double average_sum = this->array[0].value;
	double average_value = 0;
	double average_iterator = 0;
	for (size_t i = 1; i < this->size; i++) {
		average_sum += this->array[i].value;
		if (i % 10 == 0) {
			average_value += average_sum / 10;
			average_iterator++;
			average_sum = 0;
		}
	}
	average_value /= average_iterator;
	return average_value;
}

void DataVector::inc_size() {
	this->size++;
	if (size >= capacity) 
		add_memory();
}

void DataVector::add_memory() { // выделение доп памяти при обычной работе
	capacity *= 2;
	DATA* tmp = array;
	array = new DATA[capacity];
	for (size_t i = 0; i < size; ++i) 
		array[i] = tmp[i];
	delete[] tmp;
}

int DataVector::get_records_count_in_file(std::string file_name) {
	std::ifstream DATAFile(file_name, std::ios::binary);
	
	if (DATAFile.is_open())
	{
		char line_separator;
		while (DATAFile.get(line_separator)) { // идея следующая: из потока считываются символы, после чего передаются в отдельную функцию, где считается количество
			if (line_separator == '\r' || line_separator == '\n')
				return get_lines_count_in_file(DATAFile, line_separator);
		}	
	}

	std::cerr << "Error stream not found, or not have separators in file\n"; // вывод ошибки на всякий случай
	std::exit(1);
}

size_t DataVector::get_lines_count_in_file(std::ifstream& file, char line_separator) { // здесь просто считываются символы и выводится их число, символ разделителя зависит от того, какой передали
	file.seekg(0);
	size_t lines_count = 1;
	std::string file_line;

	while (getline(file, file_line, line_separator))
		lines_count++;

	file.close();
	return lines_count;
}

void load_data(DataVector& data, string file_name) {
	std::ifstream load_file;
	load_file.open(file_name);
	DATA temp_data;
	while (!load_file.eof()) {
		load_file >> temp_data;
		data.push_back(temp_data);
	}
}

void dump_data(DataVector& data, string file_name) {
	std::ofstream dump_file;
	dump_file.open(file_name);
	for (int i = 0; i < data.get_size(); i++)
		dump_file << data[i];
	dump_file.close();
}