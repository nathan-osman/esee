cmake_minimum_required(VERSION 3.2.0 FATAL_ERROR)
project(esee)

find_package(PkgConfig REQUIRED)
find_package(Qt5Widgets 5.4 REQUIRED)

pkg_check_modules(LIBEXIF REQUIRED libexif)

set(CMAKE_AUTOMOC ON)

add_subdirectory(data)
add_subdirectory(src)
