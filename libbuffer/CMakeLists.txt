add_library(buffer SHARED)
target_sources(buffer PRIVATE buffer.c synchronized_buffer.c)
target_compile_options(buffer PUBLIC "-std=c11")
target_compile_options(buffer PRIVATE "-Wall" "-Wextra" "-Wpedantic")
