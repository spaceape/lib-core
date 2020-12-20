set(NAME gpu)

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
)

set(GPU_SRC_DIR ${CORE_SRC_DIR}/${NAME})

set(inc
  gl.h
)

if(SDK)
  file(MAKE_DIRECTORY ${GPU_SRC_DIR})
  install(
    FILES
      ${inc}
    DESTINATION
      ${GPU_SRC_DIR}
  )
endif(SDK)
