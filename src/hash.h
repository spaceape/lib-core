#ifndef core_hash_h
#define core_hash_h
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
#include "global.h"

/* basic Fowler–Noll–Vo hash implementation in 32 and 64 bit variants
*/
template<typename Ht>
struct hash_traits
{
  static constexpr std::uint32_t  key  = 16777619u;
  static constexpr std::uint32_t  bias = 2166136261u;
  static constexpr std::size_t    size = 4u;
};

template<>
struct hash_traits<std::uint32_t>
{
  static constexpr std::uint32_t  key  = 16777619u;
  static constexpr std::uint32_t  bias = 2166136261u;
  static constexpr std::size_t    size = 4u;

  using value_type = std::uint32_t;
  
  union data_type {
    std::uint32_t value;
    std::uint8_t  bytes[size];
  };
};

template<>
struct hash_traits<std::uint64_t>
{
  static constexpr std::uint64_t  key  = 1099511628211u;
  static constexpr std::uint64_t  bias = 14695981039346656037u;
  static constexpr std::size_t    size = 8u;

  using value_type = std::uint64_t;

  union data_type {
    std::uint64_t value;
    std::uint8_t  bytes[size];
  };
};

template<typename Ht>
struct hash
{
  static_assert("Unsupported hash type.");
};

template<>
struct hash<std::uint32_t>
{
  using traits    = hash_traits<std::uint32_t>;
  using data_type = typename traits::data_type;

  static void set(std::uint32_t& value, std::uint32_t bias = hash_traits<std::uint32_t>::bias) noexcept
  {
          value = bias;
  }

  static void add(std::uint32_t& value, const char* bytes, std::size_t length) noexcept
  {
          if(bytes) {
              while(length) {
                  value *= hash_traits<std::uint32_t>::key;
                  value ^=*bytes++;
                  length--;
              }
          }
  }

  static void add(std::uint32_t& value, const char* string) noexcept
  {
          if(string) {
              while(string[0]) {
                  value *= hash_traits<std::uint32_t>::key;
                  value ^= string[0];
                  string++;
              }
          }
  }
  
  static void add(std::uint32_t& value, void* object) noexcept
  {
          add(value, reinterpret_cast<const char*>(reinterpret_cast<std::size_t>(object)), sizeof(std::size_t));
  }

  template<typename Xt>
  static void add(std::uint32_t& value, const Xt* object) noexcept
  {
          add(value, reinterpret_cast<void*>(object));
  }

  template<typename Xt>
  static void add(std::uint32_t& value, const Xt& object) noexcept
  {
          add(value, reinterpret_cast<const char*>(std::addressof(object)), sizeof(object));
  }

  template<typename Xt, std::size_t N>
  static void add(std::uint32_t& value, const Xt object[N]) noexcept
  {
          add(value, reinterpret_cast<const char*>(std::addressof(object[0])), sizeof(object) * N);
  }
};

template<>
struct hash<std::uint64_t>
{
  using traits    = hash_traits<std::uint64_t>;
  using data_type = typename traits::data_type;

  static void set(std::uint64_t& value, std::uint64_t bias = hash_traits<std::uint64_t>::bias) noexcept
  {
          value = bias;
  }

  static void add(std::uint64_t& value, const char* bytes, std::size_t length) noexcept
  {
          if(bytes) {
              while(length) {
                  value *= hash_traits<std::uint64_t>::key;
                  value ^=*bytes++;
                  length--;
              }
          }
  }
  static void add(std::uint64_t& value, const char* string) noexcept
  {
          if(string) {
              while(string[0]) {
                  value *= hash_traits<std::uint64_t>::key;
                  value ^= string[0];
                  string++;
              }
          }
  }
  
  static void add(std::uint64_t& value, void* object) noexcept
  {
          add(value, reinterpret_cast<const char*>(reinterpret_cast<size_t>(object)), sizeof(std::size_t));
  }

  template<typename Xt>
  static void add(std::uint64_t& value, const Xt* object) noexcept
  {
          add(value, reinterpret_cast<void*>(object));
  }

  template<typename Xt>
  static void add(std::uint64_t& value, const Xt& object) noexcept
  {
          add(value, reinterpret_cast<const char*>(std::addressof(object)), sizeof(object));
  }

