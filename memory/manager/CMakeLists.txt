set(NAME manager)

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
)

set(MANAGER_SRC_DIR ${MEMORY_SRC_DIR}/${NAME})

set(inc
  heap.h map.h shared.h
)

if(SDK)
  file(MAKE_DIRECTORY ${MANAGER_SRC_DIR})
  install(
    FILES
      ${inc}
    DESTINATION
      ${MANAGER_SRC_DIR}
  )
endif(SDK)
