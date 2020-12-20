#ifndef core_global_h
#define core_global_h
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
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <utility>
#ifdef __EXCEPTIONS
#include <stdexcept>
#endif

#define __str_of(s) #s

using schar = char;
using uchar = unsigned char;

using nullptr_t = std::nullptr_t;

namespace global {

constexpr size_t system_page_size = 4096u;

/* enable implicit dynamic alloc */
/* cache size to use */
#ifdef CACHE_SMALL_MAX
constexpr size_t cache_small_max = CACHE_SMALL_MAX;
#else
constexpr std::size_t cache_small_max = 32u;
#endif

static_assert(cache_small_max > 8, "small cache constant must be greater than 0 and should be at least 8.");

#ifdef CACHE_LARGE_MAX
constexpr std::size_t cache_large_max = CACHE_LARGE_MAX;
#else
constexpr std::size_t cache_large_max = cache_small_max * 8u;
#endif

/* get_divided_value()
*/
constexpr std::size_t get_quotient_value(std::size_t value, std::size_t divider) noexcept {
      std::size_t ret = value / divider;
      if(value % divider) {
          ret += 1;
      }
      return ret;
}

/* get_round_value()
*/
constexpr std::size_t get_round_value(std::size_t value, std::size_t granularity) noexcept {
      std::size_t rem = value % granularity;
      if(rem) {
          value -= rem;
          value += granularity;
      }
      return value;
}

/*namespace global*/ }
#endif
