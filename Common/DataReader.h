#pragma once

#include "DataFormats.h"

vector<OneCIDDataFormat> load_OCDF_data(string file_name, size_t size = 0);

vector<OneCIDDataFormat> binload_OCDF_data(string file_name, size_t size = 0);

vector<OneCIDDataFormat> read_OCDF_file(string special_msg = "");


vector<TableDataFormat> load_TDF_data(string file_name, size_t size = 0);

vector<TableDataFormat> binload_TDF_data(string file_name, size_t size = 0);

vector<TableDataFormat> read_TDF_file(string special_msg = "");
