#-------------------------------------------------------------------------------
# Zephyr Example Application
#
# Copyright (c) 2021 Nordic Semiconductor ASA
# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.13.1)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

project(app LANGUAGES C)

target_include_directories(app PRIVATE src/include)
target_include_directories(app PRIVATE src/lib/usb)

# Files from application
FILE(GLOB app_sources src/*.c)
target_sources(app PRIVATE ${app_sources})
target_sources(app PRIVATE src/lib/USB/cdc.c)
