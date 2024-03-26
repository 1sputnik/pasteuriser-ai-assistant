#include "DataManipulator.h"

// иерархия меню
void MainDataFormat_Menu(); 
	void MainTDF_Menu();
	void create_TDF_file();
void show_info();

int main()
{
	SetConsoleCP(CP_UTF8);
	SetConsoleOutputCP(CP_UTF8);

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

		std::cout << "DataManipulator: главное меню\n\n";

		std::cout << "Выберите пункт меню:\n"
			<< "1 - обработка данных стандартного формата (OCDF)\n"
			<< "2 - обработка данных табличного формата (TDF)\n"
			<< "3 - сборка табличного формата из общего файла OCDF-формата\n"
			<< "4 - информация о программе\n"
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
			msg_warning("\nОшибка ввода! Неверный пункт меню!\n\n");
		}
	}
}

void show_info() {
	system("cls");
	std::cout << "DataManipulator - это модуль, который предназначен для работы с данными, их обработкой, компоновкой и так далее.\n\n"

		<< "Этот модуль использует следующую терминологию:\n\n"

		<< "Стандартный формат данных - OCDF - это формат исходных источников данных, находящихся в папке data этого репозитория.\n"
		<< "Данные представлены следующим образом:\n"
		<< "сид;время;значение\n"
		<< "Для этого формата существует такое понятие как общий OCDF-файл.\n"
		<< "Общий OCDF-файл - это файл, данные которого представлены в формате OCDF и содержат значения всех сидов.\n\n"

		<< "Табличный формат данных - TDF - это формат, где значения всех датчиков собраны в одну таблицу и определены в каждый момент времени.\n"
		<< "Данные представлены следующим образом:\n"
		<< "время;значение_сида_1;значение_сида_2;значение_сида_3;значение_сида_4;значение_сида_5;значение_сида_6\n\n";

	system("pause");
}
