#pragma once

#include <iostream>
#include <string>
#include <conio.h>

class CommonException
{
public:
    CommonException();
    void on_pause();
    void off_pause();
    void check_error();
    virtual std::string get_message() const;
protected:
    std::string message;
    bool do_pause;
};