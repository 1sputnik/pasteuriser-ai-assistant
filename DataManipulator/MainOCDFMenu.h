#pragma once

#include "TDFCreator.h"

void MainOCDF_Menu();
void cut_percent_OCDF_data(vector<OCDF>& data);
void cut_quantity_OCDF_data(vector<OCDF>& data);
void cut_time_OCDF_data(vector<OCDF>& data);
void right_time_OCDF(vector<OCDF>& data);
void show_OCDF_data(vector<OCDF>& data);
void pars_OCDF_data_per_cid(vector<OCDF>& data);
void add_more_data(vector<OCDF>& data);
void save_OCDF_data_in_csv(vector<OCDF>& data);
void save_OCDF_data_in_bin(vector<OCDF>& data);
