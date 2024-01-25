#include "DataManipulator.h"

void MainDataFormat_Menu();
	void read_OCDF_file(); void MainOCDF_Menu(vector<OCDF>& data);
		void cut_percent_OCDF_data(vector<OCDF>& data);
		void cut_quantity_OCDF_data(vector<OCDF>& data);
		void right_time_OCDF(vector<OCDF>& data);
		//void show_OCDF_data(vector<OCDF>& data);
		//void pars_OCDF_data_per_cid(vector<OCDF>& data);
		void save_OCDF_data(vector<OCDF>& data);
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

// Main menu --------------------------------------------------------------

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

// OCDF menu --------------------------------------------------------------

void read_OCDF_file() {
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
			data_size = std::stol(bufer_str_data_size);
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
	map<unsigned char, function<void(vector<OCDF>& data)>> menu{
		make_pair('1', cut_percent_OCDF_data),
		make_pair('2', cut_quantity_OCDF_data),
		make_pair('3', right_time_OCDF),
		//make_pair('3', show_OCDF_data),
		//make_pair('4', pars_OCDF_data_per_cid),
		make_pair('5', save_OCDF_data)
	};

	while (true) {
		system("cls");

		std::cout << "Данные загружены\n"
			<< "Формат данных: OCDF\n"
			<< "Размерность данных: " << data.size() << "\n"
			<< "Первая строка данных:\t\t" << data[0] << "\n"
			<< "Последняя строка данных:\t" << data[data.size() - 1] << "\n\n";

		std::cout << "Выберите формат данных:\n"
			<< "1 - обрезать заданный процент данных\n"
			<< "2 - обрезать заданное количество данных\n"
			<< "3 - выровнять диапазон по оси времени\n"
			<< "4 - визуализировать данные\n"
			<< "5 - сохранить данные в файл\n"
			<< "0 - выход в главное меню\n"
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
			menu.at(answer[0])(data);
		}
		catch (...) {
			enter_menu_warning();
		}
	}
}

void cut_percent_OCDF_data(vector<OCDF>& data) {
	while (true) {
		system("cls");

		double cut_percent;
		string buffer_double;
		std::cout << "Введите процент данных, который нужно оставить: ";
		std::getline(std::cin, buffer_double);
		if (is_double_numeric(buffer_double)) {
			std::replace(buffer_double.begin(), buffer_double.end(), '.', ',');
			cut_percent = std::stod(buffer_double);
			if (cut_percent > 1.0 || cut_percent < 0.0) {
				enter_invalid_data();
				continue;
			}
		}
		else {
			enter_double_numeric_warning();
			continue;
		}

		bool cut_trend;
		string answer;
		std::cout << "Выберите сторону обрезки (0 - слева направо, 1 - справо налево): ";
		std::getline(std::cin, answer);
		if (answer.size() != 1) {
			enter_point_menu_warning();
			continue;
		}

		if (answer[0] == '0') {
			cut_trend = 0;
		}
		else if (answer[0] == '1') {
			cut_trend = 1;
		}
		else {
			enter_invalid_data();
			continue;
		}

		data = cut_percent_data(data, cut_percent, cut_trend);

		break;
	}
}

void cut_quantity_OCDF_data(vector<OCDF>& data) {
	while (true) {
		system("cls");

		double cut_quantity;
		string buffer_double;
		std::cout << "Введите количество данных, которое нужно оставить: ";
		std::getline(std::cin, buffer_double);
		if (is_int_numeric(buffer_double)) {
			cut_quantity = std::stoi(buffer_double);
			if (cut_quantity <= 0) {
				enter_invalid_data();
				continue;
			}
		}
		else {
			enter_int_numeric_warning();
			continue;
		}

		bool cut_trend;
		string answer;
		std::cout << "Выберите сторону обрезки (0 - слева направо, 1 - справо налево): ";
		std::getline(std::cin, answer);
		if (answer.size() != 1) {
			enter_point_menu_warning();
			continue;
		}

		if (answer[0] == '0') {
			cut_trend = 0;
		}
		else if (answer[0] == '1') {
			cut_trend = 1;
		}
		else {
			enter_invalid_data();
			continue;
		}

		data = cut_quntity_data(data, cut_quantity, cut_trend);

		break;
	}
}

void right_time_OCDF(vector<OCDF>& data) {
	while (true) {
		system("cls");

		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hConsole, 14);
		std::cout << "Внимание! Данная операция предназначена для одного сида!\n\n";
		SetConsoleTextAttribute(hConsole, 7);

		double range;
		string buffer_double;
		std::cout << "Введите необходимый диапозон между временными точками (0 - выход): ";
		std::getline(std::cin, buffer_double);
		if (is_int_numeric(buffer_double)) {
			range = std::stoi(buffer_double);
			if (range == 0) {
				return;
			}
			else if (range < 0) {
				enter_invalid_data();
				continue;
			}
		}
		else {
			enter_int_numeric_warning();
			continue;
		}

		data = right_range(data, range);

		break;
	}
}

void save_OCDF_data(vector<OCDF>& data) {
	system("cls");

	string dump_file_path;
	std::cout << "Введите имя файла для загрузки в него данных: ";
	std::getline(std::cin, dump_file_path);

	dump_data(data, dump_file_path);

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, 10);
	std::cout << "\nДанные испешно сохранены!\n\n";
	SetConsoleTextAttribute(hConsole, 7);
	system("pause");
	return;
}

// TDF menu --------------------------------------------------------------

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

// CREATE TDF --------------------------------------------------------------

void create_TDF_file() {
	std::cout << "ss";
}

// INFO --------------------------------------------------------------

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