#ifndef memory_flat_list_h
#define memory_flat_list_h
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
#include <vector>

namespace memory {

template<typename Xt>
class flat_list: public std::pmr::vector<Xt>
{
  using  base_type = typename std::pmr::vector<Xt>;

  public:
  using  node_type = typename std::remove_cv<Xt>::type;

  using  iterator = typename base_type::iterator;
  using  const_iterator = typename base_type::const_iterator;

  using  iter_type = iterator;

  static constexpr size_t elements_min = global::cache_small_max;
  static constexpr size_t elements_max = 0;

  public:
  inline  flat_list(std::pmr::memory_resource* r) noexcept:
          base_type(r) {
  }

  inline  flat_list(std::pmr::memory_resource* r, size_t reserve) noexcept:
          base_type(r) {
          base_type::reserve(reserve);
  }

  inline  flat_list(const flat_list& copy) noexcept:
          base_type(copy) {
  }

  inline  flat_list(flat_list&& copy) noexcept:
          base_type(std::move(copy)) {
  }

          ~flat_list() {
  }

  inline  iterator find(const node_type node) noexcept {
          for(auto it = base_type::begin(); it != base_type::end(); it++) {
              if(it.operator*() == node) {
                  return it;
              }
          }
          return base_type::end();
  }

  inline  bool contains(const node_type node) const noexcept {
          for(auto it = base_type::cbegin(); it != base_type::cend(); it++) {
              if(it.operator*() == node) {
                  return true;
              }
          }
          return false;
  }

  inline  flat_list& operator=(const flat_list& rhs) noexcept {
          base_type::operator=(rhs);
          return *this;
  }

  inline  flat_list& operator=(flat_list&& rhs) noexcept {
          base_type::operator=(std::move(rhs));
          return *this;
  }
};
/*namespace memory*/ }
#endif
