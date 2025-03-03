#include "StrDivider.h"

string extract_last_n_chars(const string& str, int n) {
	if (n < 0) {
		throw CommonException("Ошибка выделения последних символов строки! Значение количества меньше 0!");
	}
	else if (n >= str.size()) {
		return str;
	}
	return str.substr(str.size() - n);
}

string extract_first_n_chars(const string& str, int n) {
	if (n < 0) {
		throw CommonException("Ошибка выделения первых символов строки! Значение количества меньше 0!");
	}
	else if (n >= str.size()) {
		return str;
	}
	return str.substr(0, n);
}

vector<string> split_string(const string str, char descriptor) {
	vector<string> splited_string;
	string buffer_string = "";
	for (size_t i = 0; i < str.length(); i++) {
		if (str[i] == descriptor) {
			splited_string.push_back(buffer_string);
			buffer_string = "";
		}
		else {
			buffer_string += str[i];
		}
	}
	splited_string.push_back(buffer_string);
	return splited_string;
}

vector<string> split_string(const string str, string descriptor) {
	vector<string> splited_string;
	string buffer_string = "";
	string sub = "";
	for (size_t i = 0; i < str.length();) {
		sub = str.substr(i, descriptor.size());
		if (sub.size() != descriptor.size()) {
			if (i < str.size()) {
				buffer_string += str.substr(i);
			}
			break;
		}
		else if (sub == descriptor) {
			splited_string.push_back(buffer_string);
			buffer_string = "";
			i = i + descriptor.size();
		}
		else {
			buffer_string += str[i];
			i++;
		}
	}
	splited_string.push_back(buffer_string);
	return splited_string;
}