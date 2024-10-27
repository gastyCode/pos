cmake_minimum_required(VERSION 3.22)

project(
  SinglyLinkedList
  VERSION     0.0.1
  DESCRIPTION "Simple linked list implementation"
  LANGUAGES   C
)

add_subdirectory(libsll)
add_subdirectory(sll-example)
add_subdirectory(sll-tests)

add_subdirectory(libgraph)
add_subdirectory(graph-example)
