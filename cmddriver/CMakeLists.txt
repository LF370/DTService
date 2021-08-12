
cmake_minimum_required(VERSION 3.9)
project(cmddriver)
 
 
#设定编译参数
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -Wall -std=c++17 -pthread -g -march=native")
set(CMAKE_BUILD_TYPE "Debug")
 
#设定源码列表.cpp
#设定所有源码列表 ：aux_source_directory(<dir> <variable>)
#比如:aux_source_directory(${CMAKE_SOURCE_DIR} DIR)  将${CMAKE_SOURCE_DIR}目录下，也就是最顶级目录下所有的.cpp文件放入DIR变量中，后面的add_executable就可以很简化
#    add_executable(hello_world ${DIR})
aux_source_directory(${CMAKE_SOURCE_DIR}/src SRC_LIST)
message( STATUS  "->" ${SRC_LIST} )
 
#设定头文件路径
include_directories(/usr/local/include/)

#设定链接库的路径（一般使用第三方非系统目录下的库）
#link_directories(/usr/local/lib)
link_directories(/usr/lib/x86_64-linux-gnu)
 

   
#添加子目录,作用相当于进入子目录里面，展开子目录的CMakeLists.txt
#同时执行，子目录中的CMakeLists.txt一般是编译成一个库，作为一个模块
#在父目录中可以直接引用子目录生成的库
#add_subdirectory(math) 
 
#生成动/静态库
#add_library(动/静态链接库名称  SHARED/STATIC(可选，默认STATIC)  源码列表)
#可以单独生成多个模块

#set_target_properties(${PROJECT_NAME} PROPERTIES 
#COMPILE_FEATURES "-pthread" LINK_FLAGS "-pthread")
 
#生成可执行文件
#add_executable( IOTService   main.cpp msgengine.cpp msgpump.cpp msgpumpconf.cpp )
add_executable( ${PROJECT_NAME}  ${SRC_LIST} )

target_link_libraries( ${PROJECT_NAME} libpaho-mqttpp3.so libpaho-mqtt3cs.so libev.so libtinyxml.so )

# 异步循环libev
# libtinyxml 解析依赖库
#target_link_libraries( ${PROJECT_NAME} libev.so libtinyxml.so )

#Redis plus plus 依赖
find_path(HIREDIS_HEADER hiredis REQUIRED)
target_include_directories(${PROJECT_NAME} PRIVATE $<BUILD_INTERFACE:${HIREDIS_HEADER}>)

find_library(HIREDIS_LIB hiredis)
target_link_libraries(${PROJECT_NAME} ${HIREDIS_LIB})
message(STATUS "->"  ${HIREDIS_LIB} )
# <------------ add redis-plus-plus dependency -------------->
# NOTE: this should be *sw* NOT *redis++*
find_path(REDIS_PLUS_PLUS_HEADER sw)
target_include_directories(${PROJECT_NAME} PUBLIC ${REDIS_PLUS_PLUS_HEADER})

find_library(REDIS_PLUS_PLUS_LIB redis++)
message(STATUS "->"  ${REDIS_PLUS_PLUS_LIB} )
target_link_libraries(${PROJECT_NAME} ${REDIS_PLUS_PLUS_LIB})



#target_link_libraries(${PROJECT_NAME} PUBLIC pthread)