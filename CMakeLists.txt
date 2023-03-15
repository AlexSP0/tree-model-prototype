cmake_minimum_required(VERSION 3.14)

project(tree-model-prototype LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(tree-model-lib)
add_subdirectory(mergeproxymodel)

find_package(Qt5 REQUIRED COMPONENTS Core)

add_executable(tree-model-prototype
  main.cpp
)
target_link_libraries(tree-model-prototype PRIVATE Qt5::Core)
target_link_libraries(tree-model-prototype PRIVATE tree-model-lib)
install(TARGETS tree-model-prototype
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
