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
#include "tmp.h"
#include "memory.h"
#include "memory/pool.h"

static memory::pool<char, map> s_tmp;

      tmp::tmp() noexcept
{
}

      tmp::tmp(const tmp&) noexcept
{
}

      tmp::tmp(tmp&&) noexcept
{
}

auto  tmp::save() noexcept -> std::size_t
{
      return s_tmp.save();
}

void  tmp::save(std::size_t& offset) noexcept
{
      s_tmp.save(offset);
}

char* tmp::ptr_get(const char* src, std::size_t length) noexcept
{
      return s_tmp.ptr_get(src, length);
}

char* tmp::raw_get(std::size_t count) noexcept
{
      return s_tmp.raw_get(count);
}

char* tmp::ptr_fmt(const char* fmt, ...) noexcept
{
      char*   l_result;
      va_list l_va;
      va_start(l_va, fmt);
      l_result = ptr_fmt_v(fmt, l_va);
      va_end(l_va);
      return  l_result;
}

char* tmp::ptr_fmt_v(const char* fmt, va_list va) noexcept
{
      return s_tmp.ptr_fmt_v(fmt, va);
}

char* tmp::raw_unget(std::size_t count) noexcept
{
      return s_tmp.raw_unget(count);
}

char* tmp::ptr_unget(std::size_t count) noexcept
{
      return s_tmp.ptr_unget(count);
}

auto  tmp::get_length(std::size_t offset) noexcept
{
      return s_tmp.get_length(offset);
}

char* tmp::restore(std::size_t offset) noexcept
{
      return s_tmp.restore(offset);
}

bool  tmp::reserve(std::size_t count) noexcept
{
      return s_tmp.reserve(count);
}

tmp&  tmp::operator=(const tmp&) noexcept
{
      return *this;
}

tmp&  tmp::operator=(tmp&&) noexcept
{
      return *this;
}
