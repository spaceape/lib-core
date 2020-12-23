#ifndef memory_flat_map_traits_h
#define memory_flat_map_traits_h
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

namespace memory {
  
template<typename Kt, typename Xt>
struct flat_map_traits
{
  using  key_type   = Kt;
  using  value_type = typename std::remove_cv<Xt>::type;

  struct node_type
  {
    key_type   key;
    value_type value;

    public:
    template<typename... Args>
    inline  node_type(key_type id, Args&&... args) noexcept:
            key(id),
            value(std::forward<Args>(args)...) {
    }

    inline  node_type(const node_type& copy) noexcept:
            key(copy.key),
            value(copy.value) {
    }

    inline  node_type(node_type&& copy) noexcept:
            key(std::move(copy.key)),
            value(std::move(copy.value)) {
            copy.release();
    }

    inline  void release() noexcept {
    }

    inline  bool operator<(key_type rhs) const noexcept {
            return key < rhs;
    }

    inline  bool operator<=(key_type rhs) const noexcept {
            return key <= rhs;
    }

    inline  bool operator>(key_type rhs) const noexcept {
            return key > rhs;
    }

    inline  bool operator>=(key_type rhs) const noexcept {
            return key >= rhs;
    }

    inline  bool operator==(key_type rhs) const noexcept {
            return key == rhs;
    }

    inline  bool operator!=(key_type rhs) const noexcept {
            return key != rhs;
    }
  
    inline  node_type& operator=(const node_type& rhs) noexcept {
            key = rhs.key;
            value = rhs.value;
            return *this;
    }

    inline  node_type& operator=(node_type&& rhs) noexcept {
            key = std::move(rhs.key);
            value = std::move(rhs.value);
            rhs.release();
            return *this;
    }
  };

  using  base_type = std::pmr::vector<node_type>;
  using  iterator_type = typename base_type::iterator;
};

/*namespace memory*/ }
#endif
