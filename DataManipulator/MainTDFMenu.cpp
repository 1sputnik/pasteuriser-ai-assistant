#include "MainTDFMenu.h"

void MainTDF_Menu() {
	map<unsigned char, function<void(vector<TDF>& data)>> menu{
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
