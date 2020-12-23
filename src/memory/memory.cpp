/**
    Copyright (c) 2020, wicked systems

    Redistribution and use in source and binary forms, with or without modification, are
    permitted provided that the following conditions are met:
    1. Redistributions of source code must retain the above copyright notice, this list of
    conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice, this list
    of conditions and the following disclaimer in the documentation and/or other materials
    provided with the distribution.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
    EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
    MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL
    THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
    SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
    OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
    HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
    TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**/
#include "memory.h"
#include "metrics.h"
#include "error.h"
#include <sys/mman.h>
#include <limits>

static inline bool is_aligned(std::size_t value, std::size_t align) noexcept
{
      return (value % align) == 0u;
}

static inline bool is_aligned(void* p, std::size_t align) noexcept
{
      return is_aligned(reinterpret_cast<std::size_t>(p), align);
}

static inline std::size_t get_aligned_value(std::size_t value, std::size_t align) noexcept
{
      size_t remainder = value % align;
      if(remainder) {
          value -= remainder;
          value += align;
      }
      return value;
}

namespace memory {
/*namespace memory*/ }

/* fragment
*/

static heap       s_heap;
static fragment*  s_default_fragment = fragment::set_default(nullptr);

      fragment::fragment() noexcept:
      resource()
{
}

      fragment::fragment(const fragment& copy) noexcept:
      resource(copy)
      
{
}

      fragment::fragment(fragment&& copy) noexcept:
      resource(std::move(copy))
{
}

      fragment::~fragment()
{
}

void* fragment::reallocate(void*, std::size_t, std::size_t, std::size_t, memory::fixed) noexcept
{
      return nullptr;
}

void* fragment::reallocate(void*, std::size_t, std::size_t, std::size_t, memory::expand_throw)
{
      #ifdef __EXCEPTIONS
      throw  std::length_error("memory boundaries exceeded");
      #else
      return nullptr;
      #endif
}

void* fragment::reallocate(void*, std::size_t, std::size_t, std::size_t, ...) noexcept
{
      return nullptr;
}

fragment* fragment::get_default() noexcept
{
      return s_default_fragment;
}

fragment* fragment::set_default(fragment* fragment) noexcept
{
      if(fragment) {
          s_default_fragment = fragment;
      } else
          s_default_fragment = std::addressof(s_heap);
      return s_default_fragment;
}

fragment& fragment::operator=(const fragment&) noexcept
{
      return *this;
}

fragment& fragment::operator=(fragment&&) noexcept
{
      return *this;
}

/* heap memory manager
*/
      heap::heap() noexcept:
      fragment()
{
}

      heap::heap(const heap& copy) noexcept:
      fragment(copy)
{
}

      heap::heap(heap&& copy) noexcept:
      fragment(std::move(copy))
{
}

      heap::~heap()
{
}

void* heap::do_allocate(std::size_t size, std::size_t align) noexcept
{
      return aligned_alloc(align, size);
}

void  heap::do_deallocate(void* p, std::size_t, std::size_t) noexcept
{
      free(p);
}

bool  heap::do_is_equal(const std::pmr::memory_resource&) const noexcept
{
      return true;
}

void* heap::reallocate(void* p, std::size_t, std::size_t new_size, std::size_t align, memory::fixed) noexcept
{
      if(p) {
          return nullptr;
      } else
          return aligned_alloc(align, new_size);
}

void* heap::reallocate(void* p, std::size_t size, std::size_t, std::size_t align, memory::expand_throw)
{
      if(p) {
      #ifdef __EXCEPTIONS
          throw  std::length_error("memory boundaries exceeded");
      #else
          return nullptr;
      #endif
      } else
          return aligned_alloc(align, size);
}

void* heap::reallocate(void* p, std::size_t, std::size_t new_size, std::size_t align, ...) noexcept
{
      if(p) {
          if(is_aligned(p, align)) {
              return realloc(p, new_size);
          } else
              return nullptr;
      } else
          return aligned_alloc(align, new_size);
}

std::size_t heap::get_fixed_size() const noexcept
{
      return 0;
}

bool  heap::has_variable_size() const noexcept
{
      return true;
}

