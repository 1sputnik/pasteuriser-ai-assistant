#pragma once

#include "MainOCDFMenu.h"

void MainTDF_Menu();
void cut_percent_TDF_data(vector<TDF>& data);
void cut_quantity_TDF_data(vector<TDF>& data);
void cut_time_TDF_data(vector<TDF>& data);
void show_TDF_data(vector<TDF>& data);
void pars_TDF_data_per_cid(vector<TDF>& data);
void save_TDF_data_in_csv(vector<TDF>& data);
void save_TDF_data_in_bin(vector<TDF>& data);
