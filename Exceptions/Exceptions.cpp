#include "Exceptions.h"

void CommonException::on_pause() {
    this->do_pause = true;
}

void CommonException::off_pause() {
    this->do_pause = false;
}

void CommonException::check_error() {
    std::cout << this->get_message();
    if (this->do_pause) {
        std::cout << "\n\n��� ����������� ������� ����� �������...\n\n";
        _getch();
    }
}

CommonException::CommonException() {
    this->message = "������! ���-�� ����� �� ���!";
    this->do_pause = true;
}

std::string CommonException::get_message() const {
    return message;
}