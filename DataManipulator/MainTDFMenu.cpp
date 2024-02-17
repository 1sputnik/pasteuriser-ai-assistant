#include "MainTDFMenu.h"

void MainTDF_Menu() {
	map<unsigned char, function<void(vector<TDF>& data)>> menu{
		make_pair('1', cut_percent_TDF_data),
		make_pair('2', cut_quantity_TDF_data),
		make_pair('4', pars_TDF_data_per_cid),
		make_pair('6', save_TDF_data_in_csv),
		make_pair('7', save_TDF_data_in_bin)
	};

	vector<TDF> data;
	data = read_TDF_file();

	while (true) {
		system("cls");

		std::cout << "������ ���������\n"
			<< "������ ������: TDF\n"
			<< "����������� ������: " << data.size() << "\n"
			<< "������ ������ ������:\t\t" << data[0] << "\n"
			<< "��������� ������ ������:\t" << data[data.size() - 1] << "\n\n";

		std::cout << "�������� ������ ������:\n"
			<< "1 - �������� �������� ������� ������\n"
			<< "2 - �������� �������� ���������� ������\n"
			<< "3 - ��������������� ������\n"
			<< "4 - ���������� ������ �� ����\n"
			<< "5 - �������� ����� ������\n"
			<< "6 - ��������� ������ � ���� ������� csv\n"
			<< "7 - ��������� ������ � �������� ����\n"
			<< "0 - ����� � ������� ����\n"
			<< "������� ����� ����: ";

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

void cut_percent_TDF_data(vector<TDF>& data) {
	while (true) {
		system("cls");

		double cut_percent;
		std::cout << "������� ������� ������, ������� ����� ��������: ";
		if (!enter_double_numeric(cut_percent, true))
			continue;
		if (cut_percent > 1.0 || cut_percent < 0.0) {
			enter_invalid_data();
			continue;
		}

		bool cut_trend;
		string answer;
		std::cout << "�������� ������� ������� (0 - ����� �������, 1 - ������ ������): ";
		if (!enter_menu_point(answer))
			continue;

		if (!string_symbol_to_bool(answer, cut_trend))
			continue;

		data = cut_percent_data(data, cut_percent, cut_trend);

		break;
	}
}

void cut_quantity_TDF_data(vector<TDF>& data) {
	while (true) {
		system("cls");

		long long cut_quantity;
		std::cout << "������� ���������� ������, ������� ����� ��������: ";
		if (!enter_int_numeric(cut_quantity))
			continue;
		if (cut_quantity <= 0) {
			enter_invalid_data();
			continue;
		}

		bool cut_trend;
		string answer;
		std::cout << "�������� ������� ������� (0 - ����� �������, 1 - ������ ������): ";
		if (!enter_menu_point(answer))
			continue;

		if (!string_symbol_to_bool(answer, cut_trend))
			continue;

		data = cut_quntity_data(data, cut_quantity, cut_trend);

		break;
	}
}

void pars_TDF_data_per_cid(vector<TDF>& data) {
	long long number;
	while (true) {
		system("cls");

		std::cout << "������� ����� ����, ������� ���������� ��������: ";
		if (!enter_int_numeric(number))
			continue;

		if (number > 6 || number < 0) {
			enter_invalid_data();
			continue;
		}

		break;
	}

	vector<OCDF> OCdata = parsing_data_per_cid(data, number);

	bool save_to_bin_format = false;
	while (true) {
		system("cls");

		string answer;
		std::cout << "� ����� ������� ��������� ������ (0 - .csv, 1 - .bin): ";
		if (!enter_menu_point(answer))
			continue;
		if (!string_symbol_to_bool(answer, save_to_bin_format))
			continue;

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

	string dump_file_path;
	std::cout << "������� ��� ����� ��� �������� � ���� ������: ";
	std::getline(std::cin, dump_file_path);
	for (size_t k = dump_file_path.find('\"'); k != dump_file_path.npos; k = dump_file_path.find('\"', k))
		dump_file_path.erase(k, 1);

	dump_data(data, dump_file_path);

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, 10);
	std::cout << "\n������ ������� ���������!\n\n";
	SetConsoleTextAttribute(hConsole, 7);
	system("pause");
}

void save_TDF_data_in_bin(vector<TDF>& data) {
	system("cls");

	string dump_file_path;
	std::cout << "������� ��� ����� ��� �������� � ���� ������: ";
	std::getline(std::cin, dump_file_path);
	for (size_t k = dump_file_path.find('\"'); k != dump_file_path.npos; k = dump_file_path.find('\"', k))
		dump_file_path.erase(k, 1);

	dump_file_path += ".bin";

	bindump_data(data, dump_file_path);

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, 10);
	std::cout << "\n������ ������� ���������!\n\n";
	SetConsoleTextAttribute(hConsole, 7);
	system("pause");
}
