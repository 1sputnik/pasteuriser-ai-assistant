#include "WARNING.h"

bool enter_int_numeric(long long& numeric) {
	string buffer;
	getline(cin, buffer);
	if (buffer == "") {
		msg_warning("\nОшибка ввода данных! Введённое значение не является целым числом или число отрицательное!\n\n");
		return false;
	}
	else if (is_integer(buffer)) {
		numeric = stol(buffer);
	}
	else {
		msg_warning("\nОшибка ввода данных! Введённое значение не является целым числом или число отрицательное!\n\n");
		return false;
	}
	return true;
}

bool enter_double_numeric(double& numeric, bool invers_descriptor) {
	string buffer;
	getline(cin, buffer);
	if (is_double(buffer)) {
		if (invers_descriptor)
			replace(buffer.begin(), buffer.end(), '.', ',');
		numeric = stod(buffer);
	}
	else {
		msg_warning("\nОшибка ввода данных! Введённое значение не является действительным числом!\n\n");
		return false;
	}
	return true;
}

void msg_warning(string msg, short color) {
#if defined(_WIN32) || defined(_WIN64)
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, color);
#endif

#ifndef PAIATESTS
	std::cout << msg;
#endif

#if defined(_WIN32) || defined(_WIN64)
	SetConsoleTextAttribute(hConsole, 7);
#endif

#ifndef PAIATESTS
	std::cout << "\nДля продолжения нажмите любую клавишу...\n\n";
	_getch();
#endif
}
