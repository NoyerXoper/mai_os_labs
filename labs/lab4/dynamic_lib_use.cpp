#include <iostream>

#include <library_loader.hpp>

constexpr std::string_view DEF_EXP_LIB = "/home/noyer/Desktop/labs/os/build/labs/lab4/libs/libdefexp.so";
constexpr std::string_view TAYLOR_EXP_LIB = "/home/noyer/Desktop/labs/os/build/labs/lab4/libs/libtaylorexp.so";

constexpr std::string_view BINARY_TRANSLATION_LIB = "/home/noyer/Desktop/labs/os/build/labs/lab4/libs/libbinarytrans.so";
constexpr std::string_view TERNARY_TRANSLATION_LIB = "/home/noyer/Desktop/labs/os/build/labs/lab4/libs/libternarytrans.so";

using EFuncTypePtr = float(*)(int);
using TranslationFuncPtr = char*(*)(long);
using DeleterOfTranslationFuncPtr = void(*)(char*);

int main() {
    bool is_using_first = true;

    sys::LibraryLoader expr_loader(DEF_EXP_LIB);
    sys::LibraryLoader translation_loader(BINARY_TRANSLATION_LIB);

    EFuncTypePtr E = expr_loader.GetSymbol<EFuncTypePtr>("E");
    TranslationFuncPtr translate = translation_loader.GetSymbol<TranslationFuncPtr>("translate");
    DeleterOfTranslationFuncPtr delete_translated = translation_loader.GetSymbol<DeleterOfTranslationFuncPtr>("DeleteTranslatedString");

    int action;
    for(;;) {
        std::cin >> action;
        switch(action) {
            case 0: {
                is_using_first = !is_using_first;
                expr_loader = sys::LibraryLoader(is_using_first ? DEF_EXP_LIB : TAYLOR_EXP_LIB);
                translation_loader = sys::LibraryLoader(is_using_first ? BINARY_TRANSLATION_LIB : TERNARY_TRANSLATION_LIB);
                E = expr_loader.GetSymbol<EFuncTypePtr>("E");
                translate = translation_loader.GetSymbol<TranslationFuncPtr>("translate");
                delete_translated = translation_loader.GetSymbol<DeleterOfTranslationFuncPtr>("DeleteTranslatedString");
                break;
            }
            case 1: {
                int x;
                std::cin >> x;
                std::cout << (*E)(x) << '\n';
                break;
            }
            case 2: {
                long number;
                std::cin >> number;
                char* translated = (*translate)(number);
                std::cout << translated << '\n';
                (*delete_translated)(translated);
                break;
            }
            default: {
                std::cout << "Unknown action, exiting\n";
                return 0;
            }
        }
    }
}