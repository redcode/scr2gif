# scr2gif
# Copyright (C) 2022 Manuel Sainz de Baranda y Goñi.
# Released under the terms of the GNU General Public License v3.

cmake_minimum_required(VERSION 3.14)

project(scr2gif
	LANGUAGES C
	DESCRIPTION "Small utility for converting ZX Spectrum screens to GIF format")

include(GNUInstallDirs)
find_package(GIF REQUIRED)
add_executable(${PROJECT_NAME} "${CMAKE_CURRENT_SOURCE_DIR}/sources/scr2gif.c")

set_target_properties(
	${PROJECT_NAME} PROPERTIES
	C_STANDARD 99
	C_STANDARD_REQUIRED NO)

target_link_libraries(${PROJECT_NAME} PRIVATE GIF::GIF)
install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}")

# CMakeLists.txt EOF
