#ifndef memory_map_h
#define memory_map_h
/**
    Copyright (c) 2018, wicked systems

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
#include <global.h>
#include <memory/policy.h>
#include <memory/fragment.h>

/* map
 * memory map allocator
*/
class map: public fragment
{
  int m_mode;
  int m_flags;

  protected:
          void*  remap(void*, std::size_t, std::size_t, std::size_t, bool) noexcept;
          void*  unmap(void*, std::size_t, std::size_t) noexcept;

  virtual void*  do_allocate(std::size_t, std::size_t) noexcept override;
  virtual void   do_deallocate(void*, std::size_t, std::size_t) noexcept override;
  virtual bool   do_is_equal(const std::pmr::memory_resource&) const noexcept override;

  public:
  static  constexpr std::size_t alloc_bytes = global::system_page_size;
  static  constexpr std::size_t fixed_bytes = 0u;

  public:
          map() noexcept;
          map(int, bool = false) noexcept;
          map(const map&) noexcept;
          map(map&&) noexcept;
  virtual ~map();

  virtual void*  reallocate(void*, std::size_t, std::size_t, std::size_t, memory::fixed) noexcept override;
  virtual void*  reallocate(void*, std::size_t, std::size_t, std::size_t, memory::expand_throw) override;
  virtual void*  reallocate(void*, std::size_t, std::size_t, std::size_t, ...) noexcept override;

  virtual std::size_t get_fixed_size() const noexcept override;
  virtual bool        has_variable_size() const noexcept override;
  virtual std::size_t get_alloc_size(std::size_t) const noexcept override;

          map&   operator=(const map&) noexcept;
          map&   operator=(map&&) noexcept;
};
#endif
