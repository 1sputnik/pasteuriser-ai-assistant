#include "IOExceptions.h"

IOException::IOException(std::string message, IO_TYPES io_type) {
	this->message = message;
	this->io_type = io_type;
}

std::string IOException::get_message() const {
	switch(io_type) {
	case IN_STD:
		return "������ ������! " + this->message;
	case OUT_STD:
		return "������ ������! " + this->message;
	case IN_FILE:
		return "������ �������� ������ �� �����! " + this->message;
	case OUT_FILE:
		return "������ ���������� ������ � ����! " + this->message;
	default:
		return "����������� ������! ���-�� ����� �� ���!";
	}
}