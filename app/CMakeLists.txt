#-------------------------------------------------------------------------------
# Zephyr Example Application
#
# Copyright (c) 2021 Nordic Semiconductor ASA
# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.13.1)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

project(app LANGUAGES C)

add_subdirectory(src/bluetooth)
add_subdirectory(src/usb)
add_subdirectory(src/display)
add_subdirectory(src/sd)
add_subdirectory(src/rtc)

target_include_directories(app PRIVATE src)

# Files from application
FILE(GLOB app_sources src/*.c)
target_sources(app PRIVATE ${app_sources})
