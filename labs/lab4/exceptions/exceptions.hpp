#pragma once

#include <stdexcept>
#include <system_error>

namespace exceptions {
class FailedToLoadLibError : public std::system_error {
public:
    using system_error::system_error;
};

class FailedToLoadSymbolError : public std::system_error {
public:
    using system_error::system_error;
};

}  // namespace exceptions