#include "MainTDFMenu.h"

void MainTDF_Menu() {
	map<unsigned char, function<void(vector<TDF>& data)>> menu{
		make_pair('1', cut_percent_TDF_data),
		make_pair('2', cut_quantity_TDF_data),
		make_pair('3', cut_time_TDF_data),
		make_pair('4', show_TDF_data),
		make_pair('5', pars_TDF_data_per_cid),
		make_pair('6', save_TDF_data_in_csv),
		make_pair('7', save_TDF_data_in_bin)
	};

	vector<TDF> data;
	data = read_TDF_file("DataManipulator: работа с форматом TDF\n\n");

	while (true) {
		system("cls");

		std::cout << "DataManipulator: работа с форматом TDF\n\n";

		std::cout << "Данные загружены\n"
			<< "Формат данных: TDF\n"
			<< "Размерность данных: " << data.size() << "\n"
			<< "Первая строка данных:\t\t" << data[0] << "\n"
			<< "Последняя строка данных:\t" << data[data.size() - 1] << "\n\n";

		std::cout << "Выберите пункт меню:\n"
			<< "1 - обрезать заданный процент данных\n"
			<< "2 - обрезать заданное количество данных\n"
			<< "3 - обрезать относительно заданного момента времени\n"
			<< "4 - визуализировать данные\n"
			<< "5 - распарсить данные по сиду\n"
			<< "6 - сохранить данные в файл формата csv\n"
			<< "7 - сохранить данные в бинарный файл\n"
			<< "0 - выход в главное меню\n"
			<< "Введите пункт меню: ";

		string answer;
		getline(cin, answer);
		if (!is_menu_point(answer)) {
			msg_warning("\nОшибка ввода данных! Введено слишком много символов!\n\n");
			continue;
		}

		if (answer[0] == '0')
			return;

		try {
			menu.at(answer[0])(data);
		}
		catch (...) {
			msg_warning("\nОшибка ввода! Неверный пункт меню!\n\n");
		}
	}
}

void cut_percent_TDF_data(vector<TDF>& data) {
	while (true) {
		system("cls");

		cout << "DataManipulator: обрезка TDF-данных по заданному проценту\n\n";

		double cut_percent;
		cout << "Введите процент данных, который нужно оставить (число в диапазоне (0.0; 1.0)): ";
		string answer;
		getline(cin, answer);
		if (!string_to_double(answer, cut_percent, false)) {
			msg_warning("\nОшибка ввода данных! Введённое значение не является действительным числом!\n\n");
			continue;
		}

		if (cut_percent > 1.0 || cut_percent < 0.0) {
			msg_warning("\nОшибка ввода данных! Введённое число недопустимо!\n");
			continue;
		}

		bool cut_trend;
		cout << "Выберите сторону обрезки (0 - слева направо, 1 - справо налево): ";
		answer.clear();
		getline(cin, answer);
		if (!is_menu_point(answer)) {
			msg_warning("\nОшибка ввода данных! Введено слишком много символов!\n");
			continue;
		}

		if (!string_to_bool(answer, cut_trend)) {
			msg_warning("\nОшибка ввода данных! Введённо недопустимое значение или слишком много символов!\n");
			continue;
		}

		data = cut_percent_data(data, cut_percent, cut_trend);

		break;
	}
}

void cut_quantity_TDF_data(vector<TDF>& data) {
	while (true) {
		system("cls");

		cout << "DataManipulator: обрезка TDF-данных по заданному количеству\n\n";

		int cut_quantity;
		cout << "Введите количество данных, которое нужно оставить: ";
		string answer;
		getline(cin, answer);
		if (!string_to_integer(answer, cut_quantity)) {
			msg_warning("\nОшибка ввода данных! Введённое значение не является целым числом или число отрицательное!\n");
			continue;
		}

		if (cut_quantity <= 0) {
			msg_warning("\nОшибка ввода данных! Введённое число недопустимо!\n\n");
			continue;
		}

		bool cut_trend;
		answer.clear();
		cout << "Выберите сторону обрезки (0 - слева направо, 1 - справо налево): ";
		getline(cin, answer);
		if (!is_menu_point(answer)) {
			msg_warning("\nОшибка ввода данных! Введено слишком много символов!\n\n");
			continue;
		}

		if (!string_to_bool(answer, cut_trend)) {
			msg_warning("\nОшибка ввода данных! Введённо недопустимое значение или слишком много символов!\n");
			continue;
		}

		data = cut_quntity_data(data, cut_quantity, cut_trend);

		break;
	}
}

