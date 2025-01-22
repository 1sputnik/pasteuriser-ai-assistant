#pragma once

#include "DataFormats.h"

size_t check_quantity_data_lines_in_file(string file_name);

vector<OneCIDDataFormat> load_OCDF_data(string file_name, size_t size = 0);

vector<OneCIDDataFormat> binload_OCDF_data(string file_name, size_t size = 0);

vector<OneCIDDataFormat> read_OCDF_file(string special_msg = "");


vector<TableDataFormat> load_TDF_data(string file_name, size_t size = 0);

vector<TableDataFormat> binload_TDF_data(string file_name, size_t size = 0);

vector<TableDataFormat> read_TDF_file(string special_msg = "");
