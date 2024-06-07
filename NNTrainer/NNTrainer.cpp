#include "NNTrainer.h"

using namespace std;

void MainDataFormat_Menu();

int main()
{
	SetConsoleCP(CP_UTF8);
	SetConsoleOutputCP(CP_UTF8);

	MainDataFormat_Menu();
	return 0;
}

void show_info();

void MainDataFormat_Menu() {
	map<unsigned char, function<void()>> menu{
		make_pair('1', loadModel),
		make_pair('2', createModel),
		make_pair('3', show_info)
	};

	while (true) {
		system("cls");

		std::cout << "NNTrainer: главное меню\n\n";

		std::cout << "Выберите пункт меню:\n"
			<< "1 - загрузить модель\n"
			<< "2 - создать модель\n"
			<< "3 - информация о программе\n"
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
	std::cout << "NNTrainer - это модуль, который предназначен для создания, обучения и тестирования модели нейронной сети для прогнозирования временых рядов.\n\n"

		<< "Этот модуль позволяет:\n\n"

		<< "1. Создать модель на различных архитектурах нейронной сети\n"
		<< "2. Обучить и дообучения созданной или загруженной модель нейронной сети на данных, созданных с помочью DataManipulator\n"
		<< "3. Тестировать модель нейронной, чтобы предварительно проверить, как работает сеть\n"
		<< "4. Настраивать модель как во время создания, так и после\n\n";

	system("pause");
}
