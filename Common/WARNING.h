#pragma once

#if defined(_WIN32) || defined(_WIN64)
#include <Windows.h>
#endif
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>


bool have_promlems_with_opening_file(std::ifstream& file, std::string file_name);

bool have_promlems_with_reading_data(std::ifstream& file, std::string& str);


bool enter_menu_point(std::string& answer);

bool enter_int_numeric(long long& numeric);

bool enter_double_numeric(double& numeric, bool invers_descriptor = false);

bool string_symbol_to_bool(std::string str, bool& answer);


bool is_double_numeric(std::string str);

bool is_int_numeric(std::string str);


std::string extractLastNChars(std::string const& str, int n);

std::vector<std::string> split_string(std::string str, char descriptor);

size_t check_quantity_data_lines_in_file(std::ifstream& file, std::string file_name);

bool check_OCDF_in_file(std::string file_name);

bool check_TDF_in_file(std::string file_name);


void msg_warning(std::string msg = "\nВнимание! Что-то пошло не так!\n\n", short color = 7);
