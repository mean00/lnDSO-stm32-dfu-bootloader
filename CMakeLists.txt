#-----------------------------------------------------------------------------
#
#-----------------------------------------------------------------------------
set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)

cmake_minimum_required(VERSION 3.13)
MESSAGE(STATUS "=====================")
MESSAGE(STATUS "|Starting BootLoader|")
MESSAGE(STATUS "=====================")
#____________
SET(AF_FOLDER  ${CMAKE_SOURCE_DIR}/)
include(./mcuSelect.cmake)

SET(LN_LTO "-flto" CACHE INTERNAL "")

ADD_DEFINITIONS(" -fno-builtin-memcpy ")
#ADD_DEFINITIONS("-DENABLE_GPIO_DFU_BOOT -DGPIO_DFU_BOOT_PORT=GPIOA -DGPIO_DFU_BOOT_PIN=10")

SET(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/toolchain_arm_gd32fx.cmake CACHE INTERNAL "")
SET(CMAKE_C_COMPILER_FORCED TRUE CACHE INTERNAL "")
SET(CMAKE_CXX_COMPILER_FORCED TRUE CACHE INTERNAL "")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake CACHE INTERNAL "")
SET(LN_CUSTOM_LD_SCRIPT ${CMAKE_SOURCE_DIR}/linker_script.ld)

#____________
configure_file(flash_config.h.in flash_config.h @ONLY)
PROJECT(lnDSOBootloader NONE) # the NONE is very important !
enable_language(C CXX ASM) # this is important too!
#
include_directories(${CMAKE_SOURCE_DIR})
include_directories(${CMAKE_BINARY_DIR})

ADD_DEFINITIONS("-DVERSION=\"0.0\"")
SET(LN_EXT "arm_gd32fx" CACHE INTERNAL "")

include(lnCmake)

GENERATE_GD32_FIRMWARE(lnDSO_Bootloader main.c usb.c init.c arm_gd32fx/start.cpp)
#
MESSAGE(STATUS "Configured for ${LN_MCU}")
MESSAGE(STATUS "Total Flash ${LN_MCU_FLASH_SIZE}")

