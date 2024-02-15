#include "MainOCDFMenu.h"

void MainOCDF_Menu() {
	map<unsigned char, function<void(vector<OCDF>& data)>> menu{
		make_pair('1', cut_percent_OCDF_data),
		make_pair('2', cut_quantity_OCDF_data),
		make_pair('3', right_time_OCDF),
		make_pair('4', show_OCDF_data),
		make_pair('5', pars_OCDF_data_per_cid),
		make_pair('6', add_more_data),
		make_pair('7', save_OCDF_data_in_csv),
		make_pair('8', save_OCDF_data_in_bin)
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
			<< "6 - добавить новые данные\n"
			<< "7 - сохранить данные в файл формата csv\n"
			<< "8 - сохранить данные в бинарный файл\n"
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

void show_OCDF_data(vector<OCDF>& data) {
	bool is_one_cid = true;

	for (size_t i = 1; i < data.size(); i++) {
		if (data[0].cid != data[i].cid) {
			is_one_cid = false;
			break;
		}
	}

	system("cls");
	dump_data(data, "..\\..\\..\\..\\PyVisualisation\\temp_ocdf.csv");
	std::cout << "Данные визуализирвоаны!\n\n" << "Чтобы выйти в меню, закройте окно визуализации!\n";

	if (is_one_cid) {
		system("python ..\\..\\..\\..\\PyVisualisation\\OCDF_OneCid_Visual.py ..\\..\\..\\..\\PyVisualisation\\temp_ocdf.csv");
	}
	else {
		system("python ..\\..\\..\\..\\PyVisualisation\\OCDF_SixCids_Visual.py ..\\..\\..\\..\\PyVisualisation\\temp_ocdf.csv");
	}

	DeleteFile("..\\..\\..\\..\\PyVisualisation\\temp_ocdf.csv");
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

void add_more_data(vector<OCDF>& data) {
	system("cls");

	vector<OCDF> new_data;
	new_data = read_OCDF_file("Данные какого файла необходимо добавить?\n\n");

	system("cls");

	msg_warning("Соединяем данные...");

	vector<OCDF> all_data(data.size() + new_data.size());
	for (size_t i = 0, j = 0, k = 0; k < all_data.size(); k++) {
		if (i == data.size()) { // если закончились данные в исходном векторе
			all_data[k] = new_data[j];
			j++;
			continue;
		}
		else if (j == new_data.size()) { // если закончились данные в новом векторе
			all_data[k] = data[i];
			i++;
			continue;
		}
		else { // если данные ещё есть в исходном и новом векторах
			if (data[i].time < new_data[j].time) { // если данные из исходного вектора временем раньше, чем данные из нового вектора
				all_data[k] = data[i];
				i++;
				continue;
			}
			else if (data[i].time > new_data[j].time) { // если данные из нового вектора временем раньше, чем данные из исходного вектора
				all_data[k] = new_data[j];
				j++;
				continue;
			}
			else { // если данные из нового вектора и исходного вектора имеют одинаковое время
				all_data[k] = data[i];
				i++;
				k++;
				all_data[k] = new_data[j];
				j++;
				continue;
			}
		}
	}

	data = all_data;

	delete_msg("Соединяем данные...");
	return;
}

void save_OCDF_data_in_csv(vector<OCDF>& data) {
	system("cls");

	string dump_file_path;
	std::cout << "Введите имя файла для загрузки в него данных: ";
	std::getline(std::cin, dump_file_path);
	for (size_t k = dump_file_path.find('\"'); k != dump_file_path.npos; k = dump_file_path.find('\"', k))
		dump_file_path.erase(k, 1);

	dump_data(data, dump_file_path);

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, 10);
	std::cout << "\nДанные успешно сохранены!\n\n";
	SetConsoleTextAttribute(hConsole, 7);
	system("pause");
}

void save_OCDF_data_in_bin(vector<OCDF>& data) {
	system("cls");

	string dump_file_path;
	std::cout << "Введите имя файла для загрузки в него данных: ";
	std::getline(std::cin, dump_file_path);
	for (size_t k = dump_file_path.find('\"'); k != dump_file_path.npos; k = dump_file_path.find('\"', k))
		dump_file_path.erase(k, 1);

	dump_file_path += ".bin";

	bindump_data(data, dump_file_path);

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, 10);
	std::cout << "\nДанные успешно сохранены!\n\n";
	SetConsoleTextAttribute(hConsole, 7);
	system("pause");
}
