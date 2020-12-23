#ifndef sys_var_h
#define sys_var_h
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
#include <sys.h>
#include <traits.h>
#include <char.h>
#include <convert.h>
#include <traits.h>

namespace sys {

const char* get_environment_variable(const char*) noexcept;
const char* set_environment_variable(const char*, const char*) noexcept;

template<typename Xt, std::size_t NameSize, std::size_t DataSize, typename At>
class var
{
  using value_type = typename std::remove_cv<Xt>::type;

  char_ptr<NameSize, At>  m_name;
  value_type              m_value;
  bool                    m_defined;

  public:
          var(const char* name, const At& allocator = std::pmr::get_default_resource()) noexcept:
          m_name(name, allocator),
          m_value(),
          m_defined(false) {
  }

          var(const var& copy) noexcept:
          m_name(copy.m_name),
          m_value(),
          m_defined(copy.m_defined) {
  }

          var(var&& copy) noexcept:
          m_name(std::move(copy.m_name)),
          m_value(),
          m_defined(copy.m_defined) {
  }

          ~var() {
  }

  inline  const char*   get_name() const noexcept {
          return m_name;
  }

  inline  value_type    get_value() noexcept {
          if(m_defined == false) {
              m_value = get_environment_variable(m_name);
              m_defined = true;
          }
          return m_value;
  }

  inline  bool  is_defined() const noexcept {
          return m_defined;
  }


  inline  operator value_type() noexcept {
          return get_value();
  }

  inline  var& operator=(const value_type&) noexcept {
          return *this;
  }

  inline  var& operator=(const var&) noexcept {
          return *this;
  }

  inline  var& operator=(var&&) noexcept {
          return *this;
  }
};

template<std::size_t NameSize, std::size_t DataSize, typename At>
class var<char*, NameSize, DataSize, At>
{
  char_ptr<NameSize, At>  m_name;
  char_ptr<DataSize, At>  m_data;
  bool                    m_defined;

  public:
          var(const char* name, const At& allocator = std::pmr::get_default_resource()) noexcept:
          m_name(name, allocator),
          m_data(allocator),
          m_defined(false) {
  }

          var(const var& copy) noexcept:
          m_name(copy.m_name),
          m_data(copy.m_data),
          m_defined(copy.m_defined) {
  }

          var(var&& copy) noexcept:
          m_name(std::move(copy.m_name)),
          m_data(std::move(copy.m_data)),
          m_defined(copy.m_defined) {
  }

          ~var() {
  }

  inline  const char*   get_name() const noexcept {
          return m_name;
  }

  inline  const char*   get_value() noexcept {
          if(m_defined == false) {
              m_data    = get_environment_variable(m_name);
              m_defined = true;
          }
          return m_data;
  }

  inline  void  set_value(const char* value) noexcept {
          m_data    = set_environment_variable(m_name, value);
          m_defined = true;
  }

          void  fmt_value(const char* value, ...) noexcept {
          va_list va;
          va_start(va, value);
          m_defined = set_environment_variable(m_name, m_data.fmt_v(value, va));
          va_end(va);
  }

  inline  bool  is_defined() const noexcept {
          return m_defined;
  }

  inline  operator const char*() noexcept {
          return get_value();
  }

  inline  var& operator=(const char* value) noexcept {
          set_value(value);
          return *this;
  }

  inline  var& operator=(const var&) noexcept {
          return *this;
  }

  inline  var& operator=(var&&) noexcept {
          return *this;
  }
};

template<typename Xt, std::size_t NameSize, typename At>
class var<Xt, NameSize, 0, At>
{
  using value_type = typename std::remove_cv<Xt>::type;

  char_ptr<NameSize, At>  m_name;

  public:
          var(const char* name, const At& allocator = std::pmr::get_default_resource()) noexcept:
          m_name(name, allocator) {
  }

          var(const var& copy) noexcept:
          m_name(copy.m_name) {
  }

          var(var&& copy) noexcept:
          m_name(std::move(copy.m_name)) {
  }

          ~var() {
  }

  inline  const char*   get_name() const noexcept {
          return m_name;
  }

  inline  value_type    get_value() const noexcept {
          value_type    l_result;
          convert<value_type>::get_value(l_result, get_environment_variable(m_name));
          return l_result;
  }

  inline  operator value_type() const noexcept {
          return get_value();
  }

  inline  var& operator=(const var&) noexcept {
          return *this;
  }

  inline  var& operator=(var&&) noexcept {
          return *this;
  }
};

template<std::size_t NameSize, typename At>
class var<char*, NameSize, 0, At>
{
  char_ptr<NameSize, At>  m_name;

  public:
          var(const char* name, const At& allocator = std::pmr::get_default_resource()) noexcept:
          m_name(name, allocator) {
  }

          var(const var& copy) noexcept:
          m_name(copy.m_name) {
  }

          var(var&& copy) noexcept:
          m_name(std::move(copy.m_name)) {
  }

          ~var() {
  }

  inline  const char*   get_name() const noexcept {
          return m_name;
  }

  inline  const char*   get_value() const noexcept {
          return get_environment_variable(m_name);
  }

  inline  void          set_value(const char* value) const noexcept {
          set_environment_variable(m_name, value);
  }

  inline  operator const char*() const noexcept {
          return get_value();
  }

  inline  var& operator=(const char* value) noexcept {
          set_value(value);
          return *this;
  }

  inline  var& operator=(const var&) noexcept {
          return *this;
  }

  inline  var& operator=(var&&) noexcept {
          return *this;
  }
};
/*namespace sys*/ }
#endif
