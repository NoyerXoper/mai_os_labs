#pragma once

#include <system_error>
#include <stdexcept>
#include <string_view>
#include <string.h>

namespace exceptions { 
class SystemError : public std::system_error {
public:
    using system_error::system_error;
    SystemError(int errcode, const std::string& what_arg): system_error(errcode, std::system_category(), what_arg) {}
};

class DoubleStartProcessError: public std::logic_error {
public:
    using logic_error::logic_error;
};
}