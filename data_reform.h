#pragma once
#include "data_form.h"

void cut_vector(DataVector& in_data, DataVector& data, double cut_percent, bool cut_trend = 0, size_t range = 0);

void parsing_data_per_cid(DataVector& in_data, DataVector out_data, __int8 cid);

void right_range(DataVector& data, int range);
