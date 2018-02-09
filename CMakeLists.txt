


# CMake 最低版本号要求
cmake_minimum_required (VERSION 2.8)
# 项目信息
project (statistic)
# 编译选项
set(CMAKE_CXX_FLAGS "-std=c++11 -O3 -pthread -g")
set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")
# opencv相关
find_package(OpenCV REQUIRED)
include_directories( ${OpenCV_INCLUDE_DIRS} )
# 指定生成目标
add_executable(statistic buffer.cpp bufferThread.cpp cthread.cpp DrawCubic.cpp main.cpp people.cpp position.cpp tools.cpp writefileThread.cpp sendpost.cpp HttpCurl.cpp)
# 链接curl
target_link_libraries(statistic -lcurl -lcrypto)
# 链接opencv
target_link_libraries( statistic ${OpenCV_LIBS} )