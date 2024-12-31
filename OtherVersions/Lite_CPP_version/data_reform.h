#pragma once
#include "data_form.h"

void cut_vector(DataVector& output_data, DataVector& data, double cut_percent, bool cut_trend = false, size_t range = 0);

void parsing_data_per_cid(DataVector& output_data, DataVector input_data, __int8 cid);

void fix_time_range(DataVector& data, int range);
