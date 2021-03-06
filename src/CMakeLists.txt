project(core)
set(NAME "core")
set(PREFIX)

add_definitions(
  -fPIC
  -DCPU=${CPU}
  -DDPU=${DPU} -DDPU_H=<dpu/${DPU}.h>
  -DFPU=${FPU} -DFPU_H=<fpu/${FPU}.h>
  -DGPU=${GPU} -DGPU_H=<gpu/${GPU}.h>
  -DPIC
)

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set(CORE_SRC_DIR ${GLOBL_SRC_DIR}/${NAME})
set(CORE_CFG_DIR ${GLOBL_CFG_DIR})

set(inc
  global.h export.h error.h traits.h none.h dpu.h fpu.h gpu.h hash.h arg.h compare.h convert.h char.h log.h tmp.h
  memory.h parallel.h sys.h
)

set(srcs
  error.cpp log.cpp dpu/${DPU}.cpp fpu/${FPU}.cpp gpu/${GPU}.cpp arg.cpp
  memory/memory.cpp
  parallel/parallel.cpp
  sys/ios.cpp sys/asio.cpp sys/ios/sio.cpp sys/ios/fio.cpp sys/ios/bio.cpp sys/var.cpp sys/sys.cpp
  tmp.cpp
)

set(libs
  m dl rt pthread
)

add_subdirectory(dpu)
add_subdirectory(fpu)
add_subdirectory(gpu)

add_library(${NAME} STATIC ${srcs})
set_target_properties(${NAME} PROPERTIES PREFIX "${PREFIX}")
target_link_libraries(${NAME} ${libs})

if(SDK)
  file(MAKE_DIRECTORY ${CORE_SRC_DIR})
  install(
    FILES
      ${inc}
    DESTINATION
      ${CORE_SRC_DIR}
  )
endif(SDK)

install(
  TARGETS
    ${NAME}
  DESTINATION
    ${GLOBL_LIB_DIR}
)
