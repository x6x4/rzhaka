
add_library(runtime STATIC runtime.cpp runtime.hpp vars.hpp baza.hpp)

get_filename_component(PARENT_DIR ../ ABSOLUTE)
target_include_directories(runtime PUBLIC ${PARENT_DIR})
