#ifndef parallel_thread_base_h
#define parallel_thread_base_h
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
#include <traits.h>
#include <error.h>

namespace parallel {

struct thread_base
{
  protected:
  template<typename Xt>
  class has_launch_callback
  {
    template<typename Ot>
    static  auto test(int) -> decltype(std::declval<Ot>().thread_launch_event(), std::true_type());

    template<typename Ot>
    static  auto test(...) -> std::false_type;

    public:
    static  constexpr bool value = std::is_same<decltype(test<Xt>(0)), std::true_type>::value;
  };

  template<typename Xt>
  class has_finish_callback
  {
    template<typename Ot>
    static  auto test(int) -> decltype(std::declval<Ot>().thread_finish_event(), std::true_type());

    template<typename Ot>
    static  auto test(...) -> std::false_type;

    public:
    static  constexpr bool value = std::is_same<decltype(test<Xt>(0)), std::true_type>::value;
  };

  template<typename Xt>
  class has_error_callback
  {
    template<typename Ot>
    static  auto test(int) -> decltype(
                std::declval<Ot>().thread_error_event(
                    std::declval<unsigned int>(), 
                    std::declval<void*>()
                ),
                std::true_type()
            );

    template<typename Ot>
    static  auto test(...) -> std::false_type;

    public:
    static  constexpr bool value = std::is_same<decltype(test<Xt>(0)), std::true_type>::value;
  };

  protected:
  static  constexpr int           affinity_none =-1;
  static  constexpr int           affinity_min = 0;
  static  constexpr int           affinity_max = 65535;
  static  constexpr unsigned int  e_thread_nostart = e_intern_base + 1;
  static  constexpr unsigned int  e_thread_affinity_noset = e_intern_base + 4;

  public:
          thread_base() noexcept;
          thread_base(const thread_base&) noexcept = delete;
          thread_base(thread_base&&) noexcept = delete;
          ~thread_base();
          thread_base& operator=(const thread_base&) noexcept = delete;
          thread_base& operator=(thread_base&&) noexcept = delete;
};
/*namespace parallel*/ }
#endif
