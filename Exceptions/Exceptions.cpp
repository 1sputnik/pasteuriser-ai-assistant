#include "Exceptions.h"

void GeneralException::on_pause() {
    this->do_pause = true;
}

void GeneralException::off_pause() {
    this->do_pause = false;
}

void GeneralException::check_error() {
    std::cout << this->get_message();
    if (this->do_pause) {
        std::cout << "\n\nДля продолжения нажмите любую клавишу...\n\n";
        _getch();
    }
}

GeneralException::GeneralException() {
    this->message = "Ошибка! Что-то пошло не так!";
    this->do_pause = true;
}

std::string GeneralException::get_message() const {
    return message;
}

CommonException::CommonException(std::string str) {
    this->message = str;
}

std::string CommonException::get_message() const {
    return this->message;
}