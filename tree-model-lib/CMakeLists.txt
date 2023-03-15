find_package(Qt5 REQUIRED COMPONENTS Core)

set(SOURCES
    treeitem.cpp
    treemodel.cpp
)
set(HEADERS
    treeitem.h
    treemodel.h
)

add_library(tree-model-lib STATIC ${HEADERS} ${SOURCES})
target_link_libraries(tree-model-lib PRIVATE Qt5::Core)
