

add_subdirectory(runtime)


find_package(FLEX  REQUIRED)
find_package(BISON REQUIRED)

add_compile_options(-Wno-register)

FLEX_TARGET(
    BackendScanner
    scanner.l
    ${CMAKE_CURRENT_BINARY_DIR}/scanner.cpp
)

BISON_TARGET(
    BackendParser
    parser.y
    ${CMAKE_CURRENT_BINARY_DIR}/parser.cpp
    COMPILE_FLAGS "--graph -Wcounterexamples"
    DEFINES_FILE ${CMAKE_CURRENT_BINARY_DIR}/parser.hpp
)

ADD_FLEX_BISON_DEPENDENCY(BackendScanner BackendParser)

add_library(bisonflex STATIC
    ${BISON_BackendParser_OUTPUTS}
    ${FLEX_BackendScanner_OUTPUTS}
    scanner.hpp
)

target_include_directories(bisonflex SYSTEM PUBLIC ${CMAKE_CURRENT_BINARY_DIR})
target_include_directories(bisonflex PUBLIC        ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(bisonflex PUBLIC runtime)

add_executable(backend
    main.cpp
    liba.hpp
)

target_link_libraries(backend PUBLIC runtime bisonflex)
