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

DataVector::DataVector() { //îáû÷íûé êîíñòðóêòîð
	array = new DATA[1];
	capacity = 1;
}
DataVector::DataVector(std::string FileName) { //êîíñòðóêòîð, â ïîìîùüþ êîòîðîãî ìîæíî ñðàçó âûäåëèòü ïàìÿòü ïîä ôàéë
	size_t size_of_file = MemFromFile(FileName);
	array = new DATA[size_of_file];
	capacity = size_of_file;
}
DataVector::DataVector(size_t size_of) { //êîíñòðóêòîð, â ïîìîùüþ êîòîðîãî ìîæíî ñðàçó âûäåëèòü ïàìÿòü ïîä ôàéë
	array = new DATA[size_of];
	capacity = size_of;
}


void DataVector::MemAddForFile(std::string FileName) {//âûäåëåíèå ïàìÿòè ïîä ôàéë êàê ìåòîä êëàññà
	delete[] array;
	size_ = 0;
	size_t size_of_file = MemFromFile(FileName);
	array = new DATA[size_of_file];
	capacity = size_of_file;
}
void DataVector::MemAdd() {//âûäåëåíèå äîï ïàìÿòè ïðè îáû÷íîé ðàáîòå
	capacity *= 2;
	DATA* tmp = array;
	array = new DATA[capacity];
	for (size_t i = 0; i < size_; ++i) array[i] = tmp[i];
	delete[] tmp;
}
void DataVector::MemNAdd(size_t add_size) {//âûäåëåíèå äîï ïàìÿòè ïðè îáû÷íîé ðàáîòå
	capacity += add_size;
	DATA* tmp = array;
	array = new DATA[capacity];
	for (size_t i = 0; i < size_; ++i) array[i] = tmp[i];
	delete[] tmp;
}



int DataVector::MemFromFile(std::string FileName) { // ìåòîä êîòîðûé ñ÷èòûâàåò êîëè÷åñòâî ïåðåõîäîâ íà íîâóþ ñòðîêó è âîçâðàùàåò èõ êîëè÷åñòâî+1 äëÿ âûäåëåíèÿ ïàìÿòè
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
void DataVector::push_back(const DATA& value) { // äîáàâëåíèå â êîíåö
	if (size_ >= capacity) MemAdd();
	array[size_++] = value;
}
DATA& DataVector::operator[](size_t index) { // îïåðàöèÿ ïîëó÷åíèÿ ïî èíäåêñó
	if (index < size_)
		return array[index];
	else {
		std::cerr << "Error dont have in massive DATA[" << index << "]\n";
		std::exit(1);
	}
}
void DataVector::inc_size() {
	this->size_++;
	if (size_ >= capacity) MemAdd();
}
size_t DataVector::size() {
	return this->size_;
}
void DataVector::resize(size_t new_size) {
	while (new_size > this->capacity) MemAdd();
	if (new_size > this->size_) {
		for (size_t i = this->size_; i < new_size; i++)
			this->array[i] = DATA();
	}
	this->size_ = new_size;
}
size_t DataVector::count_distance() {
	return this->array[1].time - this->array[1].time;
}
void DataVector::clear() {
	delete[] array;
	size_ = 0;
	array = new DATA[capacity];
}
DataVector& DataVector::operator=(DataVector& other) {
	if (this != &other) {
		delete[] array;
		array = new DATA[other.capacity];
		for (size_t i = 0; i < other.size_; ++i) array[i] = other.array[i];
		size_ = other.size_;
		capacity = other.capacity;
	}
	return *this;
}
DataVector::~DataVector() {
	delete[] array;
}


void load_data(DataVector& data, string file_name) {
	std::ifstream work_file;
	work_file.open(file_name);
	DATA temp_datum;
	for (int i = 0; !work_file.eof(); i++) {
		work_file >> temp_datum;
		data.inc_size();
		data[i] = temp_datum;
	}
}
void dump_data(DataVector& data, string file_name) {
	std::ofstream result_file;
	result_file.open(file_name);
	for (int i = 0; i < data.size(); i++)
		result_file << data[i];
	result_file.close();
}