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
#include "parallel.h"
#include "parallel/posix-thread-base.h"
#include "parallel/atomic-thread-base.h"
#include "parallel/thread.h"

namespace parallel {

/* thread_base
*/
      thread_base::thread_base() noexcept
{
}

      thread_base::~thread_base()
{
}

/*namespace parallel*/ }

/* posix_thread_base
*/
      posix_thread_base::posix_thread_base(int affinity) noexcept
{
      m_handle = 0;
      pthread_mutexattr_t l_attr;
      pthread_mutexattr_init(std::addressof(l_attr));
      pthread_mutexattr_settype(std::addressof(l_attr), PTHREAD_MUTEX_NORMAL);
      pthread_mutex_init(std::addressof(m_mutex), std::addressof(l_attr));
      pthread_cond_init(std::addressof(m_condition), nullptr);
      if(affinity) {
          if(affinity > affinity_none) {
              if(affinity < affinity_max) {
                  m_affinity = affinity;
              } else
                  m_affinity = affinity_none;
          } else
              m_affinity = affinity_none;
      } else
          m_affinity = affinity_none;
      m_running = 0;
      m_awake = 0;
}

      posix_thread_base::~posix_thread_base()
{
      thread_suspend();
      pthread_cond_destroy(std::addressof(m_condition));
      pthread_mutex_destroy(std::addressof(m_mutex));
}

void  posix_thread_base::thread_lock(posix_thread_base* base) noexcept
{
      pthread_mutex_lock(std::addressof(base->m_mutex));
}

void  posix_thread_base::thread_unlock(posix_thread_base* base) noexcept
{
      pthread_mutex_unlock(std::addressof(base->m_mutex));
}

bool  posix_thread_base::thread_set_affinity(posix_thread_base* base, int affinity) noexcept
{
      cpu_set_t l_bits;
      CPU_ZERO(std::addressof(l_bits));
      if(affinity > affinity_none) {
          if(affinity < affinity_max) {
              int cpu;
              int count = CPU_COUNT(std::addressof(l_bits));
              for(cpu = 0; cpu != count; cpu++) {
                  CPU_SET(cpu, std::addressof(l_bits));
              }
          } else
              return false;
      } else
          CPU_SET(affinity, std::addressof(l_bits));
      if(pthread_setaffinity_np(base->m_handle, sizeof(cpu_set_t), std::addressof(l_bits))) {
          return false;
      }
      base->m_affinity = affinity;
      return true;
}

void  posix_thread_base::thread_wake(posix_thread_base* base) noexcept
{
      pthread_cond_signal(std::addressof(base->m_condition));
}

void  posix_thread_base::thread_wait(posix_thread_base* base) noexcept
{
      base->m_awake = 0;
      pthread_cond_wait(std::addressof(base->m_condition), std::addressof(base->m_mutex));
      base->m_awake = 1;
}

bool  posix_thread_base::thread_wait(posix_thread_base* base, int delay_ms) noexcept
{
      struct timespec l_time;
      if(delay_ms > 0) {
          if(clock_gettime(CLOCK_REALTIME, std::addressof(l_time)) == 0) {
              l_time.tv_nsec = delay_ms * parallel::nspms;
              if(l_time.tv_nsec >= parallel::nspms) {
                  l_time.tv_sec += l_time.tv_nsec / parallel::nsps;
                  l_time.tv_nsec = l_time.tv_nsec % parallel::nsps;
                  return pthread_cond_timedwait(std::addressof(base->m_condition), std::addressof(base->m_mutex), std::addressof(l_time)) == 0;
              } else
                  return false;
          } else
              return false;
      } else
          return true;
}

void  posix_thread_base::thread_yield() noexcept
{
      pthread_yield();
}

void  posix_thread_base::thread_exit(posix_thread_base*, void* result) noexcept
{
}

bool  posix_thread_base::thread_resume(void* interface, void*(*function)(void*)) noexcept
{
      int  l_error;
      bool l_result = false;
      pthread_mutex_lock(std::addressof(m_mutex));
      if(m_handle == 0) {
          if(m_running == 0) {
              pthread_attr_t l_attr;
              pthread_attr_init(std::addressof(l_attr));
              pthread_attr_setdetachstate(std::addressof(l_attr), PTHREAD_CREATE_JOINABLE);
              l_error = pthread_create(std::addressof(m_handle), nullptr, function, interface);
              if(l_error == 0) {
                  m_running = 1;
                  m_awake = 1;
              }
              pthread_attr_destroy(std::addressof(l_attr));
          }
          l_result = m_running;
      }
      pthread_mutex_unlock(std::addressof(m_mutex));
      return l_result;
}

bool  posix_thread_base::thread_suspend() noexcept
{
      bool  l_result;
      pthread_mutex_lock(std::addressof(m_mutex));
      if(m_running) {
          m_running = 0;
          if(m_awake == 0) {
              pthread_cond_signal(std::addressof(m_condition));
          } else
              m_awake = 0;
          pthread_mutex_unlock(std::addressof(m_mutex));
          l_result = pthread_join(m_handle, std::addressof(m_return)) == 0;
          pthread_mutex_lock(std::addressof(m_mutex));
          m_handle = 0;
      } else
          l_result = true;
      pthread_mutex_unlock(std::addressof(m_mutex));
      return l_result;
}

bool  posix_thread_base::is_running(bool value) noexcept
{
      bool l_result;
      pthread_mutex_lock(std::addressof(m_mutex));
      if(m_awake) {
          l_result = value;
      } else
          l_result = !value;
      pthread_mutex_unlock(std::addressof(m_mutex));
      return l_result;
}

bool  posix_thread_base::is_idle() noexcept
{
      return is_running(false);
}

void* posix_thread_base::get_exit_code() noexcept
{
      void* l_result;
      pthread_mutex_lock(std::addressof(m_mutex));
      l_result = m_return;
      pthread_mutex_unlock(std::addressof(m_mutex));
      return l_result;
}

/* atomic_thread_base
 * lock-free thread base
*/
      atomic_thread_base::atomic_thread_base(int affinity):
      posix_thread_base(affinity)
{
}

      atomic_thread_base::~atomic_thread_base()
{
}

