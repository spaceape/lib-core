#ifndef parallel_queue_base_h
#define parallel_queue_base_h
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
#include "posix-thread-base.h"
#include "atomic-thread-base.h"

namespace parallel {

template<typename Ct>
class queue_base: public Ct
{
  using base_type = Ct;

  protected:
  template<typename Xt, typename At>
  class has_accept_callback
  {
    template<typename Ot, typename Pt>
    static  auto test(int) -> decltype(std::declval<Ot>().queue_accept(std::declval<Pt>()), std::true_type());

    template<typename Ot, typename Pt>
    static  auto test(...) -> std::false_type;

    public:
    static  constexpr bool value = std::is_same<decltype(test<Xt, At>(0)), std::true_type>::value;
  };

  template<typename Xt, typename At>
  class has_discard_callback
  {
    template<typename Ot, typename Pt>
    static  auto test(int) -> decltype(std::declval<Ot>().queue_discard(std::declval<Pt>()), std::true_type());

    template<typename Ot, typename Pt>
    static  auto test(...) -> std::false_type;

    public:
    static  constexpr bool value = std::is_same<decltype(test<Xt, At>(0)), std::true_type>::value;
  };

  template<typename Xt>
  class has_wake_callback
  {
    template<typename Ot>
    static  auto test(int) -> decltype(std::declval<Ot>().thread_wake_event(), std::true_type());

    template<typename Ot>
    static  auto test(...) -> std::false_type;

    public:
    static  constexpr bool value = std::is_same<decltype(test<Xt>(0)), std::true_type>::value;
  };

  template<typename Xt>
  class has_sleep_callback
  {
    template<typename Ot>
    static  auto test(int) -> decltype(std::declval<Ot>().thread_sleep_event(), std::true_type());

    template<typename Ot>
    static  auto test(...) -> std::false_type;

    public:
    static  constexpr bool value = std::is_same<decltype(test<Xt>(0)), std::true_type>::value;
  };

  template<typename Xt, typename At>
  class has_execute_delegate
  {
    template<typename Ot, typename Pt>
    static  auto test(int) -> decltype(std::declval<Ot>().queue_execute(std::declval<Pt>()), std::true_type());

    template<typename Ot, typename Pt>
    static  auto test(...) -> std::false_type;

    public:
    static  constexpr bool value = std::is_same<decltype(test<Xt, At>(0)), std::true_type>::value;
  };

  bool    m_enable;
  
  public:
  template<typename... Args>
  inline  queue_base(Args... args) noexcept:
          base_type(std::forward<Args>(args)...),
          m_enable(true) {
  }

  inline  queue_base(const queue_base& copy) noexcept:
          base_type(copy), 
          m_enable(copy.m_enable) {
  }

  inline  queue_base(queue_base&& copy) noexcept:
          base_type(std::move(copy)),
          m_enable(copy.m_enable) {
  }

  virtual ~queue_base() {
  }

  inline  void  set_enabled(bool value) noexcept {
          m_enable = value;
  }

  inline  void  set_disabled() noexcept {
          m_enable = false;
  }

  inline  bool  is_enabled() const noexcept {
          return m_enable;
  }

  inline  queue_base& operator=(const queue_base& rhs) noexcept {
          base_type::operator=(rhs);
          m_enable = rhs.m_enable;
          return *this;
  }

  inline  queue_base& operator=(queue_base&& rhs) noexcept {
          base_type::operator=(std::move(rhs));
          m_enable = rhs.m_enable;
          return *this;
  }
};

/*namespace parallel*/ }
#endif
