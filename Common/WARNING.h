#pragma once

#include "StrConverter.h"


bool enter_int_numeric(long long& numeric);

bool enter_double_numeric(double& numeric, bool invers_descriptor = false);

void msg_warning(std::string msg = "\n��������! ���-�� ����� �� ���!\n", short color = 7);
