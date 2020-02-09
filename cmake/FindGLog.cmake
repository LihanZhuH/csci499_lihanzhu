include(FindPackageHandleStandardArgs)

if (NOT DEFINED GLOG_ROOT)
#     message("-- Setting GLOG_ROOT")
    set (GLOG_ROOT /usr /usr/local /usr/include/)
endif (NOT DEFINED GLOG_ROOT)

#set(GLOG_ROOT_DIR "" CACHE PATH "Folder contains Google glog")

find_path(GLOG_INCLUDE_DIR glog/logging.h
        PATHS
        ${GLOG_ROOT_DIR}
        PATH_SUFFIXES
        src)

find_library(GLOG_LIBRARY glog libglog
        PATHS
        ${GLOG_ROOT_DIR}
        PATH_SUFFIXES
        .libs
        lib
        lib64)

find_package_handle_standard_args(GLOG DEFAULT_MSG
        GLOG_INCLUDE_DIR GLOG_LIBRARY)

if(GLOG_FOUND)
    set(GLOG_INCLUDE_DIRS ${GLOG_INCLUDE_DIR})
    set(GLOG_LIBRARIES ${GLOG_LIBRARY})
    message(STATUS "GLOG_INCLUDE_DIRS: ${GLOG_INCLUDE_DIRS}")
    message(STATUS "GLOG_LIBRARY: ${GLOG_LIBRARY}")
    message(STATUS "Using glog")
endif()