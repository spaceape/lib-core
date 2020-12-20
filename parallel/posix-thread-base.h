#ifndef parallel_posix_thread_base_h
#define parallel_posix_thread_base_h
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
#include "thread-base.h"
#include <pthread.h>

class posix_thread_base: public parallel::thread_base
{
  pthread_t     m_handle;
  pthread_mutex_t m_mutex;
  pthread_cond_t m_condition;

  using base_type = parallel::thread_base;

  protected:
  void*         m_return;
  int           m_affinity;
  unsigned int  m_running:1;
  unsigned int  m_awake:1;

  protected:
  static  void  thread_lock(posix_thread_base*) noexcept;
  static  void  thread_unlock(posix_thread_base*) noexcept;
  static  bool  thread_set_affinity(posix_thread_base*, int) noexcept;
  static  void  thread_wake(posix_thread_base*) noexcept;
  static  void  thread_wait(posix_thread_base*) noexcept;
  static  bool  thread_wait(posix_thread_base*, int) noexcept;
  static  void  thread_yield() noexcept;
  static  void* thread_join(posix_thread_base*) noexcept;
  static  void  thread_exit(posix_thread_base*, void*) noexcept;

  template<typename Ct>
  static  void* thread_boot(void* p) noexcept {
          void* l_result = nullptr;
          Ct*   l_object = reinterpret_cast<Ct*>(p);
          int   l_affinity = affinity_none;
          if(l_object) {
              thread_lock(l_object);
              if(l_object->m_running) {
                  if constexpr (base_type::has_launch_callback<Ct>::value) {
                      l_object->thread_launch_event();
                  }
                  while(l_object->m_running) {
                      if(l_object->m_affinity != l_affinity) {
                          if(thread_set_affinity(l_object, l_object->m_affinity)) {
                              l_affinity = l_object->m_affinity;
                          } else
                          if constexpr (base_type::has_error_callback<Ct>::value) {
                              if(l_object->thread_error(e_thread_affinity_noset, l_result) == false) {
                                  break;
                              }
                          }
                      }
                      l_result = l_object->thread_loop(l_object);
                  }
                  if constexpr (base_type::has_finish_callback<decltype(l_object)>::value) {
                      l_object->thread_finish_event();
                  }
                  l_object->m_running = 0;
                  l_object->m_awake = 1;
              }
              thread_unlock(l_object);
              thread_exit(l_object, l_result);
              return l_result;
          }
          return nullptr;
  }

  protected:
          bool  thread_resume(void*, void*(*)(void*)) noexcept;
          bool  thread_suspend() noexcept;

  public:
          posix_thread_base(int = affinity_none) noexcept;
          posix_thread_base(const posix_thread_base&) noexcept = delete;
          posix_thread_base(posix_thread_base&&) noexcept = delete;
          ~posix_thread_base();
          bool  is_running(bool = true) noexcept;
          bool  is_idle() noexcept;
          void* get_exit_code() noexcept;
          posix_thread_base& operator=(const posix_thread_base&) noexcept = delete;
          posix_thread_base& operator=(posix_thread_base&&) noexcept = delete;
};
#endif