std::size_t heap::get_alloc_size(std::size_t size) const noexcept
{
      return global::get_round_value(size, alloc_bytes);
}

heap& heap::operator=(const heap& rhs) noexcept
{
      fragment::operator=(rhs);
      return *this;
}

heap& heap::operator=(heap&& rhs) noexcept
{
      fragment::operator=(std::move(rhs));
      return *this;
}

/* map memory manager
*/
      map::map() noexcept:
      fragment(),
      m_mode(PROT_READ | PROT_WRITE),
      m_flags(MAP_PRIVATE | MAP_ANONYMOUS)
{
}

      map::map(int flags, bool exec) noexcept:
      fragment(),
      m_mode(PROT_READ | PROT_WRITE),
      m_flags(flags)
{
      if(exec) {
          m_mode |= PROT_EXEC;
      }
}


      map::map(const map& copy) noexcept:
      fragment(copy),
      m_mode(copy.m_mode),
      m_flags(copy.m_flags)
{
}

      map::map(map&& copy) noexcept:
      fragment(std::move(copy)),
      m_mode(copy.m_mode),
      m_flags(copy.m_flags)
{
}

      map::~map()
{
}

void* map::do_allocate(std::size_t size, std::size_t align) noexcept
{
      if(size) {
          if(is_aligned(alloc_bytes, align)) {
              std::size_t l_size = get_aligned_value(size, alloc_bytes);
              void*       l_data = mmap(nullptr, l_size, m_mode, m_flags, -1, 0);
              if(l_data != MAP_FAILED) {
                  return l_data;
              } else
                  return nullptr;
          } else
              return nullptr;
      } else
          return nullptr;
}

void  map::do_deallocate(void* p, std::size_t size, std::size_t) noexcept
{
      munmap(p, size);
}

bool  map::do_is_equal(const std::pmr::memory_resource&) const noexcept
{
      return true;
}

void* map::reallocate(void* p, std::size_t size, std::size_t new_size, std::size_t align, memory::fixed) noexcept
{
      std::size_t l_size = get_aligned_value(size, alloc_bytes);
      if(p) {
          if(l_size < new_size) {
              std::size_t l_size_new = get_aligned_value(new_size, alloc_bytes);
              void*       l_data     = mremap(p, l_size, l_size_new, MREMAP_FIXED);
              if(l_data != MAP_FAILED) {
                  return l_data;
              } else
                  return nullptr;
          } else
              return p;
      } else
          return allocate(new_size, align);
}

void* map::reallocate(void* p, std::size_t size, std::size_t new_size, std::size_t align, memory::expand_throw)
{
      std::size_t l_size = get_aligned_value(size, alloc_bytes);
      if(p) {
          if(l_size < new_size) {
          #ifdef __EXCEPTIONS
              throw  std::length_error("memory boundaries exceeded");
          #else
              return nullptr;
          #endif
          } else
              return p;
      } else
          return allocate(new_size, align);
}

void* map::reallocate(void* p, std::size_t size, std::size_t new_size, std::size_t align, ...) noexcept
{
      std::size_t l_size = get_aligned_value(size, alloc_bytes);
      if(p) {
          if(l_size < new_size) {
              std::size_t l_size_new = get_aligned_value(new_size, alloc_bytes);
              void*       l_data     = mremap(p, l_size, l_size_new, MREMAP_MAYMOVE);
              if(l_data != MAP_FAILED) {
                  return l_data;
              } else
                  return nullptr;
          } else
              return p;
      } else
          return allocate(new_size, align);
}

std::size_t map::get_fixed_size() const noexcept
{
      return 0;
}

bool  map::has_variable_size() const noexcept
{
      return true;
}

std::size_t map::get_alloc_size(std::size_t size) const noexcept
{
      return global::get_round_value(size, alloc_bytes);
}

map&  map::operator=(const map& rhs) noexcept
{
      fragment::operator=(rhs);
      m_mode = rhs.m_mode;
      m_flags = rhs.m_flags;
      return *this;
}

map&  map::operator=(map&& rhs) noexcept
{
      fragment::operator=(std::move(rhs));
      m_mode = rhs.m_mode;
      m_flags = rhs.m_flags;
      return *this;
}
