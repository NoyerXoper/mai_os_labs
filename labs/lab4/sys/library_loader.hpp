#pragma once

#include <memory>

namespace sys {
struct ModuleDummy;
using Module = std::unique_ptr<ModuleDummy>;

class LibraryLoader {
public:
    LibraryLoader(std::string_view relative_path);

    LibraryLoader(LibraryLoader&& other) noexcept;
    LibraryLoader& operator=(LibraryLoader&& other) noexcept;

    ~LibraryLoader() noexcept;

    template <typename FuncTypePtr>
    FuncTypePtr GetSymbol(std::string_view symbol);
    Module lib;
};
}  // namespace sys

#ifdef __linux__
#    include <library_loader_unix.ipp>
#endif