#include "ModelCreater.h"

using std::string;
using std::function;
using std::map;
using std::make_pair;

void createVanilaLSTM();
void createTInLSTM();
void train_model(OCDFNeuron* model);
void save_model(OCDFNeuron* model);
char define_model_type_per_file_name(string file_name);
char define_model_type_per_class_type(OCDFNeuron* file_name);
void model_menu(OCDFNeuron* model);

void createModel() {
	map<unsigned char, function<void()>> menu{
		make_pair('1', createVanilaLSTM),
		make_pair('2', createTInLSTM)
	};

	while (true) {
		system("cls");

		std::cout << "NNTrainer: Выбор  модели\n\n";

		std::cout << "Выберите тип нейронной сети:\n"
			<< "1 - Стандартная LSTM с одним входом\n"
			<< "2 - Стандартная LSTM с двумя входами\n"
			<< "0 - выход в главное меню \n"
			<< "Введите пункт меню: ";

		string answer;
		if (!enter_menu_point(answer))
			continue;

		if (answer[0] == '0')
			return;

		try {
			menu.at(answer[0])();
			return;
		}
		catch (...) {
			msg_warning("\nОшибка ввода! Неверный пункт меню!\n\n");
		}
	}
}

int enter_plus_integer_numeric(std::string title_msg = "") {
	long long numeric;
	while (true) {
		system("cls");

		std::cout << title_msg;

		std::cout << "Введите значение: ";
		if (!enter_int_numeric(numeric))
			continue;
		if (numeric <= 0) {
			msg_warning("\nОшибка ввода данных! Введено недопустимое число!\n\n");
			continue;
		}

		break;
	}
	return int(numeric);
}

double enter_plus_real_numeric(std::string title_msg = "") {
	double numeric;
	while (true) {
		system("cls");

		std::cout << title_msg;

		std::cout << "Введите значение: ";
		if (!enter_double_numeric(numeric))
			continue;
		if (numeric <= 0) {
			msg_warning("\nОшибка ввода данных! Введено недопустимое число!\n\n");
			continue;
		}

		break;
	}
	return numeric;
}

void createVanilaLSTM() {
	int input_range = enter_plus_integer_numeric("NNTrainer: Создание VanilaLSTM: ввод входной размерности\n\n");
	int hidden_range = enter_plus_integer_numeric("NNTrainer: Создание VanilaLSTM: ввод скрытой размерности\n\n");
	int output_range = enter_plus_integer_numeric("NNTrainer: Создание VanilaLSTM: ввод выходной размерности\n\n");
	int epochs = enter_plus_integer_numeric("NNTrainer: Создание VanilaLSTM: ввод количество эпох\n\n");
	double learning_rate = enter_plus_real_numeric("NNTrainer: Создание VanilaLSTM: ввод скорости обучения\n\n");
	double target_error = enter_plus_real_numeric("NNTrainer: Создание VanilaLSTM: ввод минимальной ошибки\n\n");

	std::cout << "NNTrainer: Создание VanilaLSTM на один вход\n\nИдём настройка сети...\n";

	AbstractActivator* main_activator = new Logistic();
	AbstractActivator* additional_activator = new Tanh();

	OCDFNeuron* voc_lstm = new OCDFVanilaLSTM(input_range, hidden_range, output_range, main_activator, additional_activator);
	voc_lstm->set_epochs(epochs);
	voc_lstm->set_learning_rate(learning_rate);
	voc_lstm->set_target_error(target_error);

	model_menu(voc_lstm);
}

