cmake_minimum_required(VERSION 3.14)
project(BluetoothWidget-cpp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 COMPONENTS Widgets Bluetooth REQUIRED)

add_executable(BluetoothWidget-cpp main.cpp bluetoothwidget.cpp)
target_link_libraries(BluetoothWidget-cpp PRIVATE Qt6::Widgets Qt6::Bluetooth)

# For macOS, to create an application bundle
if (APPLE)
    set_target_properties(BluetoothWidget-cpp PROPERTIES MACOSX_BUNDLE TRUE)
endif()
