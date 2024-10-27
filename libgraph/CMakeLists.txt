add_library(graph SHARED)
target_sources(graph PRIVATE graph.c)
target_compile_options(graph PUBLIC "-std=c11")
target_compile_options(graph PRIVATE "-Wall" "-Wextra" "-Wpedantic")
