set(NAME parallel)

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
)

set(PARALLEL_SRC_DIR ${CORE_SRC_DIR}/${NAME})

set(inc
  thread-base.h atomic-thread-base.h posix-thread-base.h
  thread.h queue.h
)

if(SDK)
  file(MAKE_DIRECTORY ${PARALLEL_SRC_DIR})
  install(
    FILES
      ${inc}
    DESTINATION
      ${PARALLEL_SRC_DIR}
  )
endif(SDK)
