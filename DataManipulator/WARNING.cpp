#include "WARNING.h"

void enter_menu_warning() {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, 12);
	std::cerr << "\n������ �����! �������� ����� ����!\n\n";
	SetConsoleTextAttribute(hConsole, 7);
	system("pause");
}

void load_data_warning() {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, 12);
	std::cerr << "\n������ �������� ������! ���������� ������� ������!\n\n";
	SetConsoleTextAttribute(hConsole, 7);
	system("pause");
}

void check_data_format_warining() {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, 12);
	std::cerr << "\n������ �������� ������! �������� ������ ������ ��� ���������� ������� ������ � �������!\n\n";
	SetConsoleTextAttribute(hConsole, 7);
	system("pause");
}

void open_file_warning() {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, 12);
	std::cerr << "\n������ �������� ������! ���������� ������� ����!\n\n";
	SetConsoleTextAttribute(hConsole, 7);
	system("pause");
}

void overmuch_data_in_file_warning() {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, 12);
	std::cerr << "\n������ �������� ������! ������� ����� ������!\n\n";
	SetConsoleTextAttribute(hConsole, 7);
	system("pause");
}

void enter_int_numeric_warning() {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, 12);
	std::cerr << "\n������ ����� ������! �������� �������� �� �������� ����� ������!\n\n";
	SetConsoleTextAttribute(hConsole, 7);
	system("pause");
}

void enter_double_numeric_warning() {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, 12);
	std::cerr << "\n������ ����� ������! �������� �������� �� �������� �������������� ������!\n\n";
	SetConsoleTextAttribute(hConsole, 7);
	system("pause");
}

void enter_point_menu_warning() {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, 12);
	std::cerr << "\n������ ����� ������! ������� ������� ����� ��������!\n\n";
	SetConsoleTextAttribute(hConsole, 7);
	system("pause");
}

void enter_invalid_data() {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, 12);
	std::cerr << "\n������ ����� ������! �������� ����� �����������!\n\n";
	SetConsoleTextAttribute(hConsole, 7);
	system("pause");
}