# Create a library called "tau_stubs" which includes the source file "apex.cpp". 
# The extension is already found. Any number of sources could be listed here. 

include_directories(${CMAKE_SOURCE_DIR}/src)
link_directories (${CMAKE_BINARY_DIR}/src)

# C example

set(MM_SOURCES "C/matmult.c" "C/matmult_initialize.c" "C/matmult_initialize.h")

add_executable(matmult_C ${MM_SOURCES})
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pthread -DPTHREADS")
target_link_libraries (matmult_C taustubs stdc++ pthread m)
target_include_directories(matmult_C PRIVATE ${CMAKE_SOURCE_DIR}/src)
add_dependencies (matmult_C taustubs)
add_test ("test_matmult_C" matmult_C)

INSTALL(TARGETS matmult_C RUNTIME DESTINATION bin OPTIONAL)

# C++ example

set(CPI_SOURCES "C++/cpi.cpp" "C++/big_wheel.hpp")

add_executable(cpi_cpp ${CPI_SOURCES})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
target_link_libraries (cpi_cpp taustubs pthread m)
target_include_directories(cpi_cpp PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_compile_features(cpi_cpp PUBLIC "cxx_std_11")
add_dependencies (cpi_cpp taustubs)
add_test ("test_cpi_cpp" cpi_cpp)

# Fortran example

if (BUILD_FORTRAN_EXAMPLE)

    set(HELLO_SOURCES "Fortran/hello.F90")

    add_executable(hello_fortran ${HELLO_SOURCES})
    target_link_libraries (hello_fortran taustubs stdc++)
    target_include_directories(hello_fortran PRIVATE ${CMAKE_SOURCE_DIR}/src)
    add_dependencies (hello_fortran taustubs)
    add_test ("test_hello_fortran" cpi_cpp)

    INSTALL(TARGETS matmult_C cpi_cpp hello_fortran RUNTIME DESTINATION bin OPTIONAL)

else (BUILD_FORTRAN_EXAMPLE)
    INSTALL(TARGETS matmult_C cpi_cpp RUNTIME DESTINATION bin OPTIONAL)
endif (BUILD_FORTRAN_EXAMPLE)

