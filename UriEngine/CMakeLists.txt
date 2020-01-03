cmake_minimum_required(VERSION 3.10)

# if(WIN32)
#     set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS") 
# endif(WIN32)

project(UriEngine)

file(GLOB SRC_LIST src/*.cpp src/*.h)
# set(SRC_LIST src/*.cpp src/*.h)

add_executable(UriEngine WIN32 ${SRC_LIST})

target_link_libraries(UriEngine DXGI D3D12)