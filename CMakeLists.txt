cmake_minimum_required(VERSION 3.14)

project(mini_db)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

enable_testing()

add_subdirectory(googletest)

set(Sources
    buffer/buffer.cpp
    buffer/buffer_manager.cpp
    file/block_id.cpp
    file/file_manager.cpp
    file/page.cpp
    log/log_manager.cpp
    log/log_iterator.cpp
)

set(Headers
    buffer/buffer.h
    buffer/buffer_manager.h
    file/block_id.h
    file/file_manager.h
    file/page.h
    log/log_manager.h
    log/log_iterator.h
)

add_library(${PROJECT_NAME} STATIC ${Sources} ${Headers})

add_subdirectory(test)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})