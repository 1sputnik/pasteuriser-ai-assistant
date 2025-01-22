#pragma once

#include "StrDivider.h"

bool string_to_double(string str, double& numeric, bool invers_descriptor = false);

bool string_to_integer(string str, int& numeric);

bool string_to_bool(string str, bool& answer);

bool is_double(const string& str);

bool is_integer(const string& str);

bool is_target_format_data(const string& str);

bool is_menu_point(const string& answer);
