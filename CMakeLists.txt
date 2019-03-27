# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.14)

project ("HotkeyMessage" LANGUAGES CXX)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed
set(CMAKE_AUTOMOC ON)
# Create code from a list of Qt designer ui files
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)
link_libraries(Qt5::Core Qt5::Gui Qt5::Widgets)

include_directories(
    "include"
    "src"
)

set (PLUGIN_SRC
	"src/configui.ui"
    "src/config.h"
    "src/config.cpp"
    "src/plugin.h"
    "src/plugin.cpp"
)
source_group("plugin" FILES ${PLUGIN_SRC})

# Turn on the ability to create folders to organize projects (.vcproj)
# It creates "CMakePredefinedTargets" folder by default and adds CMake
# defined projects like INSTALL.vcproj and ZERO_CHECK.vcproj
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

add_library (HotkeyMessage SHARED 
	${PLUGIN_SRC})
