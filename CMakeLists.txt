cmake_minimum_required(VERSION 3.20)
project(main)

set(CMAKE_CXX_STANDARD 20)

find_package(SFML CONFIG REQUIRED COMPONENTS Graphics Window System)

add_executable(main
    main.cpp
    handlers/Adaptive_Search.cpp
    handlers/Hooke_Jeeves.cpp
    handlers/Nelder_Mid.cpp
    handlers/Visualizer.cpp
    handlers/Panel.cpp
)

target_link_libraries(main
    SFML::Graphics
    SFML::Window
    SFML::System
)

#       mkdir build
#       cd build#
#       cmake .. -DCMAKE_TOOLCHAIN_FILE=C:/Users/User/Desktop/AlgMeth/6/code/minimizator/vcpkg/scripts/buildsystems/vcpkg.cmake
# cmake --build .
# .\Debug\main.exe
# .\main.exe