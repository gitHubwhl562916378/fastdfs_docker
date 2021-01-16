cmake_minimum_required(VERSION 3.9 FATAL_ERROR)

project(client_demo
        LANGUAGES CXX
        VERSION "1.0"
        DESCRIPTION "fastdfs client demo"
        HOMEPAGE_URL "https://github.com/happyfish100/fastdfs")

set(CMAKE_CXX_STANDARD 17)
include_directories(/usr/include/fastcommon /usr/include/fastdfs)

file(GLOB CXX_SRCS *.cpp *.hpp *.h)
add_executable(${PROJECT_NAME} ${CXX_SRCS})

target_link_libraries(${PROJECT_NAME} fastcommon fdfsclient)