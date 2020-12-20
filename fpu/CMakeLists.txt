set(NAME fpu)

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
)

set(FPU_SRC_DIR ${CORE_SRC_DIR}/${NAME})

set(inc
  soft.h
)

if(SDK)
  file(MAKE_DIRECTORY ${FPU_SRC_DIR})
  install(
    FILES
      ${inc}
    DESTINATION
      ${FPU_SRC_DIR}
  )
endif(SDK)
