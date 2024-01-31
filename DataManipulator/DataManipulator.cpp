#include "DataManipulator.h"

// иерархия меню
void MainDataFormat_Menu(); 
	void MainOCDF_Menu();
		void cut_percent_OCDF_data(vector<OCDF>& data);
		void cut_quantity_OCDF_data(vector<OCDF>& data);
		void right_time_OCDF(vector<OCDF>& data);
		//void show_OCDF_data(vector<OCDF>& data);
		void pars_OCDF_data_per_cid(vector<OCDF>& data);
		void save_OCDF_data(vector<OCDF>& data);
	void read_TDF_file(); 
		void MainTDF_Menu(vector<TDF>& data);
	void create_TDF_file();
		void create_TDF_file_out_general_OCDF_file();
		void create_TDF_file_out_six_OCDF_files();
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
		make_pair('1', MainOCDF_Menu),
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

// OCDF menu --------------------------------------------------------------

vector<OCDF> read_OCDF_file(string special_msg = "") {
	vector<OCDF> data;

	string load_file_path;
	long long data_size;

	while (true) {
		system("cls");

		std::cout << special_msg;

		std::cout << "Введите имя файла для загрузки из него данных: ";
		std::getline(std::cin, load_file_path);

		std::cout << "Сколько данных необходимо загрузить (введите 0, если надо загрузить все данные): ";
		if (!enter_int_numeric(data_size))
			continue;

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

	return data;
}

void MainOCDF_Menu() {
	map<unsigned char, function<void(vector<OCDF>& data)>> menu{
		make_pair('1', cut_percent_OCDF_data),
		make_pair('2', cut_quantity_OCDF_data),
		make_pair('3', right_time_OCDF),
		//make_pair('3', show_OCDF_data),
		make_pair('5', pars_OCDF_data_per_cid),
		make_pair('6', save_OCDF_data)
	};

	vector<OCDF> data;
	data = read_OCDF_file();

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
			<< "5 - распарсить данные по сиду\n"
			<< "6 - сохранить данные в файл\n"
			<< "0 - выход в главное меню\n"
			<< "Введите пункт меню: ";

		string answer;
		if (!enter_menu_point(answer))
			continue;

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
		std::cout << "Введите процент данных, который нужно оставить: ";
		if (!enter_double_numeric(cut_percent, true))
			continue;
		if (cut_percent > 1.0 || cut_percent < 0.0) {
			enter_invalid_data();
			continue;
		}

		bool cut_trend;
		string answer;
		std::cout << "Выберите сторону обрезки (0 - слева направо, 1 - справо налево): ";
		if (!enter_menu_point(answer))
			continue;

		if (!string_symbol_to_bool(answer, cut_trend))
			continue;

		data = cut_percent_data(data, cut_percent, cut_trend);

		break;
	}
}

void cut_quantity_OCDF_data(vector<OCDF>& data) {
	while (true) {
		system("cls");

		long long cut_quantity;
		std::cout << "Введите количество данных, которое нужно оставить: ";
		if (!enter_int_numeric(cut_quantity))
			continue;
		if (cut_quantity <= 0) {
			enter_invalid_data();
			continue;
		}

		bool cut_trend;
		string answer;
		std::cout << "Выберите сторону обрезки (0 - слева направо, 1 - справо налево): ";
		if (!enter_menu_point(answer))
			continue;

		if (!string_symbol_to_bool(answer, cut_trend))
			continue;

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

		long long range;
		std::cout << "Введите необходимый диапозон между временными точками (0 - выход): ";
		if (!enter_int_numeric(range))
			continue;
		if (range == 0) {
			return;
		}
		else if (range < 0) {
			enter_invalid_data();
			continue;
		}

		data = right_range(data, range);

		break;
	}
}

void pars_OCDF_data_per_cid(vector<OCDF>& data) {
	while (true) {
		system("cls");

		long long number;
		std::cout << "Введите номер сида, который необходимо оставить: ";
		if (!enter_int_numeric(number))
			continue;

		if (number > 6 || number < 0) {
			enter_invalid_data();
			continue;
		}

		data = parsing_data_per_cid(data, number);

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
	std::cout << "\nДанные успешно сохранены!\n\n";
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
	map<unsigned char, function<void()>> menu{
		make_pair('1', create_TDF_file_out_general_OCDF_file),
		make_pair('2', create_TDF_file_out_six_OCDF_files)
	};

	while (true) {
		system("cls");

		std::cout << "Выбрите формат файла, для создания файла формата TDF:\n"
			<< "1 - создать TDF файл из общего OCDF фала со всеми сидами\n"
			<< "2 - создать TDF файл из шести OCDF файлов разных сидов\n"
			<< "0 - выход\n"
			<< "Выберите пункт меню: ";

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

void create_TDF_file_out_general_OCDF_file() {
	while (true) {
		system("cls");

		string load_file_path;
		std::cout << "Введите имя файла для загрузки из него данных (введите 0, чтобы выйти): ";
		std::getline(std::cin, load_file_path);
		if (load_file_path == "0")
			return;
	}
}

void create_TDF_file_out_six_OCDF_files() {

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
