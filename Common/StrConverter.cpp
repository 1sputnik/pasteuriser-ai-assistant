#include "StrConverter.h"


static vector<char> integer_symbols{ '1', '2', '3', '4', '5', '6', '7', '8', '9', '0' };
static vector<char> float_symbols{ '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '.'};
static vector<char> format_symbols{ '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '.', ';', '\n', '\r' };


bool string_to_double(string str, double& numeric, bool invers_descriptor) {
	if (str.length() > 11) {
		return false;
	}
	if (is_double(str)) {
		if (invers_descriptor)
			replace(str.begin(), str.end(), '.', ',');
		try {
			numeric = stod(str);
		}
		catch (...) {
			numeric = 0;
			throw StrConvertException("Не удалось преобразовать std::string в double!", str);
		}
		return true;
	}
	return false;
}

bool string_to_integer(string str, int& numeric) {
	if (str.length() > 9) {
		return false;
	}
	else if (is_integer(str)) {
		try {
			numeric = int(stol(str));
		}
		catch (...) {
			numeric = 0;
			throw StrConvertException("Не удалось преобразовать std::string в int!", str);
		}
		return true;
	}
	return false;
}

bool string_to_short(string str, short& numeric) {
	string max_short_value_str = "32767";
	if (str.length() > 5) {
		return false;
	}
	else if (is_integer(str)) {
		try{
			numeric = short(stol(str));
		}
		catch (...) {
			numeric = 0;
			throw StrConvertException("Не удалось преобразовать std::string в short!", str);
		}
		return true;
	}
	return false;
}

bool string_to_bool(string str, bool& answer) {
	if ( (str == "0") || (str == "false") ) {
		answer = false;
		return true;
	}
	else if ( (str == "1") || (str == "true") ) {
		answer = true;
		return true;
	}
	else {
		return false;
	}
}

bool is_double(const string& str) {
	if (str.length() == 0) {
		return false;
	}
	for (size_t i = 0; i < str.size(); i++) {
		if (std::find(float_symbols.begin(), float_symbols.end(), str[i]) == float_symbols.end())
			return false;
	}
	return true;
}

bool is_integer(const string& str) {
	if (str.length() == 0) {
		return false;
	}
	for (size_t i = 0; i < str.size(); i++) {
		if (std::find(integer_symbols.begin(), integer_symbols.end(), str[i]) == integer_symbols.end())
			return false;
	}
	return true;
}

bool is_target_format_data(const string& str) {
	if (str.length() == 0) {
		return false;
	}
	for (size_t i = 0; i < str.size(); i++) {
		if (std::find(format_symbols.begin(), format_symbols.end(), str[i]) == format_symbols.end())
			return false;
	}
	return true;
}

bool is_menu_point(const string& answer) {
	if (answer.size() != 1 || !is_integer(answer)) {
		return false;
	}
	return true;
}
