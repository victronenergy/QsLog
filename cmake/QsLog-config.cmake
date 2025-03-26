
# QsLogConfig.cmake
# 这个文件是为QsLog库设置的配置文件
get_filename_component(QsLog_CONFIG_DIR "${CMAKE_CURRENT_LIST_FILE}" PATH)
# 设置库头文件的路径
if(NOT QsLog_INCLUDE_DIRS)
    set(QsLog_INCLUDE_DIRS "${QsLog_CONFIG_DIR}/../include")
else()
    message(FATAL_ERROR "The QsLog_INCLUDE_DIRS variable is set multiple times, please check.")
endif()

# 设置库的路径
if(NOT QsLog_LIBRARIES)
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(QsLog_LIBRARIES "${QsLog_CONFIG_DIR}/../lib/QsLog_d.lib")
    else()
        set(QsLog_LIBRARIES "${QsLog_CONFIG_DIR}/../lib/QsLog.lib")
    endif()
else()
    message(FATAL_ERROR "The QsLog_LIBRARIES variable is set multiple times, please check.")
endif()

# 或者如果你的库是动态链接库，可以这样设置
# set(QsLog_LIBRARIES "C:/path/to/QsLog/lib/QsLog.dll")

# 外部项目可以通过以下方式找到并链接QsLog库
include_directories(${QsLog_INCLUDE_DIRS})

# 添加库路径
link_directories(${QsLog_CONFIG_DIR}/../lib)

# 设置库的版本
set(QsLog_VERSION 2.0)

# 可选：设置库的依赖
# set(QsLog_DEPENDENCIES some_other_library)

# 这个函数用于查找并设置QsLog库
find_package_handle_standard_args(QsLog
                                  REQUIRED_VARS QsLog_INCLUDE_DIRS QsLog_LIBRARIES
                                  VERSION_VAR QsLog_VERSION)

# 提供库的路径变量给外部项目
if(QsLog_FOUND)
    get_filename_component(QsLog_DIR "${QsLog_CONFIG_DIR}/../" PATH)
    set(QsLog_FOUND True)
endif()
