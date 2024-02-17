#pragma once

#include <map>
#include <functional>
#include <filesystem>
#include "../Common/DataReformer.h"
#include "../Common/DataFormats.h"
#include "../Common/WARNING.h"
#include "../Common/DataReader.h"
#include "../Common/DataDumper.h"

using std::map;
using std::function;
using std::vector;
using std::make_pair;

void MainTDF_Menu();
void cut_percent_TDF_data(vector<TDF>& data);
//void cut_quantity_TDF_data(vector<OCDF>& data);
//void show_TDF_data(vector<OCDF>& data);
//void pars_TDF_data_per_cid(vector<OCDF>& data);
//void add_more_data(vector<OCDF>& data);
void save_TDF_data_in_csv(vector<TDF>& data);
void save_TDF_data_in_bin(vector<TDF>& data);
