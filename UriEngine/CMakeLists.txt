cmake_minimum_required(VERSION 3.10)

if(WIN32)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS") 
endif(WIN32)

project(UriEngine)

set(SRC_LIST src/main.cpp
             src/stdafx.h
             src/Utils.h
             src/IGameApp.cpp
             src/IGameApp.h
             src/MainWnd.cpp
             src/MainWnd.h
             src/Render.cpp
             src/Render.h)

add_executable(UriEngine ${SRC_LIST})

target_link_libraries(UriEngine DXGI D3D12)