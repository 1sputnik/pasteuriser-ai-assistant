#include "MainOCDFMenu.h"

void MainOCDF_Menu() {
	map<unsigned char, function<void(vector<OCDF>& data)>> menu{
		make_pair('1', cut_percent_OCDF_data),
		make_pair('2', cut_quantity_OCDF_data),
		make_pair('3', cut_time_OCDF_data),
		make_pair('4', right_time_OCDF),
		make_pair('5', show_OCDF_data),
		make_pair('6', pars_OCDF_data_per_cid),
		make_pair('7', add_more_data),
		make_pair('8', save_OCDF_data_in_csv),
		make_pair('9', save_OCDF_data_in_bin)
	};

	vector<OCDF> data;
	data = read_OCDF_file("DataManipulator: работа с форматом OCDF\n\n");

	while (true) {
		system("cls");

		std::cout << "DataManipulator: работа с форматом OCDF\n\n";

		std::cout << "Данные загружены\n"
			<< "Формат данных: OCDF\n"
			<< "Размерность данных: " << data.size() << "\n"
			<< "Первая строка данных:\t\t" << data[0] << "\n"
			<< "Последняя строка данных:\t" << data[data.size() - 1] << "\n\n";

		std::cout << "Выберите пункт меню:\n"
			<< "1 - обрезать заданный процент данных\n"
			<< "2 - обрезать заданное количество данных\n"
			<< "3 - обрезать относительно заданного момента времени\n"
			<< "4 - выровнять диапазон по оси времени\n"
			<< "5 - визуализировать данные\n"
			<< "6 - распарсить данные по сиду\n"
			<< "7 - добавить новые данные\n"
			<< "8 - сохранить данные в файл формата csv\n"
			<< "9 - сохранить данные в бинарный файл\n"
			<< "0 - выход в главное меню\n"
			<< "Введите пункт меню: ";

		string answer;
		std::getline(std::cin, answer);

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

void cut_percent_OCDF_data(vector<OCDF>& data) {
	while (true) {
		system("cls");

		std::cout << "DataManipulator: обрезка OCDF-данных по заданному проценту\n\n";

		double cut_percent;
		std::cout << "Введите процент данных, который нужно оставить (число в диапазоне (0.0; 1.0)): ";
		string answer;
		std::getline(std::cin, answer);
		if (!string_to_double(answer, cut_percent, false)) {
			msg_warning("\nОшибка ввода данных! Введённое значение не является действительным числом!\n\n");
			continue;
		}

		if (cut_percent > 1.0 || cut_percent < 0.0) {
			msg_warning("\nОшибка ввода данных! Введённое число недопустимо!\n\n");
			continue;
		}

		bool cut_trend;
		std::cout << "\nВыберите сторону обрезки (0 - слева направо, 1 - справо налево): ";
		answer.clear();
		std::getline(std::cin, answer);
		if (!is_menu_point(answer)) {
			msg_warning("\nОшибка ввода данных! Введено слишком много символов!\n\n");
			continue;
		}

		if (!string_to_bool(answer, cut_trend)) {
			msg_warning("\nОшибка ввода данных! Введённо недопустимое значение или слишком много символов!\n");
			continue;
		}

		vector<OCDF> buffer_data = cut_percent_data(data, cut_percent, cut_trend);
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

void cut_quantity_OCDF_data(vector<OCDF>& data) {
	while (true) {
		system("cls");

		std::cout << "DataManipulator: обрезка OCDF-данных по заданному количеству\n\n";

		long long cut_quantity;
		std::cout << "Введите количество данных, которое нужно оставить: ";
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
		std::cout << "\nВыберите сторону обрезки (0 - слева направо, 1 - справо налево): ";
		std::getline(std::cin, answer);

		if (!is_menu_point(answer)) {
			msg_warning("\nОшибка ввода данных! Введено слишком много символов!\n\n");
			continue;
		}

		if (!string_to_bool(answer, cut_trend)) {
			msg_warning("\nОшибка ввода данных! Введённо недопустимое значение или слишком много символов!\n");
			continue;
		}

		vector<OCDF> buffer_data = cut_percent_data(data, cut_quantity, cut_trend);
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

void cut_time_OCDF_data(vector<OCDF>& data) {
	while (true) {
		system("cls");

		std::cout << "DataManipulator: обрезка OCDF-данных по заданному моменту времени\n\n";

		long long cut_time;
		std::cout << "Введите момент времени, которое будет являться границей обрезки: ";
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
		std::cout << "\nВыберите сторону обрезки (0 - слева направо, 1 - справо налево): ";
		std::getline(std::cin, answer);

		if (!is_menu_point(answer)) {
			msg_warning("\nОшибка ввода данных! Введено слишком много символов!\n\n");
			continue;
		}

		if (!string_to_bool(answer, cut_trend)) {
			msg_warning("\nОшибка ввода данных! Введённо недопустимое значение или слишком много символов!\n");
			continue;
		}

		vector<OCDF> buffer_data = cut_data_per_time(data, cut_time, cut_trend);
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

void right_time_OCDF(vector<OCDF>& data) {
	while (true) {
		system("cls");

		std::cout << "DataManipulator: выравнивание OCDF данных по оси временни\n\n";

		std::cout << "Внимание! Данная операция предназначена для одного сида!\n\n";

		long long range;
		std::cout << "Введите необходимый диапозон между временными точками (0 - выход): ";
		string answer;
		getline(cin, answer);
		if (!string_to_integer(answer, range)) {
			msg_warning("\nОшибка ввода данных! Введённое значение не является целым числом или число отрицательное!\n");
			continue;
		}

		if (range == 0) {
			return;
		}
		else if (range < 0) {
			msg_warning("\nОшибка ввода данных! Введённое число недопустимо!\n\n");
			continue;
		}

		vector<OCDF> buffer_data = right_range(data, range);
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

void show_OCDF_data(vector<OCDF>& data) {
	if (data.size() < 2) {
		msg_warning("\nОшибка визуализации! Недостаточно данных для визуализации!");
		return;
	}

	bool is_one_cid = true;

	for (size_t i = 1; i < data.size(); i++) {
		if (data[0].cid != data[i].cid) {
			is_one_cid = false;
			break;
		}
	}

	system("cls");

	std::cout << "DataManipulator: визуализация OCDF формата\n\n";

	dump_data(data, "..\\..\\..\\..\\PyVisualisation\\temp_ocdf.csv");
	std::cout << "Данные визуализирвоаны!\n\n" << "Чтобы выйти в меню, закройте окно визуализации!\n";

	if (is_one_cid) {
		system("py ..\\..\\..\\..\\PyVisualisation\\OCDF_OneCid_Visual.py ..\\..\\..\\..\\PyVisualisation\\temp_ocdf.csv");
	}
	else {
		system("py ..\\..\\..\\..\\PyVisualisation\\OCDF_SixCids_Visual.py ..\\..\\..\\..\\PyVisualisation\\temp_ocdf.csv");
	}

	DeleteFile("..\\..\\..\\..\\PyVisualisation\\temp_ocdf.csv");
}

void pars_OCDF_data_per_cid(vector<OCDF>& data) {
	while (true) {
		system("cls");

		std::cout << "DataManipulator: парсинг OCDF формата по указанному сиду\n\n";

		long long number;
		std::cout << "Введите номер сида, который необходимо оставить (0 - чтобы выйти): ";
		string answer;
		getline(cin, answer);
		if (!string_to_integer(answer, number)) {
			msg_warning("\nОшибка ввода данных! Введённое значение не является целым числом или число отрицательное!\n");
			continue;
		}

		if (number == 0)
			return;

		if (number > 6 || number < 0) {
			msg_warning("\nОшибка ввода данных! Введённое число недопустимо!\n\n");
			continue;
		}

		bool check_cid_flag = false;
		for (size_t i = 0; i < data.size(); i++) {
			if (data[i].cid == number) {
				check_cid_flag = true;
				break;
			}
		}

		if (check_cid_flag) {
			data = parsing_data_per_cid(data, number);
		}
		else {
			msg_warning("\nОшибка ввода данных! Введён отсутствующий в данных сид!\n\n");
			continue;
		}

		break;
	}
}

void add_more_data(vector<OCDF>& data) {
	system("cls");

	vector<OCDF> new_data;
	new_data = read_OCDF_file("DataManipulator: добавление новых данных OCDF формата\n\nДанные какого файла необходимо добавить?\n\n");

	system("cls");

	std::cout << "DataManipulator: добавление новых данных OCDF формата\n\n";

	std::cout << "Соединяем данные...";

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
}

void save_OCDF_data_in_csv(vector<OCDF>& data) {
	system("cls");

	std::cout << "DataManipulator: сохранение данных OCDF формата в текстовом режиме\n\n";

	string dump_file_path;
	std::cout << "Введите имя файла для загрузки в него данных: ";
	std::getline(std::cin, dump_file_path);
	for (size_t k = dump_file_path.find('\"'); k != dump_file_path.npos; k = dump_file_path.find('\"', k))
		dump_file_path.erase(k, 1);

	vector<string> reserved_formats = {".tlstm", ".vlstm", ".lstm", ".bin"};
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

void save_OCDF_data_in_bin(vector<OCDF>& data) {
	system("cls");

	std::cout << "DataManipulator: сохранение данных OCDF формата в бинарном режиме\n\n";

	string dump_file_path;
	std::cout << "Введите имя файла для загрузки в него данных: ";
	std::getline(std::cin, dump_file_path);
	for (size_t k = dump_file_path.find('\"'); k != dump_file_path.npos; k = dump_file_path.find('\"', k))
		dump_file_path.erase(k, 1);

	dump_file_path += ".bin";

	bindump_data(data, dump_file_path);
}