void cut_time_TDF_data(vector<TDF>& data) {
	while (true) {
		system("cls");

		cout << "DataManipulator: обрезка TDF-данных по заданному моменту времени\n\n";

		int cut_time;
		cout << "Введите момент времени, который будет являться границей обрезки: ";
		string answer;
		getline(cin, answer);
		if (!string_to_integer(answer, cut_time)) {
			msg_warning("\nОшибка ввода данных! Введённое значение не является целым числом или число отрицательное!\n");
			continue;
		}

		if (cut_time <= 0) {
			msg_warning("\nОшибка ввода данных! Введённое число недопустимо!\n\n");
			continue;
		}

		bool cut_trend;
		answer.clear();
		cout << "\nВыберите сторону обрезки (0 - слева направо, 1 - справо налево): ";
		getline(cin, answer);
		if (!is_menu_point(answer)) {
			msg_warning("\nОшибка ввода данных! Введено слишком много символов!\n\n");
			continue;
		}

		if (!string_to_bool(answer, cut_trend)) {
			msg_warning("\nОшибка ввода данных! Введённо недопустимое значение или слишком много символов!\n");
			continue;
		}

		vector<TDF> buffer_data = cut_data_per_time(data, cut_time, cut_trend);
		if (buffer_data.size() == 0) {
			msg_warning("\nОшибка выполнения операции! Результат выполенния операции есть пустая последовательность данных!\n\n");
			return;
		}
		else {
			data = buffer_data;
		}

		break;
	}
}

void show_TDF_data(vector<TDF>& data) {
	system("cls");

	cout << "DataManipulator: визуализация TDF формата\n\n";

	dump_data(data, "..\\..\\..\\..\\PyVisualisation\\temp_ocdf.csv");
	cout << "Данные визуализирвоаны!\n\n" << "Чтобы выйти в меню, закройте окно визуализации!\n";

	system("python ..\\..\\..\\..\\PyVisualisation\\TDF_Visual.py ..\\..\\..\\..\\PyVisualisation\\temp_ocdf.csv");

	DeleteFile("..\\..\\..\\..\\PyVisualisation\\temp_ocdf.csv");
}

void pars_TDF_data_per_cid(vector<TDF>& data) {
	int number;
	while (true) {
		system("cls");

		cout << "DataManipulator: парсинг TDF формата по указанному сиду\n\n";

		cout << "Введите номер сида, который необходимо оставить (0 - чтобы выйти): ";
		string answer;
		getline(cin, answer);
		if (!string_to_integer(answer, number)) {
			msg_warning("\nОшибка ввода данных! Введённое значение не является целым числом или число отрицательное!\n");
			continue;
		}

		if (number == 0)
			return;

		if (number > 6 || number < 0) {
			msg_warning("\nОшибка ввода данных! Введённое число недопустимо!\n");
			continue;
		}

		break;
	}

	vector<OCDF> OCdata = parsing_data_per_cid(data, number);

	bool save_to_bin_format = false;
	while (true) {
		system("cls");

		cout << "DataManipulator: парсинг TDF формата по указанному сиду\n\n";

		string answer;
		cout << "В каком формате сохранить даныне (0 - .csv, 1 - .bin): ";
		getline(cin, answer);

		if (!is_menu_point(answer)) {
			msg_warning("\nОшибка ввода данных! Введено слишком много символов!\n");
			continue;
		}

		if (!string_to_bool(answer, save_to_bin_format)) {
			msg_warning("\nОшибка ввода данных! Введённо недопустимое значение или слишком много символов!\n");
			continue;
		}

		break;
	}

	if (save_to_bin_format) {
		save_OCDF_data_in_bin(OCdata);
	}
	else {
		save_OCDF_data_in_csv(OCdata);
	}
}

void save_TDF_data_in_csv(vector<TDF>& data) {
	system("cls");

	cout << "DataManipulator: сохранение данных TDF формата в текстовом режиме\n\n";

	string dump_file_path;
	cout << "Введите имя файла для загрузки в него данных: ";
	getline(cin, dump_file_path);
	for (size_t k = dump_file_path.find('\"'); k != dump_file_path.npos; k = dump_file_path.find('\"', k))
		dump_file_path.erase(k, 1);

	vector<string> reserved_formats = { ".tlstm", ".vlstm", ".lstm", ".bin" };
	try {
		for (size_t i = 0; i < reserved_formats.size(); i++)
			if (dump_file_path.substr(dump_file_path.length() - reserved_formats[i].size(), reserved_formats[i].size()) == reserved_formats[i])
				msg_warning("\nОшибка ввода пути! Введён зарезервирвоанный формат!\n\n");
	}
	catch (...) {
		msg_warning("\nОшибка ввода пути! Не удалось сохранить данные по указанному пути!\n\n");
		return;
	}

	if (dump_file_path.find('.') == dump_file_path.npos) {
		msg_warning("\nОшибка ввода пути! Не удалось сохранить данные по указанному пути!\n\n");
		return;
	}

	dump_data(data, dump_file_path);
}

void save_TDF_data_in_bin(vector<TDF>& data) {
	system("cls");

	cout << "DataManipulator: сохранение данных TDF формата в бинарном режиме\n\n";

	string dump_file_path;
	cout << "Введите имя файла для загрузки в него данных: ";
	getline(cin, dump_file_path);
	for (size_t k = dump_file_path.find('\"'); k != dump_file_path.npos; k = dump_file_path.find('\"', k))
		dump_file_path.erase(k, 1);

	dump_file_path += ".bin";

	bindump_data(data, dump_file_path);
}
