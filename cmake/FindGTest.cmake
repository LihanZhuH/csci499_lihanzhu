if (NOT DEFINED GTEST_ROOT)
#     message("-- Setting GTEST_ROOT========================")
    set (GTEST_ROOT /usr /usr/local /usr/include/)
endif (NOT DEFINED GTEST_ROOT)

#set(GTEST_ROOT_DIR "" CACHE PATH "Folder contains Google test")

find_path(GTEST_INCLUDE_DIR gtest/gtest.h
        PATHS
        ${GTEST_ROOT_DIR}
        PATH_SUFFIXES
        src)

find_library(GTEST_LIBRARY gtest libgtest
        PATHS
        ${GTEST_ROOT_DIR}
        PATH_SUFFIXES
        .libs
        lib
        lib64)

find_package_handle_standard_args(GTEST DEFAULT_MSG
        GTEST_INCLUDE_DIR GTEST_LIBRARY)

if(GTEST_FOUND)
    set(GTEST_INCLUDE_DIRS ${GTEST_INCLUDE_DIR})
    set(GTEST_LIBRARIES ${GTEST_LIBRARY})
    message(STATUS "GTEST_INCLUDE_DIRS ${GTEST_INCLUDE_DIRS}")
    message(STATUS "GTEST_LIBRARY ${GTEST_LIBRARY}")
    message(STATUS "Using gtest")
endif()