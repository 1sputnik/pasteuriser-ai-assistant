#include "WARNING.h"

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
