#include "WARNING.h"

bool have_promlems_with_opening_file(std::ifstream& file, std::string file_name) {
	try {
		file.open(file_name);
	}
	catch (...) {
		if (file.is_open())
			file.close();
		return true;
	}
	if (!file.is_open()) {
		return true;
	}
	return false;
}

bool have_promlems_with_reading_data(std::ifstream& file, std::string& str) {
	try {
		std::getline(file, str);
	}
	catch (...) {
		file.close();
		return true;
	}
	if (str == "") {
		file.close();
		return true;
	}
	return false;
}


bool enter_menu_point(std::string& answer) {
	std::getline(std::cin, answer);
	if (answer.size() != 1) {
		enter_point_menu_warning();
		return false;
	}
	return true;
}

bool enter_int_numeric(long long& numeric) {
	std::string buffer;
	std::getline(std::cin, buffer);
	if (is_int_numeric(buffer)) {
		numeric = std::stol(buffer);
	}
	else {
		enter_int_numeric_warning();
		return false;
	}
	return true;
}

bool enter_double_numeric(double& numeric, bool invers_descriptor) {
	std::string buffer;
	std::getline(std::cin, buffer);
	if (is_double_numeric(buffer)) {
		if (invers_descriptor)
			std::replace(buffer.begin(), buffer.end(), '.', ',');
		numeric = std::stod(buffer);
	}
	else {
		enter_double_numeric_warning();
		return false;
	}
	return true;
}

bool string_symbol_to_bool(std::string str, bool& answer) {
	if (str[0] == '0') {
		answer = 0;
	}
	else if (str[0] == '1') {
		answer = 1;
	}
	else {
		enter_invalid_data();
		return false;
	}
	return true;
}


bool is_double_numeric(std::string str) {
	std::vector<char> numerics{ '1', '2', '3', '4', '5', '6', '7', '8', '9', '.', '0' };
	for (size_t i = 0; i < str.size(); i++) {
		if (std::find(numerics.begin(), numerics.end(), str[i]) == numerics.end())
			return false;
	}
	return true;
}

bool is_int_numeric(std::string str) {
	std::vector<char> numerics{ '1', '2', '3', '4', '5', '6', '7', '8', '9', '0' };
	for (size_t i = 0; i < str.size(); i++) {
		if (std::find(numerics.begin(), numerics.end(), str[i]) == numerics.end())
			return false;
	}
	return true;
}


std::string extractLastNChars(std::string const& str, int n) {
	if (str.size() < n) {
		return str;
	}

	return str.substr(str.size() - n);
}

std::vector<std::string> split_string(std::string str, char descriptor) {
	std::vector<std::string> splited_string;
	std::string buffer_string = "";
	for (size_t i = 0; i < str.length(); i++) {
		if (str[i] == descriptor) {
			splited_string.push_back(buffer_string);
			buffer_string = "";
		}
		else {
			buffer_string += str[i];
		}
	}
	if (buffer_string.size() != 0) {
		splited_string.push_back(buffer_string);
	}
	return splited_string;
}

size_t check_quantity_data_lines_in_file(std::ifstream& file, std::string file_name) {
	file.open(file_name);
	std::string buffer_string;
	size_t new_size = 0;
	while (!file.eof()) {
		std::getline(file, buffer_string);
		if (buffer_string.length() > 4)
			new_size++;
	}
	file.close();
	return new_size;
}

bool check_OCDF_in_file(std::string file_name) {
	std::ifstream file;
	if (have_promlems_with_opening_file(file, file_name)) {
		return false;
	}
	std::string test_str;
	if (have_promlems_with_reading_data(file, test_str)) {
		return false;
	}
	file.close();
	std::vector<std::string> splited_test_str = split_string(test_str, ';');
	if (splited_test_str.size() != 3) {
		return false;
	}
	for (size_t i = 0; i < splited_test_str.size(); i++) {
		if (!is_double_numeric(splited_test_str[i])) {
			return false;
		}
	}
	return true;
}

bool check_TDF_in_file(std::string file_name) {
	std::ifstream file;
	if (have_promlems_with_opening_file(file, file_name)) {
		return false;
	}
	std::string test_str;
	if (have_promlems_with_reading_data(file, test_str)) {
		return false;
	}
	file.close();
	std::vector<std::string> splited_test_str = split_string(test_str, ';');
	if (splited_test_str.size() != 7) {
		return false;
	}
	for (size_t i = 0; i < splited_test_str.size(); i++) {
		if (!is_double_numeric(splited_test_str[i])) {
			return false;
		}
	}
	return true;
}

void begin_loaging() {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, 10);
	std::cerr << "\nЗагрузка, пожалуйства подождите...\n\n";
	SetConsoleTextAttribute(hConsole, 7);
}

void end_loading() {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, 10);
	std::cerr << "\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b";
	SetConsoleTextAttribute(hConsole, 7);
}

void msg_warning(std::string msg, short color) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, color);
	std::cout << msg;
	SetConsoleTextAttribute(hConsole, 7);
}

void delete_msg(std::string msg) {
	for (size_t i = 0; i < msg.length(); i++)
		std::cout << "\b";
}

void enter_menu_warning() {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, 12);
	std::cerr << "\nОшибка ввода! Неверный пункт меню!\n\n";
	SetConsoleTextAttribute(hConsole, 7);
	system("pause");
}

void load_data_warning() {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, 12);
	std::cerr << "\nОшибка загрузки данных! Невозможно считать данные!\n\n";
	SetConsoleTextAttribute(hConsole, 7);
	system("pause");
}

void check_data_format_warining() {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, 12);
	std::cerr << "\nОшибка загрузки данных! Неверный формат данных или невозможно считать строку с данными!\n\n";
	SetConsoleTextAttribute(hConsole, 7);
	system("pause");
}

void open_file_warning() {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, 12);
	std::cerr << "\nОшибка загрузки данных! Невозможно открыть файл!\n\n";
	SetConsoleTextAttribute(hConsole, 7);
	system("pause");
}

void overmuch_data_in_file_warning() {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, 12);
	std::cerr << "\nОшибка загрузки данных! Слишком много данных!\n\n";
	SetConsoleTextAttribute(hConsole, 7);
	system("pause");
}

void enter_int_numeric_warning() {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, 12);
	std::cerr << "\nОшибка ввода данных! Введённое значение не является целым числом!\n\n";
	SetConsoleTextAttribute(hConsole, 7);
	system("pause");
}

void enter_double_numeric_warning() {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, 12);
	std::cerr << "\nОшибка ввода данных! Введённое значение не является действительным числом!\n\n";
	SetConsoleTextAttribute(hConsole, 7);
	system("pause");
}

void enter_point_menu_warning() {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, 12);
	std::cerr << "\nОшибка ввода данных! Введено слишком много символов!\n\n";
	SetConsoleTextAttribute(hConsole, 7);
	system("pause");
}

void enter_invalid_data() {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, 12);
	std::cerr << "\nОшибка ввода данных! Введённое число недопустимо!\n\n";
	SetConsoleTextAttribute(hConsole, 7);
	system("pause");
}

void cids_warning() {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, 12);
	std::cerr << "\nОшибка преобразования данных! Данные содержат не все сиды!\n\n";
	SetConsoleTextAttribute(hConsole, 7);
	system("pause");
}