cmake_minimum_required(VERSION 3.5)

project(Gui)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# set to ON or OFF to toggle the use of ENVVAR_CONFIG preprocessor definition
set(USE_ENVVAR_CONFIG ON)

# creates "compile_commands.json" in build directory which makes 
# clangd (cpp language server) index symbols from current source
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# load Qt cmake module
find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)

# load Freetype cmake module 
find_package(Freetype REQUIRED)

set(PROJECT_SOURCES
    main.cpp

    gui.cpp
    gui.hpp

    mav.cpp

    can/can.cpp
    can/can.hpp
    can/utils.cpp
    can/utils.h

    serialport/serialport.hpp
    serialport/serialport.cpp

    gps/gps.cpp
    gps/gps.hpp
    gps/gps_init.h
    gps/parser.cpp
    gps/parser.hpp

    gps/msgs/NMEA.hpp

    #TODO: check what is rly used
    epaper/e-Paper/EPD_IT8951.c
    epaper/e-Paper/EPD_IT8951.h
    epaper/GUI/GUI_BMPfile.c
    epaper/GUI/GUI_BMPfile.h
    epaper/GUI/GUI_Paint.c
    epaper/GUI/GUI_Paint.h
    epaper/Fonts/font8.c
    epaper/Fonts/font12.c
    epaper/Fonts/font12CN.c
    epaper/Fonts/font16.c
    epaper/Fonts/font20.c
    epaper/Fonts/font24.c
    epaper/Fonts/font24CN.c
    epaper/Fonts/font32.c
    epaper/Fonts/fonts.h
    epaper/Config/Debug.h
    epaper/Config/DEV_Config.c
    epaper/Config/DEV_Config.h
    epaper/bcm2835/bcm2835.c
    epaper/bcm2835/bcm2835.h

    fontdraw/fontdraw.c
    fontdraw/fontdraw.h
    fontdraw/sbt_fontdraw.c
    fontdraw/sbt_fontdraw.h

    draw_gui/draw_gui.cpp
    draw_gui/draw_gui.hpp

    UI_config/utf_text.h
    UI_config/font.h
)

if(USE_ENVVAR_CONFIG)
    add_compile_options("-DENVVAR_CONFIG")
endif()

add_executable(Gui
    ${PROJECT_SOURCES}
)


target_link_libraries(Gui PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    pthread
    ${FREETYPE_LIBRARIES}
)

target_include_directories(Gui PRIVATE 
    "epaper/bcm2835"
    "epaper/e-Paper"
    "epaper/Fonts"
    "epaper/GUI"
    "epaper/Config"
    ${FREETYPE_INCLUDE_DIRS}
    "fontdraw"
    "UI_config"
    "can" 
    "gps" 
    "gps/msgs"
)
