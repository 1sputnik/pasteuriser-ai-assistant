#include "IOExceptions.h"

IOException::IOException(std::string message, IO_TYPES io_type) {
	this->message = message;
	this->io_type = io_type;
}

std::string IOException::get_message() const {
	switch(io_type) {
	case IN_STD:
		return "Ошибка записи! " + this->message;
	case OUT_STD:
		return "Ошибка вывода! " + this->message;
	case IN_FILE:
		return "Ошибка загрузки данных из файла! " + this->message;
	case OUT_FILE:
		return "Ошибка сохранения данных в файл! " + this->message;
	default:
		return "Неизвестная ошибка! Что-то пошло не так!";
	}
}