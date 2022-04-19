#-----------------------------------------------------------------------------
# Force PA13 to ground to force DFU
#-----------------------------------------------------------------------------
set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)

cmake_minimum_required(VERSION 3.13)
MESSAGE(STATUS "=====================")
MESSAGE(STATUS "|Starting BootLoader|")
MESSAGE(STATUS "=====================")
#____________
SET(AF_FOLDER  ${CMAKE_SOURCE_DIR}/)
OPTION(USE_GD32F3 "Build for USE_GD32F303" OFF)
include(./mcuSelect.cmake)

#SET(LN_LTO "-flto" CACHE INTERNAL "")

ADD_DEFINITIONS(" -fno-builtin-memcpy ")
ADD_DEFINITIONS("-DENABLE_GPIO_DFU_BOOT -DGPIO_DFU_BOOT_PORT=GPIOA -DGPIO_DFU_BOOT_PIN=13 -DENABLE_CHECKSUM") 

ADD_DEFINITIONS("-DXXH_NO_STDLIB  -DXXH_NO_XXH3 -DXXH_NO_LONG_LONG -DXXH_NO_STREAM") 

ADD_DEFINITIONS("-DLN_MCU_SPEED=${LN_MCU_SPEED}")

SET(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/toolchain_arm_gd32fx.cmake CACHE INTERNAL "")
SET(CMAKE_C_COMPILER_FORCED TRUE CACHE INTERNAL "")
SET(CMAKE_CXX_COMPILER_FORCED TRUE CACHE INTERNAL "")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake CACHE INTERNAL "")
SET(LN_CUSTOM_LD_SCRIPT ${CMAKE_SOURCE_DIR}/linker_script.ld)

#____________
SET(BOOTLOADER_SIZE 8)
math(EXPR APP_START    "0x8000000" OUTPUT_FORMAT HEXADECIMAL )  
math(EXPR PAYLOAD_SIZE "${LN_MCU_FLASH_SIZE}-${BOOTLOADER_SIZE}-${LN_MCU_EEPROM_SIZE}")  

configure_file(flash_config.h.in flash_config.h @ONLY)
PROJECT(lnDSOBootloader NONE) # the NONE is very important !
enable_language(C CXX ASM) # this is important too!
#
include_directories(${CMAKE_SOURCE_DIR})
include_directories(${CMAKE_BINARY_DIR})

ADD_DEFINITIONS("-DVERSION=\"0.0\"")
SET(LN_EXT "arm_gd32fx" CACHE INTERNAL "")

include(lnCmake)
include_directories(src)

include_directories(lnArduino_mini/include)

# LCD driver
#_____________________________________________________
add_subdirectory(simplerILI9341)
include_directories( lcdDriver/ )
include_directories(simplerILI9341)
GENERATE_GD32_FIRMWARE(lnDSO_Bootloader src/main.cpp src/usb.cpp src/init.cpp src/xxhash.c arm_gd32fx/start.cpp src/stubs.cpp src/lcd.cpp 
                lnArduino_mini/systemHelper.cpp
                lnArduino_mini/lnGPIO.cpp lnArduino_mini/lnPinMapping.cpp lnArduino_mini/systemAlloc.cpp
                lnArduino_mini/lnRCU.cpp lnArduino_mini/lnRCU_systemClock_arm_gd32fx.cpp
                lnArduino_mini/lnIRQ_arm_gd32fx.cpp
                lcdDriver/mini8bit.cpp
                )
target_link_libraries(lnDSO_Bootloader simplerILI9341)
#
#

MESSAGE(STATUS "Configured for ${LN_MCU}")
MESSAGE(STATUS "Total Flash ${LN_MCU_FLASH_SIZE}")

IF(USE_GD32F3)
    set_target_properties(lnDSO_Bootloader PROPERTIES OUTPUT_NAME lnDSO_Bootloader_USE_GD32F303)
ENDIF(USE_GD32F3)

