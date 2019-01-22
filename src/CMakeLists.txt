# Copyright (c) 2019 University of Oregon
# Distributed under the Boost Software License, Version 1.0. (See accompanying
# file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

# Create a library called "tau_stubs" which includes the source file "apex.cpp". 
# The extension is already found. Any number of sources could be listed here. 

include_directories(${CMAKE_SOURCE_DIR}/src)

INSTALL(FILES taustubsf.h tautimer.hpp DESTINATION include)

set( SOURCES 
    tautimer.hpp
    tautimer.cpp
)

add_library(taustubs ${SOURCES})
target_compile_features(taustubs PUBLIC "cxx_std_11")
target_link_libraries (taustubs dl)
set_target_properties(taustubs PROPERTIES VERSION ${PROJECT_VERSION})

install(
    TARGETS taustubs 
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)