#ifndef parallel_queue_h
#define parallel_queue_h
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
#include <traits.h>
#include <none.h>
#include <vector>
#include "queue-base.h"

namespace parallel {

/* queue
 * flat execution queue with no consumer
*/
template<typename Xt, typename Ct, typename Vt>
class queue: public queue_base<none>, protected Vt
{
  using base_type = queue_base<none>;
  using pool_type = Vt;
  using node_type = typename pool_type::value_type;
  using iter_type = typename pool_type::iterator;

  private:
  off_t   m_live;

  protected:
  virtual bool queue_accept(node_type&) noexcept {
          return true;
  }

  virtual bool queue_discard(node_type&) noexcept {
          return true;
  }

  public:
  using iterator = typename pool_type::iterator;

  public:
  inline  queue() noexcept:
          base_type(),
          pool_type(),
          m_live(-1) {
  }

  template<typename... Args>
  inline  queue(Args... args) noexcept:
          base_type(std::forward<Args>(args)...),
          pool_type() {
  }

  inline  queue(const queue& copy) noexcept:
          queue() {
          assign(copy);
  }

  inline  queue(queue&& copy) noexcept:
          queue() {
          assign(std::move(copy));
  }

  virtual ~queue() {
  }

  inline  auto get_head() noexcept {
          return pool_type::begin();
  }

  inline  auto get_tail() noexcept {
          return pool_type::end();
  }

  template<typename... Args>
  inline  bool enqueue(Args&&... args) noexcept {
          if(base_type::is_enabled()) {
              if(pool_type::size() < (size_t)std::numeric_limits<int>::max()) {
                  node_type& l_node = pool_type::emplace_back(std::forward<Args>(args)...);
                  if(queue_accept(l_node)) {
                      if(m_live < 0) {
                          m_live = 0;
                      }
                      return true;
                  }
                  pool_type::pop_back();
              }
          }
          return false;
  }

  inline  bool dequeue() noexcept {
          node_type& l_node = pool_type::back();
          if(queue_discard(l_node)) {
              if(m_live > -1) {
                  if(m_live == (ssize_t)pool_type::size() - 1) {
                      m_live =-1;
                  }
              }
              pool_type::pop_back();
              return true;
          }
          return false;
  }

  inline  bool assign(const queue& copy) noexcept {
          if(this != std::addressof(copy)) {
              if(copy.size()) {
                  iter_type l_head = copy.begin();
                  iter_type l_tail = copy.m_live;
                  if(l_head != l_tail) {
                      copy.erase(l_head, l_tail);
                  }
              }
              pool_type::clear();
              if(copy.size()) {
                  pool_type::insert(pool_type::begin(), copy.begin(), copy.end());
                  m_live = 0;
              } else
                  m_live = 1;
              return true;
          }
          return false;
  }

  inline  bool assign(queue&& copy) noexcept {
          if(assign(copy)) {
              copy.clear();
              return true;
          }
          return false;
  }

  inline  node_type* shift() noexcept {
          if(pool_type::size()) {
              if(m_live < (ssize_t)pool_type::size()) {
                  return std::addressof(pool_type::at(m_live++));
              }
              pool_type::clear();
              m_live =-1;
          }
          return nullptr;
  }

  inline  void clear() noexcept {
          pool_type::clear();
  }

  inline  queue& operator=(const queue& rhs) noexcept {
          assign(rhs);
  }

  inline  queue& operator=(queue&& rhs) noexcept {
          assign(std::move(rhs));
  }
};

/* queue
 * posix consumer queue
*/
template<typename Xt, typename Vt>
class queue<Xt, posix_thread_base, Vt>: public queue_base<posix_thread_base>, protected Vt
{
  using base_type = queue_base<posix_thread_base>;
  using pool_type = Vt;
  using node_type = typename pool_type::value_type;
  using iter_type = typename pool_type::iterator;

  private:
  off_t m_live;
  off_t m_last;

  protected:
  template<typename Ct>
  inline  void* thread_loop(Ct* consumer) noexcept {
          if(m_live < 0) {
              if(m_last >= 0) {
                  pool_type::clear();
                  if constexpr (base_type::has_sleep_callback<Ct>::value) { 
                      consumer->thread_sleep_event();
                  }
              }
              thread_wait(this);
          } else
          if(m_live >= 0) {
              if(m_live < (ssize_t)pool_type::size()) {
                  if(m_last < 0) {
                      if constexpr (base_type::has_wake_callback<Ct>::value) {
                          consumer->thread_wake_event();
                      }
                  }
                  thread_unlock(this);
                  if constexpr (base_type::has_execute_delegate<Ct, node_type>::value) {
                      consumer->queue_execute(pool_type::at(m_live));
                  } else
                      pool_type::at(m_live)();
                  thread_lock(this);
                  m_last = m_live++;
              } else
                  m_live =-1;
          }
          return nullptr;
  }

  virtual bool queue_accept(node_type&) noexcept {
          return true;
  }

  virtual bool queue_discard(node_type&) noexcept {
          return true;
  }

  friend class has_launch_callback<queue>;
  friend class has_finish_callback<queue>;
  friend class posix_thread_base;

  public:
  template<typename... Args>
  inline  queue(Args... args) noexcept:
          base_type(std::forward<Args>(args)...),
          pool_type(),
          m_live(-1),
          m_last(-1) {
  }

          queue(const queue&) noexcept = delete;
          queue(queue&&) noexcept = delete;

  virtual ~queue() {
  }

  /* enqueue()
  */
  template<typename... Args>
          bool enqueue(Args... args) noexcept {
          if(base_type::is_enabled()) {
              base_type::thread_lock(this);
              if(pool_type::size() < (size_t)std::numeric_limits<int>::max()) {
                  node_type& l_node = pool_type::emplace_back(std::forward<Args>(args)...);
                  if(queue_accept(l_node)) {
                      if(m_live < 0) {
                          m_live = 0;
                          base_type::thread_wake(this);
                      }
                      base_type::thread_unlock(this);
                      return true;
                  }
                  pool_type::pop_back();
              }
              base_type::thread_unlock(this);
          }
          return false;
  }

  /* dequeue()
  */
          bool dequeue() noexcept {
          bool l_result = false;
          base_type::thread_lock(this);
          node_type& l_node = pool_type::back();
          l_result = queue_discard(l_node);
          if(l_result) {
              if(m_live > -1) {
                  if(m_live == pool_type::size() - 1) {
                      m_live =-1;
                  }
              }
              pool_type::pop_back();
          }
          base_type::thread_lock(this);
          return l_result;
  }

  inline  bool resume() noexcept {
          return thread_resume(this, thread_boot<queue>);
  }

  inline  bool suspend() noexcept {
          return thread_suspend();
  }

  inline  queue& operator=(const queue& copy) noexcept = delete;
  inline  queue& operator=(queue&&) noexcept = delete;
};

/*namespace parallel*/ }
#endif