void createTInLSTM() {
	int input_range = enter_plus_integer_numeric("NNTrainer: Создание VanilaLSTM: ввод входной размерности\n\n");
	int hidden_range = enter_plus_integer_numeric("NNTrainer: Создание VanilaLSTM: ввод скрытой размерности\n\n");
	int output_range = enter_plus_integer_numeric("NNTrainer: Создание VanilaLSTM: ввод выходной размерности\n\n");
	int epochs = enter_plus_integer_numeric("NNTrainer: Создание VanilaLSTM: ввод количество эпох\n\n");
	double learning_rate = enter_plus_real_numeric("NNTrainer: Создание VanilaLSTM: ввод скорости обучения\n\n");
	double target_error = enter_plus_real_numeric("NNTrainer: Создание VanilaLSTM: ввод минимальной ошибки\n\n");

	std::cout << "NNTrainer: Создание VanilaLSTM на два входа\n\nИдём настройка сети...\n";

	AbstractActivator* main_activator = new Logistic();
	AbstractActivator* additional_activator = new Tanh();

	OCDFNeuron* twi_lstm = new TInOCDFVanilaLSTM(input_range, hidden_range, output_range, main_activator, additional_activator);
	twi_lstm->set_epochs(epochs);
	twi_lstm->set_learning_rate(learning_rate);
	twi_lstm->set_target_error(target_error);

	model_menu(twi_lstm);
}

void test_model(OCDFNeuron* model) {
	system("cls");
	vector<OCDF> data = read_OCDF_file("NNTrainer: Загрузка данных для тестирования модели\n\n");
	system("cls");

	std::cout << "NNTrainer: Тестирование модели\n\n";

	vector<OCDF> predicts = model->predict(data);

	while (true) {
		system("cls");
		std::cout << "NNTrainer: Визуализирование результатов тестирования модели\n\n";

		bool do_visual;
		string answer;
		std::cout << "\nВизуализировать данные (0 - нет, 1 - да): ";
		if (!enter_menu_point(answer))
			continue;
		if (!string_symbol_to_bool(answer, do_visual))
			continue;

		if (do_visual) {
			system("cls");
			std::cout << "NNTrainer: Сохранение данных во временные файлы\n";
			dump_data(data, "PyVisualisation\\real_data.csv");
			dump_data(predicts, "PyVisualisation\\predict_data.csv");
			std::cout << "Данные сохранены! Ожидайте визуализацию\n\n" 
				<< "Чтобы выйти в меню, закройте окно визуализации!\n";

			char class_type = define_model_type_per_class_type(model);
			switch (class_type)
			{
			case 'o': system("py PyVisualisation\\VanilaLSTM_Test.py PyVisualisation\\real_data.csv PyVisualisation\\predict_data.csv"); break;
			case 't': system("py PyVisualisation\\TInVanilaLSTM_Test.py PyVisualisation\\real_data.csv PyVisualisation\\predict_data.csv"); break;
			default:
				delete model;
				msg_warning("\n\nОшибка определения модели! Невозможно определить тип модели!\n");
				return;
			}
			system("pause");
		}

		return;
	}
}

void train_model(OCDFNeuron* model) {
	system("cls");
	vector<OCDF> data = read_OCDF_file("NNTrainer: Загрузка данных для обучения модели\n\n");
	system("cls");

	std::cout << "NNTrainer: Обучение модели\n\n";

	if (!model->configured_scaler) {
		vector<OCDF> temp_data = parsing_data_per_cid(data, data[0].cid);
		model->scaler.configure(temp_data, 0.01, true);
		model->scaler.set_scale(0, 1);
		model->configured_scaler = true;
		temp_data.clear();
	}

	model->fit(data);
}

char define_model_type_per_class_type(OCDFNeuron* model) {
	std::string name_of_class_type = model->get_class_name();
	if (name_of_class_type == "VanilaLSTM")
		return 'o';
	if (name_of_class_type == "TInVanilaLSTM")
		return 't';
	return 'n';
}

void save_model(OCDFNeuron* model) {
	system("cls");
	std::cout << "NNTrainer: Сохранение модели\n\n";

	string save_file_path;
	std::cout << "Введите имя файла для загрузки в него модели: ";
	std::getline(std::cin, save_file_path);
	for (size_t k = save_file_path.find('\"'); k != save_file_path.npos; k = save_file_path.find('\"', k))
		save_file_path.erase(k, 1);

	string postfix_file_name = "";
	char class_type = define_model_type_per_class_type(model);
	switch (class_type)
	{
	case 'o': postfix_file_name = ".vlstm"; break;
	case 't': postfix_file_name = ".tlstm"; break;
	default:
		delete model;
		msg_warning("\n\nОшибка определения модели! Невозможно определить тип модели!\n");
		return;
	}

	model->save_model(save_file_path + postfix_file_name);
}

