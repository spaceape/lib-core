#ifndef sys_asio_h
#define sys_asio_h
/** 
    Copyright (c) 2016-2020, wicked systems
    All rights reserved.

    Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following
    conditions are met:
    * Redistributions of source code must retain the above copyright notice, this list of conditions and the following
      disclaimer.
    * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following 
      disclaimer in the documentation and/or other materials provided with the distribution.
    * Neither the name of wicked systems nor the names of its contributors may be used to endorse or promote products derived
      from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
    INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
    SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
    CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
    EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**/
#include "ios.h"
#include "ios/fio.h"
#include "ios/bio.h"

namespace sys {

/* asio
   bufferred ios chain with an internal source of type Xt
*/
template<typename Xt = fio>
class asio: public bio
{
  Xt      m_source;

  public:
  template<typename... Args>
  inline  asio(Args&&... args) noexcept:
          bio(),
          m_source(std::forward<Args>(args)...) {
          set_source(std::addressof(m_source));
  }

  template<typename... Args>
  inline  asio(fragment* resource, Args&&... args) noexcept:
          bio(resource, nullptr),
          m_source(std::forward<Args>(args)...) {
          set_source(std::addressof(m_source));
  }

  inline  asio(const asio& copy) noexcept:
          bio(copy),
          m_source(copy.m_source) {
          set_source(std::addressof(m_source));
  }
  
  inline  asio(asio&& copy) noexcept:
          bio(std::move(copy)),
          m_source(std::move(copy.m_source)) {
          set_source(std::addressof(m_source));
  }

  inline  Xt* operator->() noexcept {
          return std::addressof(m_source);
  }
  
  inline  asio& operator=(const asio& rhs) noexcept {
          if(std::addressof(rhs) != this) {
              bio::operator=(rhs);
              m_source.operator=(rhs.m_source);
              set_source(std::addressof(m_source));
          }
          return *this;
  }

  inline  asio& operator=(asio&& rhs) noexcept {
          if(std::addressof(rhs) != this) {
              bio::operator=(std::move(rhs));
              m_source.operator=(std::move(rhs.m_source));
              set_source(std::addressof(m_source));
          }
          return *this;
  }
};

/* asio<fio>
   bufferred file I/O specialisation
*/
template<>
class asio<fio>: public bio
{
  fio     m_source;

  public:
  template<typename... Args>
  inline  asio(Args&&... args) noexcept:
          bio(),
          m_source(std::forward<Args>(args)...) {
          set_source(std::addressof(m_source));
  }

  template<typename... Args>
  inline  asio(fragment* resource, Args&&... args) noexcept:
          bio(resource, nullptr),
          m_source(std::forward<Args>(args)...) {
          set_source(std::addressof(m_source));
  }

  inline  asio(const asio& copy) noexcept:
          bio(copy),
          m_source(copy.m_source) {
          set_source(std::addressof(m_source));
  }
  
  inline  asio(asio&& copy) noexcept:
          bio(std::move(copy)),
          m_source(std::move(copy.m_source)) {
          set_source(std::addressof(m_source));
  }

  inline  bool set_blocking(bool value = true) noexcept {
          return m_source.set_blocking(value);
  }
  
  inline  bool set_nonblocking() noexcept {
          return m_source.set_nonblocking();
  }
  
  inline  int  get_fd() const noexcept {
          return m_source.get_fd();
  }
  
  inline  operator int() const noexcept {
          return m_source.get_fd();
  }

  inline  operator ios*() noexcept {
          return this;
  }

  inline  operator bool() const noexcept {
          return m_source.get_fd() >= 0;
  }

  inline  fio* operator->() noexcept {
          return std::addressof(m_source);
  }

  inline  asio& operator=(const asio& rhs) noexcept {
          if(std::addressof(rhs) != this) {
              bio::operator=(rhs);
              m_source.operator=(rhs.m_source);
              set_source(std::addressof(m_source));
          }
          return *this;
  }

  inline  asio& operator=(asio&& rhs) noexcept {
          if(std::addressof(rhs) != this) {
              bio::operator=(std::move(rhs));
              m_source.operator=(std::move(rhs.m_source));
              set_source(std::addressof(m_source));
          }
          return *this;
  }
};

/*namespace sys*/ }
#endif
