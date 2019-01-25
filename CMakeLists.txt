cmake_minimum_required (VERSION 3.9 FATAL_ERROR)

if (NOT DEFINED BUILD_FORTRAN_EXAMPLE)
    SET(BUILD_FORTRAN_EXAMPLE OFF)
endif (NOT DEFINED BUILD_FORTRAN_EXAMPLE)

# set the project name
if (BUILD_FORTRAN_EXAMPLE)
    project (taustubs LANGUAGES CXX C Fortran VERSION 0.1 DESCRIPTION "TAU stub library")
else (BUILD_FORTRAN_EXAMPLE)
    project (taustubs LANGUAGES CXX C VERSION 0.1 DESCRIPTION "TAU stub library")
endif (BUILD_FORTRAN_EXAMPLE)

# if the user didn't specify, set the installation directory.
if(NOT DEFINED CMAKE_INSTALL_PREFIX OR CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    SET(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}" CACHE STRING "APEX Installation directory" FORCE)
endif()

if (NOT DEFINED BUILD_SHARED_LIBS)
    set(BUILD_SHARED_LIBS OFF)
endif (NOT DEFINED BUILD_SHARED_LIBS)

if (NOT DEFINED BUILD_STATIC_EXECUTABLES)
    set(STATIC_EXECUTABLES OFF)
endif (NOT DEFINED BUILD_STATIC_EXECUTABLES)

################################################################################
# Set the RPATH to make sure executables always work without "make install"
################################################################################
if (NOT BUILD_STATIC_EXECUTABLES)
    MESSAGE("Fixing RPATH")
    # use, i.e. don't skip the full RPATH for the build tree
    SET(CMAKE_SKIP_BUILD_RPATH  FALSE)

    # when building, don't use the install RPATH already
    # (but later on when installing)
    SET(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)

    SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")

    # add the automatically determined parts of the RPATH
    # which point to directories outside the build tree to the install RPATH
    SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

    # the RPATH to be used when installing, but only if it's not a system directory
    LIST(FIND CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES "${CMAKE_INSTALL_PREFIX}/lib" isSystemDir)
    IF("${isSystemDir}" STREQUAL "-1")
    SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
    ENDIF("${isSystemDir}" STREQUAL "-1")
    # use PIC for shared objects
    set(CMAKE_POSITION_INDEPENDENT_CODE TRUE)
    set(POSITION_INDEPENDENT_CODE TRUE)
    set(BUILD_SHARED_LIBS TRUE)
else()
    set(CMAKE_FIND_LIBRARY_SUFFIXES ${CMAKE_STATIC_LIBRARY_SUFFIX})
    set(CMAKE_POSITION_INDEPENDENT_CODE FALSE)
    set(POSITION_INDEPENDENT_CODE FALSE)
    set(BUILD_SHARED_LIBS FALSE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DAPEX_STATIC")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DAPEX_STATIC")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static")
    set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS)
    set(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS)
    set(USE_JEMALLOC FALSE)
    unset(JEMALLOC_ROOT)
    set(USE_TCMALLOC FALSE)
    unset(GPERFTOOLS_ROOT)
endif()

# Enable all warnings as errors, and be pedantic.

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    execute_process(COMMAND ${CMAKE_C_COMPILER} -dumpversion
                    OUTPUT_VARIABLE GCC_VERSION)
    if(GCC_VERSION VERSION_GREATER 4.8 OR GCC_VERSION VERSION_EQUAL 4.8)
        set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wpedantic")
        set(CMAKE_C_FLAGS "-Wall -Wextra -Wpedantic")
    else(GCC_VERSION VERSION_GREATER 4.8 OR GCC_VERSION VERSION_EQUAL 4.8)
        set(CMAKE_CXX_FLAGS "-Wall -Wextra -pedantic")
        set(CMAKE_C_FLAGS "-Wall -Wextra -pedantic")
    endif(GCC_VERSION VERSION_GREATER 4.8 OR GCC_VERSION VERSION_EQUAL 4.8)
endif(CMAKE_CXX_COMPILER_ID MATCHES "GNU")

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(CMAKE_CXX_FLAGS "-Wall -Wextra -pedantic")
    set(CMAKE_C_FLAGS "-Wall -Wextra -pedantic")
endif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")

if (CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")
    set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -Werror")
endif (CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")

add_subdirectory (src)
enable_testing()
add_subdirectory (examples)

