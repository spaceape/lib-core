#ifndef memory_flat_list_traits_h
#define memory_flat_list_traits_h
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
#include <memory.h>
#include <traits.h>

namespace memory {
namespace flat_list_traits {

  struct has_prev_impl {
    template<typename Xt>
    static constexpr auto test(int) -> decltype(std::declval<Xt>().get_prev());

    template<typename Xt>
    static constexpr auto test(...) -> std::false_type;
  };

  template<typename Xt>
  struct has_prev: std::conditional<
          std::is_same<Xt*, decltype(has_prev_impl::test<Xt>(0))>::value,
          std::true_type, 
          std::false_type
      >::type {
  };

  struct has_next_impl {
    template<typename Xt>
    static constexpr auto test(int) -> decltype(std::declval<Xt>().get_next());

    template<typename Xt>
    static constexpr auto test(...) -> std::false_type;
  };

  template<typename Xt>
  struct has_next: std::conditional<
          std::is_same<Xt*, decltype(has_next_impl::test<Xt>(0))>::value,
          std::true_type, 
          std::false_type
      >::type {
  };

/*namespace flat_list_traits*/ }
/*namespace memory*/ }
#endif
