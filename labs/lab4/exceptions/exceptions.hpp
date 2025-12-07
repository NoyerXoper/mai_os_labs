#pragma once

#include <stdexcept>
#include <system_error>

namespace exceptions {
class FailedToLoadLibError : public std::system_error {
public:
    using system_error::system_error;
    //FailedToLoadLibError(int ev, const std::error_category& cat, const char* what) : system_error(ev, cat, what) {}
};

class FailedToLoadSymbolError : public std::system_error {
public:
    using system_error::system_error;
    //FailedToLoadSymbolError(int ev, const std::error_category& cat, const char* what) : system_error(ev, cat, what) {}
};

}