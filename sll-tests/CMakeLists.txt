find_package(SLLTests REQUIRED)
add_executable(sll-run-tests)
target_sources(sll-run-tests PRIVATE main.c)
target_link_libraries(sll-run-tests PRIVATE sll slltests::slltests)
