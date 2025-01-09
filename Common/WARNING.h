#pragma once

#include "StrConverter.h"


bool enter_int_numeric(long long& numeric);

bool enter_double_numeric(double& numeric, bool invers_descriptor = false);

void msg_warning(std::string msg = "\nВнимание! Что-то пошло не так!\n", short color = 7);
