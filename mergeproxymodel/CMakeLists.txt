find_package(Qt5 REQUIRED COMPONENTS Core)

set(SOURCES
    mergeproxymodel.cpp
)
set(HEADERS
    mergeproxymodel.h
)

add_library(mergeproxymodel-lib STATIC ${HEADERS} ${SOURCES})
target_link_libraries(mergeproxymodel-lib PRIVATE Qt5::Core)
