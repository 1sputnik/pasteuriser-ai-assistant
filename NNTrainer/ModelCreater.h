#pragma once

#include <iostream>
#include <map>
#include <functional>
#include <typeinfo>
#include "../NeuroModels/NeuroSkeletons.h"
#include "../NeuroModels/VanilaLSTM.h"
#include "../NeuroModels/TInVanilaLSTM.h"
#include "../Common/DataDumper.h"
#include "../Common/DataReader.h"
#include "../Common/DataFormats.h"
#include "../Common/WARNING.h"

void createOCDFModel();
void loadOCDFModel();