  template<typename Xt, std::size_t N>
  static void add(std::uint64_t& value, const Xt object[N]) noexcept
  {
          add(value, reinterpret_cast<const char*>(std::addressof(object[0])), sizeof(object) * N);
  }
};

template<typename Ht>
class hash_ptr
{
  using data_type = typename hash_traits<Ht>::data_type;
  using value_type = typename hash_traits<Ht>::value_type;

  data_type m_hash;

  public:
  inline  hash_ptr() noexcept {
  }

  template<typename Xt>
  inline  hash_ptr(typename std::remove_cv<Xt>::type* other) noexcept {
          hash_traits<Ht>::set(m_hash.value);
          hash_traits<Ht>::add(m_hash.value, other);
  }

  template<typename Xt>
  inline  hash_ptr(const typename std::remove_cv<Xt>::type* other) noexcept {
          hash_traits<Ht>::set(m_hash.value);
          hash_traits<Ht>::add(m_hash.value, other);
  }

  template<typename Xt>
  inline  hash_ptr(const typename std::remove_cv<Xt>::type& other) noexcept {
          hash_traits<Ht>::set(m_hash.value);
          hash_traits<Ht>::add(m_hash.value, std::addressof(other));
  }

  inline  hash_ptr(const hash_ptr& copy) noexcept:
          m_hash(copy.m_hash) {
  }

  inline  hash_ptr(hash_ptr&& copy) noexcept:
          m_hash(copy.m_hash) {
  }

  inline  ~hash_ptr() {
  }

  inline  value_type get() const noexcept {
          return m_hash.value;
  }

  inline  void set(const hash_ptr& other) noexcept {
          return m_hash.value = other.m_hash.value;
  }
  
  inline  void set(hash_ptr&& other) noexcept {
          set(other);
  }

  template<typename Xt>
  inline  void set(typename std::remove_cv<Xt>::type* other) noexcept {
          hash_traits<Ht>::set(m_hash.value);
          hash_traits<Ht>::add(m_hash.value, other);
  }

  template<typename Xt>
  inline  void set(const typename std::remove_cv<Xt>::type* other) noexcept {
          hash_traits<Ht>::set(m_hash.value);
          hash_traits<Ht>::add(m_hash.value, other);
  }

  template<typename Xt>
  inline  void set(const typename std::remove_cv<Xt>::type& other) noexcept {
          hash_traits<Ht>::set(m_hash.value);
          hash_traits<Ht>::add(m_hash.value, std::addressof(other));
  }

  inline  void reset() noexcept {
          hash_traits<Ht>::set(m_hash.value);        
  }

  inline  value_type release() noexcept {
          value_type l_result = m_hash.value;
          reset();
          return l_result;
  }

  inline  std::uint8_t operator[](int index) const noexcept {
          return m_hash.bytes[index];
  }

  inline  operator value_type() const noexcept {
          return m_hash.value;
  }

  inline  operator bool() const noexcept {
          return m_hash.value;
  }

  inline  bool operator<(const hash_ptr& rhs) const noexcept {
          return m_hash.value < rhs.m_hash.value;
  }

  inline  bool operator<=(const hash_ptr& rhs) const noexcept {
          return m_hash.value <= rhs.m_hash.value;
  }

  inline  bool operator>(const hash_ptr& rhs) const noexcept {
          return m_hash.value > rhs.m_hash.value;
  }

  inline  bool operator>=(const hash_ptr& rhs) const noexcept {
          return m_hash.value >= rhs.m_hash.value;
  }
  
  inline  bool operator==(const hash_ptr& rhs) const noexcept {
          return m_hash.value == rhs.m_hash.value;
  }

  inline  bool operator!=(const hash_ptr& rhs) const noexcept {
          return m_hash.value != rhs.m_hash.value;
  }

  inline  hash_ptr& operator=(const hash_ptr& rhs) noexcept {
          m_hash = rhs.m_hash;
          return *this;
  }

  inline  hash_ptr& operator=(hash_ptr&& rhs) noexcept {
          m_hash = rhs.m_hash;
          return *this;
  }
};
#endif
