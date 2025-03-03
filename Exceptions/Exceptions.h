#pragma once

#include <iostream>
#include <string>
#include <conio.h>

class GeneralException : public std::exception
{
public:
    GeneralException();
    void on_pause();
    void off_pause();
    void check_error();
    virtual std::string get_message() const;
protected:
    std::string message;
    bool do_pause;
};

class CommonException : public GeneralException
{
public:
    CommonException(std::string str);
    virtual std::string get_message() const final;
};