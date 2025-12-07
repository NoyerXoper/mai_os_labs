#include "library_loader.hpp"

#include <dlfcn.h>

#include "exceptions.hpp"

namespace sys {
struct ModuleDummy {
    void* handle;
};

LibraryLoader::LibraryLoader(std::string_view relative_path) {
    void* handle = dlopen(relative_path.data(), RTLD_LAZY);
    if (!handle) {
        throw exceptions::FailedToLoadLibError(0, std::system_category(),
                                               dlerror());
    }
    lib = std::make_unique<ModuleDummy>(handle);
}

LibraryLoader::LibraryLoader(LibraryLoader&& other) noexcept
    : lib(std::move(other.lib)) {}

LibraryLoader& LibraryLoader::operator=(LibraryLoader&& other) noexcept {
    LibraryLoader temp = std::move(other);
    std::swap(lib, temp.lib);
    return *this;
}

template <class FuncTypePtr>
FuncTypePtr LibraryLoader::GetSymbol(std::string_view symbol_name) {
    dlerror();
    void* symbol = dlsym(lib->handle, symbol_name.data());
    if (!symbol) {
        throw exceptions::FailedToLoadSymbolError(0, std::system_category(),
                                                  dlerror());
    }
    return reinterpret_cast<FuncTypePtr>(symbol);
}

LibraryLoader::~LibraryLoader() noexcept {
    if (lib && lib->handle) {
        dlclose(lib->handle);
    }
}

}  // namespace sys