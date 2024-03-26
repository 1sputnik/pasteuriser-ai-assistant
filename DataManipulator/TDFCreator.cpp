#include "TDFCreator.h"

bool find_cid(vector<OCDF>& data, short cid) {
	for (size_t i = 0; i < data.size(); i++) {
		if (data[i].cid == cid)
			return true;
	}
	return false;
}

bool check_all_cids(vector<OCDF>& data) {
	if (!find_cid(data, 1))
		return false;
	if (!find_cid(data, 2))
		return false;
	if (!find_cid(data, 3))
		return false;
	if (!find_cid(data, 4))
		return false;
	if (!find_cid(data, 5))
		return false;
	if (!find_cid(data, 6))
		return false;
	return true;
}

void create_TDF_file() {
	// блок чтения файлов
	vector<OCDF> general_OCDF_data;
	general_OCDF_data = read_OCDF_file("DataManipulator: Сборка TDF-файла из общего OCDF-файла со всеми сидами\n\n");

	system("cls");

	std::cout << "DataManipulator: Сборка TDF-файла из общего OCDF-файла со всеми сидами\n\n";

	// проверка на наличие всех сидов
	std::cout << "Проверяем наличие всех сидов...";
	if (!check_all_cids(general_OCDF_data)) {
		msg_warning("\nОшибка преобразования данных!Данные содержат не все сиды!\n\n");
		return;
	}

	system("cls");

	std::cout << "DataManipulator: Сборка TDF-файла из общего OCDF-файла со всеми сидами\n\n";

	// парсим данные
	std::cout << "Парсим данные...";
	vector<OCDF> first_cid_data;
	first_cid_data = parsing_data_per_cid(general_OCDF_data, 1);
	vector<OCDF> second_cid_data;
	second_cid_data = parsing_data_per_cid(general_OCDF_data, 2);
	vector<OCDF> third_cid_data;
	third_cid_data = parsing_data_per_cid(general_OCDF_data, 3);
	vector<OCDF> forth_cid_data;
	forth_cid_data = parsing_data_per_cid(general_OCDF_data, 4);
	vector<OCDF> fifth_cid_data;
	fifth_cid_data = parsing_data_per_cid(general_OCDF_data, 5);
	vector<OCDF> sixth_cid_data;
	sixth_cid_data = parsing_data_per_cid(general_OCDF_data, 6);
	general_OCDF_data.clear();

	if (first_cid_data.size() < 2 || second_cid_data.size() < 2 ||
		third_cid_data.size() < 2 || forth_cid_data.size() < 2 ||
		fifth_cid_data.size() < 2 || sixth_cid_data.size() < 2) {
		msg_warning("\nОшибка при сборке TDF-файла! Данных некоторого сида слишком мало, чтобы выполнить данную операцию!\n\n");
	}

	system("cls");

	std::cout << "DataManipulator: Сборка TDF-файла из общего OCDF-файла со всеми сидами\n\n";

	// вводим необходимый диапазон данных относительно времени
	long long range;
	while (true) {
		std::cout << "Введите необходимый диапозон между временными точками: ";
		if (!enter_int_numeric(range))
			continue;

		else if (range < 0) {
			msg_warning("\nОшибка ввода данных! Введённое число недопустимо!\n\n");
			continue;
		}

		break;
	}

	// выравниваем диапазоны данных
	std::cout << "Определяем временные границы данных сидов...";

	vector<int> first_elements_of_cids{ first_cid_data[0].time, second_cid_data[0].time, third_cid_data[0].time,
		forth_cid_data[0].time, fifth_cid_data[0].time, sixth_cid_data[0].time };
	auto min_element = std::max_element(first_elements_of_cids.begin(), first_elements_of_cids.end());
	int min_time_border = *min_element;
	first_elements_of_cids.clear();

	vector<int> last_elements_of_cids{ first_cid_data[first_cid_data.size() - 1].time, second_cid_data[second_cid_data.size() - 1].time, third_cid_data[third_cid_data.size() - 1].time,
		forth_cid_data[forth_cid_data.size() - 1].time, fifth_cid_data[fifth_cid_data.size() - 1].time, sixth_cid_data[sixth_cid_data.size() - 1].time };
	auto max_element = std::min_element(last_elements_of_cids.begin(), last_elements_of_cids.end());
	int max_time_border = *max_element;
	last_elements_of_cids.clear();

	std::cout << "\b\b\b.\nВыравниваем диапазоны сидов по времени...";

	first_cid_data = cut_data_per_time(first_cid_data, min_time_border, false, true);
	first_cid_data = cut_data_per_time(first_cid_data, max_time_border, true, true);
	first_cid_data = right_range(first_cid_data, range);

	second_cid_data = cut_data_per_time(second_cid_data, min_time_border, false, true);
	second_cid_data = cut_data_per_time(second_cid_data, max_time_border, true, true);
	second_cid_data = right_range(second_cid_data, range);

	third_cid_data = cut_data_per_time(third_cid_data, min_time_border, false, true);
	third_cid_data = cut_data_per_time(third_cid_data, max_time_border, true, true);
	third_cid_data = right_range(third_cid_data, range);

	forth_cid_data = cut_data_per_time(forth_cid_data, min_time_border, false, true);
	forth_cid_data = cut_data_per_time(forth_cid_data, max_time_border, true, true);
	forth_cid_data = right_range(forth_cid_data, range);

	fifth_cid_data = cut_data_per_time(fifth_cid_data, min_time_border, false, true);
	fifth_cid_data = cut_data_per_time(fifth_cid_data, max_time_border, true, true);
	fifth_cid_data = right_range(fifth_cid_data, range);

	sixth_cid_data = cut_data_per_time(sixth_cid_data, min_time_border, false, true);
	sixth_cid_data = cut_data_per_time(sixth_cid_data, max_time_border, true, true);
	sixth_cid_data = right_range(sixth_cid_data, range);

	vector<size_t> data_sizes{ first_cid_data.size(), second_cid_data.size(), third_cid_data.size(), forth_cid_data.size(), fifth_cid_data.size(), sixth_cid_data.size() };
	auto min_size_iter = std::min_element(data_sizes.begin(), data_sizes.end());
	size_t min_size = *min_size_iter;
	data_sizes.clear();

	first_cid_data.resize(min_size);
	second_cid_data.resize(min_size);
	third_cid_data.resize(min_size);
	forth_cid_data.resize(min_size);
	fifth_cid_data.resize(min_size);
	sixth_cid_data.resize(min_size);

	system("cls");

	std::cout << "DataManipulator: Сборка TDF-файла из общего OCDF-файла со всеми сидами\n\n";

	// Собираем TDF-последовательность
	std::cout << "Собираем данные в TDF-формат...";

	vector<TDF> data(first_cid_data.size());
	for (size_t i = 0; i < data.size(); i++) {
		TDF temp(first_cid_data[i].time, first_cid_data[i].value, second_cid_data[i].value, third_cid_data[i].value, forth_cid_data[i].value, fifth_cid_data[i].value, sixth_cid_data[i].value);
		data[i] = temp;
	}

	system("cls");

	std::cout << "DataManipulator: Сборка TDF-файла из общего OCDF-файла со всеми сидами\n\n";

	// сохраняем TDF-последовательсноть
	string dump_file_path;
	std::cout << "Введите имя файла для загрузки в него данных: ";
	std::getline(std::cin, dump_file_path);
	for (size_t k = dump_file_path.find('\"'); k != dump_file_path.npos; k = dump_file_path.find('\"', k))
		dump_file_path.erase(k, 1);

	bool save_to_bin_format = false;
	while (true) {
		system("cls");

		string answer;
		std::cout << "В каком формате сохранить даныне (0 - .csv, 1 - .bin): ";
		if (!enter_menu_point(answer))
			continue;
		if (!string_symbol_to_bool(answer, save_to_bin_format))
			continue;

		break;
	}

	if (save_to_bin_format) {
		dump_file_path += ".bin";
		bindump_data(data, dump_file_path);
	}
	else {
		dump_data(data, dump_file_path);
	}
}