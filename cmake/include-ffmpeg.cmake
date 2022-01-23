# 已经编译好的方式

IF (APPLE)

set(ffmpeg_libs_DIR /usr/local/Cellar/ffmpeg/4.4.1_3/lib)
set(ffmpeg_headers_DIR /usr/local/Cellar/ffmpeg/4.4.1_3/include)

add_library(avutil SHARED IMPORTED)
add_library(avcodec SHARED IMPORTED)
add_library(avformat SHARED IMPORTED)
add_library(swresample SHARED IMPORTED)
add_library(swscale SHARED IMPORTED)
add_library(avfilter SHARED IMPORTED)

#指定所添加依赖库的导入路径
set_target_properties( avcodec PROPERTIES IMPORTED_LOCATION ${ffmpeg_libs_DIR}/libavcodec.dylib )
set_target_properties( avfilter PROPERTIES IMPORTED_LOCATION ${ffmpeg_libs_DIR}/libavfilter.dylib )
set_target_properties( swresample PROPERTIES IMPORTED_LOCATION ${ffmpeg_libs_DIR}/libswresample.dylib )
set_target_properties( swscale PROPERTIES IMPORTED_LOCATION ${ffmpeg_libs_DIR}/libswscale.dylib )
set_target_properties( avformat PROPERTIES IMPORTED_LOCATION ${ffmpeg_libs_DIR}/libavformat.dylib )
set_target_properties( avutil PROPERTIES IMPORTED_LOCATION ${ffmpeg_libs_DIR}/libavutil.dylib )

# 添加头文件路径到编译器的头文件搜索路径下，多个路径以空格分隔
include_directories( ${ffmpeg_headers_DIR} )
link_directories(${ffmpeg_libs_DIR} )

add_library(ffmpeg_lib STATIC
    ${CMAKE_SOURCE_DIR}/src/RTSPThread.cpp
    ${CMAKE_SOURCE_DIR}/src/RTSPThread.hpp
)

target_link_libraries(ffmpeg_lib
    PUBLIC avcodec
    avformat avutil swresample swscale swscale avfilter
)

ELSEIF(WIN32)

include_directories(${CMAKE_SOURCE_DIR}/3rd/ffmpeg/include/)
link_directories(${CMAKE_SOURCE_DIR}/3rd/ffmpeg/lib/)

file(GLOB_RECURSE HEADERS  ${CMAKE_SOURCE_DIR}/3rd/ffmpeg/include/*.h)

add_library(ffmpeg_lib STATIC ${HEADERS}
    ${CMAKE_SOURCE_DIR}/src/RTSPThread.cpp
    ${CMAKE_SOURCE_DIR}/src/RTSPThread.hpp
)
target_link_libraries(ffmpeg_lib
    PUBLIC avcodec
    avformat avutil swresample swscale swscale avfilter
)

ENDIF()

# RTSPThread need qt lib
target_link_libraries(ffmpeg_lib
    PUBLIC Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Network
)