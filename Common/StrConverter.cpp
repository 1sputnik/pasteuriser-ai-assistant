#include "StrConverter.h"

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
	vector<char> numerics{ '1', '2', '3', '4', '5', '6', '7', '8', '9', '.', '0' };
	for (size_t i = 0; i < str.size(); i++) {
		if (std::find(numerics.begin(), numerics.end(), str[i]) == numerics.end())
			return false;
	}
	return true;
}

bool is_integer(const string& str) {
	if (str.length() == 0) {
		return false;
	}
	vector<char> numerics{ '1', '2', '3', '4', '5', '6', '7', '8', '9', '0' };
	for (size_t i = 0; i < str.size(); i++) {
		if (std::find(numerics.begin(), numerics.end(), str[i]) == numerics.end())
			return false;
	}
	return true;
}

bool is_target_format_data(const string& str) {
	if (str.length() == 0) {
		return false;
	}
	vector<char> numerics{ '1', '2', '3', '4', '5', '6', '7', '8', '9', '.', '0', ';', '\n', '\r' };
	for (size_t i = 0; i < str.size(); i++) {
		if (std::find(numerics.begin(), numerics.end(), str[i]) == numerics.end())
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
