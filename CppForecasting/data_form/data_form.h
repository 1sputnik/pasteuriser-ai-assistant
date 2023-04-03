#pragma once
#include <fstream>
#include <vector>
#include <iostream>
#include <string>

struct DATA;
class DataVector;

using std::string;
using std::vector;

typedef DataVector data_vector;

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
	DataVector() {//обычный конструктор
		array = new DATA[1];
		capacity = 1;
	}

	DataVector(std::string FileName) { //конструктор, в помощью которого можно сразу выделить память под файл
		size_t size_of_file = MemFromFile(FileName);
		array = new DATA[size_of_file];
		capacity = size_of_file;
	}
	DataVector(size_t size_of) { //конструктор, в помощью которого можно сразу выделить память под файл
		array = new DATA[size_of];
		capacity = size_of;
	}
	void MemAddForFile(std::string FileName) {//выделение памяти под файл как метод класса
		delete[] array;
		size_ = 0;
		size_t size_of_file = MemFromFile(FileName);
		array = new DATA[size_of_file];
		capacity = size_of_file;
	}

	void MemAdd() {//выделение доп памяти при обычной работе
		capacity *= 2;
		DATA* tmp = array;
		array = new DATA[capacity];
		for (size_t i = 0; i < size_; ++i) array[i] = tmp[i];
		delete[] tmp;
	}

	int MemFromFile(std::string FileName) { // метод который считывает количество переходов на новую строку и возвращает их количество+1 для выделения памяти
		std::ifstream DATAFile(FileName, std::ios::binary);
		int PAR = 1;
		if (DATAFile.is_open())
		{
			std::string symb;
			while (getline(DATAFile, symb, '\n'))
				PAR++;
		}
		DATAFile.close();
		return PAR;
	}

	void push_back(const DATA& value) { // добавление в конец
		if (size_ >= capacity) MemAdd();
		array[size_++] = value;
	}
	DATA* begin() {
		return &array[0];
	}

	const DATA* begin() const {
		return &array[0];
	}

	DATA* end() {
		return &array[size_];
	}

	const DATA* end() const {
		return &array[size_];
	}

	/*void DeleteElement(size_t index) {
		for (size_t i = index + 1; i < size_; ++i) {
			array[i - 1] = array[i];
		}
		--size_;
	}
	size_t getSize() {
		return size_;
	}*/
	DATA& operator[](size_t index) { // операция получения по индексу
		if (index < size_)
			return array[index];
		else {
			std::cerr << "Error dont have in massive DATA[" << index << "]\n";
			std::exit(1);
		}
	}
	DATA& operator[](size_t index) const { // операция получения по индексу
		if (index < size_)
			return array[index];
		else {
			std::cerr << "Error dont have in massive DATA[" << index << "]\n";
			std::exit(1);
		}
	}
	size_t size() const {
		return size_;
	}
	size_t size() {
		return this->size_;
	}
	void clear() {
		delete[] array;
		size_ = 0;
		array = new DATA[capacity];
	}
	DataVector& operator=(DataVector& other) {
		if (this != &other) {
			delete[] array;
			array = new DATA[other.capacity];
			for (size_t i = 0; i < other.size_; ++i) array[i] = other.array[i];
			size_ = other.size_;
			capacity = other.capacity;
		}
		return *this;
	}

	DataVector& operator=(DataVector&& other) noexcept {
		if (this != &other) {
			delete[] array;
			array = other.array;
			size_ = other.size_;
			capacity = other.capacity;
			other.array = nullptr;
			other.size_ = other.capacity = 0;
		}
		return *this;
	}
	~DataVector() {
		delete[] array;
	}
};


void load_data(data_vector& data, string file_name);
void dump_data(const data_vector& data, string file_name);
