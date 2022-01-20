file(GLOB_RECURSE CPPS  ./*.cpp)

add_executable(example-rtsp ${CPPS})

target_link_libraries(example-rtsp obc_baselib)
target_link_libraries(example-rtsp ffmpeg_lib)