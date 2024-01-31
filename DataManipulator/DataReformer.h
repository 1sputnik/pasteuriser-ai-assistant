#pragma once

#include "DataFormats.h"

vector<OneCIDDataFormat> cut_percent_data(vector<OneCIDDataFormat> data, double cut_percent, bool cut_trend);
vector<OneCIDDataFormat> cut_quntity_data(vector<OneCIDDataFormat> data, size_t cut_quantity, bool cut_trend);
//vector<OneCIDDataFormat> cut_percent_data(vector<TableDataFormat> data, double cut_percent, bool cut_trend);
//vector<OneCIDDataFormat> cut_quntity_data(vector<TableDataFormat> data, size_t cut_quantity, bool cut_trend);

vector<OneCIDDataFormat> parsing_data_per_cid(vector<OneCIDDataFormat> in_data, short cid);

vector<OneCIDDataFormat> right_range(vector<OneCIDDataFormat> data, size_t range);
//vector<OneCIDDataFormat> right_range(vector<TableDataFormat> data, size_t range);
