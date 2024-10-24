add_executable(sll-example)
target_sources(sll-example PRIVATE main.c)
target_link_libraries(sll-example PRIVATE sll)
