include(FindPackageHandleStandardArgs)

if (NOT DEFINED GFLAGS_ROOT)
#     message("-- Setting GFLAGS_ROOT")
    set (GFLAGS_ROOT /usr /usr/local /usr/include/)
endif (NOT DEFINED GFLAGS_ROOT)

#set(GFLAGS_ROOT_DIR "" CACHE PATH "Folder contains Google glog")

find_path(GFLAGS_INCLUDE_DIR gflags/gflags.h
        PATHS
        ${GFLAGS_ROOT_DIR}
        PATH_SUFFIXES
        src)

find_library(GFLAGS_LIBRARY gflags libgflags
        PATHS
        ${GFLAGS_ROOT_DIR}
        PATH_SUFFIXES
        .libs
        lib
        lib64)

find_package_handle_standard_args(GFLAGS DEFAULT_MSG
        GFLAGS_INCLUDE_DIR GFLAGS_LIBRARY)

if(GFLAGS_FOUND)
    set(GFLAGS_INCLUDE_DIRS ${GFLAGS_INCLUDE_DIR})
    set(GFLAGS_LIBRARIES ${GFLAGS_LIBRARY})
    message(STATUS "GFLAGS_INCLUDE_DIRS: ${GFLAGS_INCLUDE_DIRS}")
    message(STATUS "GFLAGS_LIBRARY: ${GFLAGS_LIBRARY}")
    message(STATUS "Using gflag")
endif()