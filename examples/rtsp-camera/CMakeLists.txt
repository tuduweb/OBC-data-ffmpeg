file(GLOB_RECURSE CPPS  ./*.cpp)
file(GLOB_RECURSE HPPS  ./*.hpp)

add_executable(example-rtsp-camera ${CPPS} ${HPPS})

target_link_libraries(example-rtsp-camera obc_baselib)
target_link_libraries(example-rtsp-camera ffmpeg_lib)