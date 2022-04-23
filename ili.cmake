SET(ILI ${CMAKE_SOURCE_DIR}/stm32f1ili)
SET(OCM3 ${ILI}/libopencm3)
SET(HS ${ILI}/heatshrink)
include_directories(${ILI})
include_directories(${ILI}/heatshrink)
include_directories(${OCM3}/include)
ADD_DEFINITIONS(-DDSO138_PLATFORM -DSTM32F1)
add_library(ili STATIC  ${ILI}/ili9341_stm32_parallel8.cpp
                    ${OCM3}//lib/stm32/f1/rcc.c ${OCM3}/lib/stm32/f1/gpio.c 
                    ${OCM3}/lib/stm32/common/rcc_common_all.c ${OCM3}/lib/stm32/common/gpio_common_all.c 
                    ${HS}/heatshrink_decoder.c)
