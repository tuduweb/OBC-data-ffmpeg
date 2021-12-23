file(GLOB_RECURSE CPPS  ./*.cpp )

add_executable(example-hello ${CPPS})

target_link_libraries(example-hello obc_baselib)
target_link_libraries(example-hello ffmpeg_lib)