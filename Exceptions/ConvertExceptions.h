#pragma once

#include "Exceptions.h"

class StrConvertException : public GeneralException
{
public:
    StrConvertException(std::string message, std::string failed_str);
    virtual std::string get_message() const final;
private:
    std::string failed_str;
};
