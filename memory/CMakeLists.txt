set(NAME memory)

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
)

set(MEMORY_SRC_DIR ${CORE_SRC_DIR}/${NAME})

set(inc
  metrics.h policy.h
  flat_list_traits.h flat_list.h
  flat_map_traits.h flat_map.h hash_map.h
  linked_list_traits.h linked_list_base.h linked_list.h ordered_list.h
  pool_base.h pool.h page.h single_page_pool.h multi_page_pool.h
  page.h
)

add_subdirectory(manager)

if(SDK)
  file(MAKE_DIRECTORY ${MEMORY_SRC_DIR})
  install(
    FILES
      ${inc}
    DESTINATION
      ${MEMORY_SRC_DIR}
  )
endif(SDK)
