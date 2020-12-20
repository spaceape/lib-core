#ifndef memory_fragment_h
#define memory_fragment_h
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
#include "resource.h"

class fragment: public resource
{
  protected:
  virtual void*  do_allocate(std::size_t, std::size_t) noexcept override = 0;
  virtual bool   do_is_equal(const std::pmr::memory_resource&) const noexcept override = 0;

  public:
          fragment() noexcept;
          fragment(const fragment&) noexcept;
          fragment(fragment&&) noexcept;
          ~fragment();

  virtual void*  reallocate(void*, std::size_t, std::size_t, std::size_t, memory::fixed) noexcept;
  virtual void*  reallocate(void*, std::size_t, std::size_t, std::size_t, memory::expand_throw);
  virtual void*  reallocate(void*, std::size_t, std::size_t, std::size_t, ...) noexcept;

  inline  bool        has_fixed_size() const noexcept {
          return get_fixed_size();
  }

  virtual std::size_t get_fixed_size() const noexcept = 0;
  virtual bool        has_variable_size() const noexcept = 0;
  virtual std::size_t get_alloc_size(std::size_t) const noexcept = 0;

  static  fragment*   get_default() noexcept;
  static  fragment*   set_default(fragment*) noexcept;
  
          fragment&   operator=(const fragment&) noexcept;
          fragment&   operator=(fragment&&) noexcept;
};
#endif
