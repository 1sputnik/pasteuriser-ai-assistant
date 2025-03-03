#pragma once

#include "includes.h"


string extract_last_n_chars(const string& str, int n);

string extract_first_n_chars(const string& str, int n);

vector<string> split_string(const string str, char descriptor);

vector<string> split_string(const string str, string descriptor);