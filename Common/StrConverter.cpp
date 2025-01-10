#include "StrConverter.h"


vector<char> integer_symbols{ '1', '2', '3', '4', '5', '6', '7', '8', '9', '0' };
vector<char> float_symbols{ '1', '2', '3', '4', '5', '6', '7', '8', '9', '.', '0' };
vector<char> format_symbols{ '1', '2', '3', '4', '5', '6', '7', '8', '9', '.', '0', ';', '\n', '\r' };


bool string_to_double(string str, double& numeric, bool invers_descriptor) {
	if (str.length() > 11) {
		return false;
	}
	if (is_double(str)) {
		if (invers_descriptor)
			replace(str.begin(), str.end(), '.', ',');
		numeric = stod(str);
		return true;
	}
	return false;
}

bool string_to_integer(string str, long long& numeric) {
	if (str.length() > 9) {
		return false;
	}
    else if (is_integer(str)) {
		numeric = stol(str);
		return true;
	}
	return false;
}

bool string_to_bool(string str, bool& answer) {
	if (str == "0") {
		answer = false;
	}
	else if (str == "1") {
		answer = true;
	}
	else {
		return false;
	}
	return true;
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