char define_model_type_per_file_name(string file_name) {
	if (file_name.substr(file_name.size() - 6, file_name.size()) == ".vlstm")
		return 'o';
	if (file_name.substr(file_name.size() - 6, file_name.size()) == ".tlstm")
		return 't';
	return 'n';
}

void loadModel() {
	system("cls");
	std::cout << "NNTrainer: Загрузка модели\n\n";

	string load_file_path;
	std::cout << "Введите имя файла для загрузки из него модели: ";
	std::getline(std::cin, load_file_path);
	for (size_t k = load_file_path.find('\"'); k != load_file_path.npos; k = load_file_path.find('\"', k))
		load_file_path.erase(k, 1);

	char model_type = define_model_type_per_file_name(load_file_path);
	OCDFNeuron* model = nullptr;
	switch (model_type) {
	case 'o': model = new OCDFVanilaLSTM(); break;
	case 't': model = new TInOCDFVanilaLSTM(); break;
	case 'n': 
	default:
		delete model;
		msg_warning("\n\nОшибка определения модели! Невозможно определить тип модели!\n");
		return;
	}

	model->load_model(load_file_path);

	AbstractActivator* main_activator = new Logistic();
	AbstractActivator* additional_activator = new Tanh();

	model->set_main_activator(additional_activator);
	model->set_additional_activator(additional_activator);

	model_menu(model);
}

void change_epochs(OCDFNeuron* model) {
	int epochs = enter_plus_integer_numeric("NNTrainer: Изменение модели: ввод количества эпох\n\n");
	model->set_epochs(epochs);
}

void change_target_error(OCDFNeuron* model) {
	double target_error = enter_plus_real_numeric("NNTrainer: Изменение модели: ввод целевой ошибки\n\n");
	model->set_target_error(target_error);
}

void change_learning_rate(OCDFNeuron* model) {
	double learning_rate = enter_plus_real_numeric("NNTrainer: Изменение модели: ввод скорости обучения\n\n");
	model->set_learning_rate(learning_rate);
}

void model_menu(OCDFNeuron* model) {
	map<unsigned char, function<void(OCDFNeuron* model)>> menu{
		make_pair('1', train_model),
		make_pair('2', save_model),
		make_pair('3', test_model),
		make_pair('4', change_epochs),
		make_pair('5', change_learning_rate),
		make_pair('6', change_target_error)
	};

	while (true) {
		system("cls");

		std::cout << "NNTrainer: Меню настройки модели\n\n";

		std::cout << "Модель: " << model->get_class_name() << "\n"
			<< "Входная размерность: " << model->get_input_range() << "\n"
			<< "Скрытая размерность: " << model->get_hidden_range() << "\n"
			<< "Выходная размерность: " << model->get_output_range() << "\n"
			<< "Количество эпох: " << model->get_epochs() << "\n"
			<< "Скорость обучения: " << model->get_learning_rate() << "\n"
			<< "Целевая ошибка: " << model->get_target_error() << "\n"
			<< "Последняя ошибка сети: " << model->get_last_error() << "\n\n";

		std::cout << "Выберите тип нейронной сети:\n"
			<< "1 - Обучить сеть\n"
			<< "2 - Сохранить сеть\n"
			<< "3 - Тестировать сеть\n"
			<< "4 - Изменить количество эпох\n"
			<< "5 - Изменить скорость обучения\n"
			<< "6 - Изменить целевую ощибку\n"
			<< "0 - выход из программы\n"
			<< "Введите пункт меню: ";

		string answer;
		if (!enter_menu_point(answer))
			continue;

		if (answer[0] == '0') {
			delete model;
			return;
		}

		try {
			menu.at(answer[0])(model);
		}
		catch (...) {
			msg_warning("\nОшибка ввода! Неверный пункт меню!\n\n");
		}
	}
}
