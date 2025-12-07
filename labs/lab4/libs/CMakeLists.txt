cmake_minimum_required(VERSION 3.10)
project(shared_libs LANGUAGES CXX)

add_compile_options(-std=c++20)

include_directories(exp_cal translation)

add_library(defexp SHARED exp_cal/definition_based.cpp)
add_library(taylorexp SHARED exp_cal/taylor_based.cpp)


add_library(binarytrans SHARED translation/translation_binary.cpp)
add_library(ternarytrans SHARED translation/translation_ternary.cpp)