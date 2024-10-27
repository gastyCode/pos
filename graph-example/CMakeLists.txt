add_executable(graph-example)
target_sources(graph-example PRIVATE main.c)
find_package(Dijkstra REQUIRED)
target_link_libraries(graph-example PRIVATE graph sll dijkstra::dijkstra pthread)
