#pragma once 

#include "Exceptions.h"

enum IO_TYPES {
    IN_STD,
    OUT_STD,
    IN_FILE,
    OUT_FILE
};

class IOException : public CommonException
{
public:
    IOException(std::string message, IO_TYPES io_type);
    virtual std::string get_message() const final;
private:
    IO_TYPES io_type;
};