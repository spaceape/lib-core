set(NAME dpu)

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
)

set(DPU_SRC_DIR ${CORE_SRC_DIR}/${NAME})

set(inc
  soft.h
)

if(SDK)
  file(MAKE_DIRECTORY ${DPU_SRC_DIR})
  install(
    FILES
      ${inc}
    DESTINATION
      ${DPU_SRC_DIR}
  )
endif(SDK)
