#include "WARNING.h"

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