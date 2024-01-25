#pragma once

#include "DataFormats.h"

vector<OneCIDDataFormat> cut_percent_data(vector<OneCIDDataFormat> data, double cut_percent, bool cut_trend);
vector<OneCIDDataFormat> cut_quntity_data(vector<OneCIDDataFormat> data, size_t cut_quantity, bool cut_trend);
//
//void parsing_data_per_cid(DataVector& in_data, DataVector out_data, __int8 cid);
//
vector<OneCIDDataFormat> right_range(vector<OneCIDDataFormat> data, size_t range);