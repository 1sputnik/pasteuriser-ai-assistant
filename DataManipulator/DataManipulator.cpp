#include "DataManipulator.h"

void MainDataFormat_Menu();
	void read_OCDF_file(); void MainOCDF_Menu(vector<OCDF>& data);
	void read_TDF_file(); void MainTDF_Menu(vector<TDF>& data);
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

void MainDataFormat_Menu() {
	map<char, function<void()>> menu{
		make_pair('1', read_OCDF_file),
		make_pair('2', read_TDF_file),
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
		std::getline(std::cin, answer);
		if (answer.size() != 1) {
			enter_point_menu_warning();
			continue;
		}

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

void read_OCDF_file() {
	map<unsigned char, bool> menu{
		make_pair('y', true),
		make_pair('n', false)
	};

	vector<OCDF> data;

	string load_file_path;
	string bufer_str_data_size;
	long long data_size;

	while (true) {
		system("cls");
		std::cout << "Введите имя файла для загрузки из него данных: ";
		std::getline(std::cin, load_file_path);

		std::cout << "Сколько данных необходимо загрузить (введите 0, если надо загрузить все данные): ";
		std::getline(std::cin, bufer_str_data_size);
		if (is_int_numeric(bufer_str_data_size)) {
			data_size = std::stoi(bufer_str_data_size);
		}
		else {
			enter_int_numeric_warning();
			continue;
		}

		try {
			if (check_OCDF_in_file(load_file_path)) {
				data = load_data(data, load_file_path, data_size);
				break;
			}
			else {
				check_data_format_warining();
			}
		}
		catch (...) {
			load_data_warning();
			data.clear();
		}
	}

	MainOCDF_Menu(data);
}

void MainOCDF_Menu(vector<OCDF>& data) {
	//map<unsigned char, function<void(vector<OCDF>& data)>> menu{
	map<unsigned char, function<void()>> menu{
		make_pair('1', read_OCDF_file),
		make_pair('2', read_TDF_file)
	};

	while (true) {
		system("cls");

		std::cout << "Данные загружены\n"
			<< "Формат данных: OCDF\n"
			<< "Размерность данных: " << data.size() << "\n"
			<< "Первая строка данных:\t\t" << data[0] << "\n"
			<< "Последняя строка данных:\t" << data[data.size() - 1] << "\n\n";

		std::cout << "Выберите формат данных:\n"
			<< "1 - обрезать данные\n"
			<< "2 - выровнять диапазон по оси времени\n"
			<< "3 - визуализировать данные\n"
			<< "4 - вывод первых 100 строк данных\n"
			<< "5 - сохранить данные в файл\n"
			<< "0 - выход в главное меню\n"
			<< "Введите пункт меню: ";

		string answer;
		std::getline(std::cin, answer);

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

void read_TDF_file() {
	map<unsigned char, bool> menu{
		make_pair('y', true),
		make_pair('n', false)
	};

	vector<TDF> data;

	MainTDF_Menu(data);
}

void MainTDF_Menu(vector<TDF>& data) {
	std::cout << "ss";
}

void create_TDF_file() {
	std::cout << "ss";
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