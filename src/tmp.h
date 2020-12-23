#ifndef core_tmp_h
#define core_tmp_h
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
#include <global.h>

class tmp
{
  public:
          tmp() noexcept;
          tmp(const tmp&) noexcept;
          tmp(tmp&&) noexcept;

  static  auto  save() noexcept -> std::size_t;
  static  void  save(std::size_t& offset) noexcept;

  static  char* ptr_get(const char*, std::size_t = 0) noexcept;
  static  char* raw_get(std::size_t) noexcept;
  static  char* ptr_fmt(const char*, ...) noexcept;
  static  char* ptr_fmt_v(const char*, va_list) noexcept;
  static  char* raw_unget(std::size_t) noexcept;
  static  char* ptr_unget(std::size_t) noexcept;

  static  auto  get_length(std::size_t) noexcept;
  static  char* restore(std::size_t) noexcept;
  static  bool  reserve(std::size_t) noexcept;

          tmp&  operator=(const tmp&) noexcept;
          tmp&  operator=(tmp&&) noexcept;
};
#endif
