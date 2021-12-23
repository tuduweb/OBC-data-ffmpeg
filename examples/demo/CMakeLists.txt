file(GLOB_RECURSE CPPS  ./*.cpp )

add_executable(example-demo ${CPPS})

target_link_libraries(example-demo obc_baselib)