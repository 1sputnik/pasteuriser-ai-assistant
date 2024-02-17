#include "DataManipulator.h"

// иерархия меню
void MainDataFormat_Menu(); 
	void MainTDF_Menu();
	void create_TDF_file();
void show_info();

int main()
{
	setlocale(LC_ALL, "Russian");
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, 7);

	MainDataFormat_Menu();
	return 0;
}

// Main menu --------------------------------------------------------------

void MainDataFormat_Menu() {
	map<unsigned char, function<void()>> menu {
		make_pair('1', MainOCDF_Menu),
		make_pair('2', MainTDF_Menu),
		make_pair('3', create_TDF_file),
		make_pair('4', show_info)
	};
	
	while (true) {
		system("cls");

		std::cout << "Выберите формат данных:\n"
			<< "1 - стандартный формат (OCDF)\n"
			<< "2 - табличный формат (TDF)\n"
			<< "3 - собрать табличный формат из файлов стандартного формата\n"
			<< "4 - информация\n"
			<< "0 - выход из программы\n"
			<< "Введите пункт меню: ";
		
		string answer;
		if (!enter_menu_point(answer))
			continue;

		if (answer[0] == '0')
			return;

		try {
			menu.at(answer[0])();
		}
		catch (...) {
			enter_menu_warning();
		}
	}
}

void show_info() {
	system("cls");
	std::cout << "DataManipulator - это модуль, который предназначен для работы с данными, их обработкой, компоновкой и так далее.\n\n"

		<< "Этот модуль использует следующую терминологию:\n\n"

		<< "Стандартный формат данных - OCDF - это формат исходных источников данных. Данные представлены следующим образом:\n"
		<< "сид;время;значение\n\n"

		<< "Табличный формат данных - TDF - это формат, где значения всех датчиков собраны в одну таблицу и определены в каждый момент времени. Представление данных:\n"
		<< "время;значение_сида_1;значение_сида_2;значение_сида_3;значение_сида_4;значение_сида_5;значение_сида_6\n\n";

	system("pause");
}
