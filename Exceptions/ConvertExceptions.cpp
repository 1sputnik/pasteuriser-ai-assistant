#include "ConvertExceptions.h"

StrConvertException::StrConvertException(std::string message, std::string failed_str) {
	this->message = message;
	this->failed_str = message;
}

std::string StrConvertException::get_message() const {
	return "Ошибка конвертации! " + this->message + " Исходные данные: " + this->failed_str + ".";
}