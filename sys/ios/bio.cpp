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
#include "bio.h"
#include <cstring>

namespace sys {

      constexpr size_t s_lock_max = 255;
      constexpr size_t s_read_min = 64;
      constexpr size_t s_read_max = std::numeric_limits<std::uint32_t>::max() / 4;

      bio::bio() noexcept:
      bio(fragment::get_default(), nullptr)
{
}

      bio::bio(ios* io) noexcept:
      bio(fragment::get_default(), io)
{
}

      bio::bio(fragment* resource, ios* io) noexcept:
      m_io(io),
      m_resource(resource),
      m_data_head(nullptr),
      m_file_pos(-1),
      m_read_pos(-1),
      m_read_iter(0),
      m_read_size(0),
      m_data_tail(nullptr),
      m_lock_ctr(0),
      m_enable_bit(true),
      m_commit_bit(false)
{
      if(m_io) {
          if(m_io->is_seekable()) {
              m_file_pos = m_io->seek(0, SEEK_CUR);
              m_read_pos = m_file_pos;
          } else
              m_file_pos = 0;
      }
}

      bio::bio(const bio& copy) noexcept:
      m_io(copy.m_io),
      m_resource(nullptr),
      m_data_head(nullptr),
      m_file_pos(copy.m_file_pos),
      m_read_pos(-1),
      m_read_iter(0),
      m_read_size(0),
      m_data_tail(nullptr),
      m_lock_ctr(0),
      m_enable_bit(true),
      m_commit_bit(false)
{
}

      bio::bio(bio&& copy) noexcept:
      m_io(std::move(copy.m_io)),
      m_resource(std::move(copy.m_resource)),
      m_data_head(copy.m_data_head),
      m_file_pos(copy.m_file_pos),
      m_read_pos(copy.m_read_pos),
      m_read_iter(copy.m_read_iter),
      m_read_size(copy.m_read_size),
      m_data_tail(copy.m_data_tail),
      m_lock_ctr(copy.m_lock_ctr),
      m_enable_bit(copy.m_enable_bit),
      m_commit_bit(copy.m_commit_bit)
{
      copy.release();
}

      bio::~bio()
{
      reset(false);
}

/* load()
   fill the internal data buffer with data from the source stream;
   unless the buffer is locked - as there is no other way to access the internal data, we can
   get away with just advancing the internal data pointers without actively reading anything
   from file; a periodic seek() is used to make sure that the pointers don't exceed the
   actual file boundaries - so this scheme will obvoiusly not work for serial (unseekable)
   streams;
   <force> will bypass this behaviour for internal use
*/
std::int32_t bio::load(std::size_t count, bool force) noexcept
{
      if(m_file_pos >= 0) {
          if(count < s_read_max) {
              off_t       l_file_pos  = m_read_pos + m_read_iter;
              std::size_t l_copy_size = 0;
              std::size_t l_load_size = 0;
              bool        l_perform   = force || is_locked() || is_serial();
              if(m_read_iter >= 0) {
                  // check if there's a latent seek() and if so, compute the copy and load
                  // sizes
                  if(m_read_iter < m_read_size) {
                      l_copy_size = m_read_size - m_read_iter;
                      if(l_copy_size < count) {
                          l_load_size = count - l_copy_size;
                          if(l_perform) {
                              if(l_load_size > 0) {
                                  // // reserve and resize the internal buffer
                                  // std::size_t  l_data_size = m_data_tail - m_data_head;
                                  // if(l_data_size < count) {
                                  //     if(m_lock_ctr == 0) {
                                  //         reserve(count);
                                  //     } else
                                  //         return -1;
                                  // }
                                  // compact the internal buffer
                                  m_read_pos = l_file_pos;
                                  m_read_size = m_read_size - m_read_iter; 
                                  std::memmove(m_data_head, m_data_head + m_read_iter, m_read_size);
                                  m_read_iter = 0;
                              }
                          }
                      } else
                          l_copy_size = count;
                  } else
                      l_load_size = count;
              } else
                  l_load_size = count;

              // load data into the internal buffer
              if(l_load_size > 0) {
                  // prepare
                  if(l_perform == true) {
                      std::size_t  l_data_size = m_data_tail - m_data_head;
                      if(l_data_size < count) {
                          if(m_lock_ctr == 0) {
                              reserve(count);
                          } else
                              return 0;
                      }
                  }
                  // seek
                  if(l_perform == true) {
                      if(m_file_pos != m_read_pos + m_read_size) {
                          if(m_io->is_seekable()) {
                              if(m_io->seek(l_file_pos, SEEK_SET) >= 0) {
                                  m_file_pos = l_file_pos;
                              } else
                                  return 0;
                          } else
                              return 0;
                      }
                  } else
                  if(l_perform == false) {
                      l_file_pos = m_io->seek(l_file_pos + l_load_size, SEEK_SET);
                      if(l_file_pos >= 0) {
                          m_file_pos = l_file_pos;
                      }
                  }
                  // load
                  if(l_perform == true) {
                      std::size_t l_free_size = m_data_tail - m_data_head - m_read_size;
                      if(l_load_size > l_free_size) {
                          if(s_read_min < l_free_size) {
                              l_load_size = s_read_min;
                          } else
                              l_load_size = l_free_size;
                      }
                      l_load_size = m_io->read(l_load_size, m_data_head + m_read_size);
                      m_file_pos  = m_file_pos + l_load_size;
                      m_read_size = m_read_size + l_load_size;
                  }
              }
              return count;
          }
      }
      return 0;
}

/* seek()
   move the file cursor, as documented by ::lseek();
   seek() tries to be as lazy as possible, especially in relative mode (whence == SEEK_CUR)
*/
std::int32_t bio::seek(std::int32_t offset, std::int32_t whence) noexcept
{
      if(whence == SEEK_SET) {
          // seek at an absolute position within the file: if offset points inside our buffer
          // update m_file_pos and return success
          if(offset >= 0) {
              if(m_file_pos >= 0) {
                  off_t l_move_pos = offset - m_read_pos;
                  if(l_move_pos >= 0) {
                      if(m_lock_ctr > 0) {
                          if(l_move_pos > 0) {
                              return -1;
                          }
                      }
                      if(l_move_pos < m_read_size) {
                          m_read_iter = l_move_pos;
                          return offset;
                      }
                  }
                  if(m_lock_ctr == 0) {
                      if(m_io->is_seekable()) {
                          m_read_iter = l_move_pos;
                          return  offset;
                      }
                  }
              }
          }
      } else
      if(whence == SEEK_CUR) {
          // relative seek:
          // if offset points inside our buffer - update m_file_pos and return success;
          // if current pos is unknown, retrieving it means seeking as well, so just do an
          // immediate seek instead, if supported
          if(m_file_pos >= 0) {
              off_t l_move_pos = offset + m_read_pos;
              if(l_move_pos >= 0) {
                  if(m_lock_ctr > 0) {
                      if(l_move_pos > 0) {
                          return -1;
                      }
                  }
                  if(l_move_pos < m_read_size) {
                      m_read_iter = l_move_pos;
                      return m_read_pos + m_read_iter;
                  }
              }
              if(m_lock_ctr == 0) {
                  if(m_io->is_seekable()) {
                      m_read_iter = l_move_pos;
                      return m_read_pos + m_read_iter;
                  }
              }
          }
      } else
      if(whence == SEEK_END) {
          // seek at the end of the file: no way to tell where that is unless the parent
          // stream supports it
          if(m_io->is_seekable()) {
              off_t l_seek_pos = m_io->seek(offset, SEEK_END);
              if(l_seek_pos >= 0) {
                  return  seek(l_seek_pos + offset, SEEK_SET);
              }
          }
      }
      return 0;
}

/* lock()
*/
std::int32_t bio::lock() noexcept
{
      if(m_lock_ctr < s_lock_max) {
          m_lock_ctr++;
      }
      return seek(0, SEEK_CUR);
}

/* get_char()
*/
int   bio::get_char() noexcept
{
      if(auto l_read_size = load(1, true); l_read_size >= 1) {
          auto l_read_char = m_data_head[m_read_iter++];
          return l_read_char;
      }
      return EOF;
}

/* get_char()
*/
unsigned int bio::get_byte() noexcept
{
      if(auto l_read_size = load(1, true); l_read_size >= 1) {
          auto l_read_char = static_cast<unsigned int>(m_data_head[m_read_iter++]);
          return l_read_char;
      }
      return static_cast<unsigned int>(EOF);
}

/* read()
   find the size of the file and fully read into the internal buffer
*/
std::int32_t  bio::read() noexcept
{
      std::int32_t l_base = 0;
      std::int32_t l_size = get_size();
      if(l_size > 0) {
          seek(l_base, SEEK_SET);
          load(l_size, true);
      }
      return l_size;
}

/* read()
   read data into the internal buffer
*/
std::int32_t  bio::read(std::size_t count) noexcept
{
      return  load(count, false);
}

/* read()
   read from input stream to memory;
   tries to optimise I/O by eliminating seeks() and replacing reads() with copies from the
   internal buffer, whenever positions and sizes allow.
*/
std::int32_t  bio::read(std::size_t count, char* memory) noexcept
{
      if(m_file_pos >= 0) {
          if(count < s_read_max) {
              off_t       l_file_pos  = m_read_pos + m_read_iter;
              std::size_t l_copy_size = 0;
              std::size_t l_load_size = 0;
              off_t       l_result    = 0;
              if(m_read_iter >= 0) {
                  // check if there's a latent seek() and if so, compute the copy and load
                  // sizes
                  if(m_read_iter < m_read_size) {
                      l_copy_size = m_read_size - m_read_iter;
                      if(l_copy_size < count) {
                          l_load_size = count - l_copy_size;
                      } else
                          l_copy_size = count;
                  } else
                      l_load_size = count;

                  // seek and load the necessary data from file
                  if(l_load_size > 0) { 
                      if(m_file_pos != l_file_pos + static_cast<std::int32_t>(l_copy_size)) {
                          if(m_io->is_seekable()) {
                              if(m_io->seek(l_file_pos, SEEK_SET) >= 0) {
                                  m_file_pos = l_file_pos;
                              } else
                                  return 0;
                          } else
                              return 0;
                      }
                      l_load_size  = m_io->read(l_load_size, memory + l_copy_size);
                      m_file_pos  += l_load_size;
                      l_result    += l_load_size;
                  }
                  // copy the data from the internal buffer
                  if(l_copy_size > 0) {
                      std::memcpy(memory, m_data_head + m_read_iter, l_copy_size);
                      l_result += l_copy_size;
                  }
              } else
              if(m_read_iter < 0) {
                  // seek() is always necessary in this scenario, so it must be possible;
                  // also, if <count> is large enough to require a read beyond the limits of
                  // the internal buffer then give up copying from internal bufer altogether
                  if(m_io->is_seekable()) {
                      if(m_read_iter + count > 0) {
                          if(m_read_iter + static_cast<std::int32_t>(count) < m_read_size) {
                              l_copy_size = m_read_iter - m_read_pos + count;
                              l_load_size = m_read_pos - m_read_iter;
                          } else
                              l_load_size = count;
                      } else
                          l_load_size = count;
                  } else
                      return 0;

                  // copy the output buffer and update the internal file pointer
                  if(auto l_seek_ret = m_io->seek(l_file_pos, SEEK_SET); l_seek_ret >= 0) {
                      m_file_pos = l_seek_ret;
                      if(auto l_load_ret = m_io->read(l_load_size, memory); l_load_ret >= static_cast<std::int32_t>(l_load_size)) {
                          m_file_pos += l_load_ret;
                          l_result   += l_load_ret;
                          if(l_copy_size > 0) {
                              std::memcpy(memory + l_load_size, m_data_head, l_copy_size);
                              l_result += l_copy_size;
                          }
                      } else
                          return 0;
                  } else
                      return 0;
              }

              // finally update the remaining internal pointers
              if(is_seekable()) {
                  if(l_load_size == 0) {
                      if(l_copy_size > 0) {
                          m_read_iter += count;
                      }
                  }
                  if(l_load_size  > 0) {
                      m_read_pos  = m_file_pos;
                      m_read_iter = 0;
                      m_read_size = 0;
                  }
              }
              return l_result;
          }
      }
      return 0;
}

std::int32_t  bio::put_char(char value) noexcept
{
      return write(1, std::addressof(value));
}

std::int32_t  bio::put_byte(unsigned char value) noexcept
{
      return write(1, reinterpret_cast<char*>(std::addressof(value)));
}

std::int32_t  bio::write(std::size_t count, const char* buffer) noexcept
{
      return 0;
}

char* bio::get_data() noexcept
{
      if(read()) {
          if(reserve(m_read_size + 8)) {
              m_data_head[m_read_size] = 0;
              return m_data_head;
          }
      }
      return nullptr;
}

std::int32_t  bio::get_size() noexcept
{
      return m_io->get_size();
}

void  bio::unlock() noexcept
{
      if(m_lock_ctr > 0) {
          m_lock_ctr--;
      }
}

bool  bio::is_seekable() const noexcept
{
      return m_io->is_seekable();
}

bool  bio::is_readable() const noexcept
{
      return m_io->is_seekable();
}

bool  bio::is_writable() const noexcept
{
      return m_io->is_seekable();
}

bool  bio::is_locked() const noexcept
{
      return m_lock_ctr;
}

bool  bio::reserve(std::size_t size) noexcept
{
      char*       l_data_head;
      std::size_t l_size_next = 0;
      std::size_t l_size_prev = m_data_tail - m_data_head;
      if(size > l_size_prev) {
          if(m_resource->has_fixed_size()) {
          // resource is static (a fixed block of memory has already been reserved for it)
          // resizing within its limits will be essentially a no-op
          // resizing outside its limits will tragically fail
              l_size_next = m_resource->get_fixed_size();
          } else
          if(m_resource->has_variable_size()) {
          // resource allows realloc()
          // - if not previously allocated, simply alloc();
          // - if empty, don't realloc(), simply alloc() - that saves an expensive and
          //   unnecessary move operation with garbage data;
          // - proceed as normal otherwise
              if(m_data_head == nullptr) {
                  l_size_next = m_resource->get_alloc_size(size);
              } else
              if(m_read_size == 0) {
                  l_size_next = m_resource->get_alloc_size(size);
              } else
              if(std::size_t l_size_exp = m_resource->get_alloc_size(size); l_size_exp <= (std::size_t)std::numeric_limits<std::int32_t>::max()) {
                  auto  l_size_new = l_size_exp;
                  auto  l_copy_ptr = reinterpret_cast<char*>(m_resource->reallocate(m_data_head, l_size_prev, l_size_exp, alignof(std::size_t)));
                  if(l_copy_ptr) {
                      l_data_head = m_data_head;
                      m_data_head = l_copy_ptr;
                      m_data_tail = l_copy_ptr + l_size_new;
                  } else
                      return false;
              } else
                  return false;
          } else
          if(m_data_head == nullptr) {
              // resource is neither static nor resizable - only the first allocation
              // succeeds
              l_size_next = m_resource->get_alloc_size(size);
          } else
              return false;

          if(l_size_next > l_size_prev) {
              if(l_size_next <= (std::size_t)std::numeric_limits<int>::max()) {
                  auto  l_size_new = l_size_next;
                  auto  l_copy_ptr = reinterpret_cast<char*>(m_resource->allocate(l_size_next, alignof(std::size_t)));
                  if(l_copy_ptr) {
                      l_data_head = m_data_head;
                      m_data_head = l_copy_ptr;
                      m_data_tail = l_copy_ptr + l_size_new;
                      if(m_data_head != l_data_head) {
                          if(l_data_head != nullptr) {
                              m_resource->deallocate(l_data_head, l_size_prev, alignof(std::size_t));
                          }
                      }
                  } else
                      return false;
              } else
                  return false;
          }
      }
      return m_data_head != nullptr;
}

void  bio::reset(bool clear) noexcept
{
      if(m_data_head) {
          if(m_commit_bit) {
          }
          m_resource->deallocate(m_data_head, m_data_tail - m_data_head);
      }
      if(clear) {
          release();
      }
}

void  bio::release() noexcept
{
      m_io         = nullptr;
      m_resource   = nullptr;
      m_data_head  = nullptr;
      m_file_pos   =-1;
      m_read_pos   =-1;
      m_read_iter  = 0;
      m_read_size  = 0;
      m_data_tail  = nullptr;
      m_lock_ctr   = 0;
      m_enable_bit = true;
      m_commit_bit = false;
}

      bio::operator ios*() noexcept
{
      return this;
}

      bio::operator bool() const noexcept
{
      return m_io;
}

bio&  bio::operator=(const bio& rhs) noexcept
{
      if(std::addressof(rhs) != this) {
          reset(false);
          m_io         = rhs.m_io;
          m_data_head  = nullptr;
          m_file_pos   = rhs.m_file_pos;
          m_read_pos   =-1;
          m_read_iter  = 0;
          m_read_size  = 0;
          m_data_tail  = nullptr;
          m_lock_ctr   = 0;
          m_enable_bit = true;
          m_commit_bit = false;
      }
      return *this;
}

bio&  bio::operator=(bio&& rhs) noexcept
{
      if(std::addressof(rhs) != this) {
          reset(false);
          m_io         = rhs.m_io;
          m_resource   = rhs.m_resource;
          m_data_head  = rhs.m_data_head;
          m_file_pos   = rhs.m_file_pos;
          m_read_pos   = rhs.m_read_pos;
          m_read_iter  = rhs.m_read_iter;
          m_read_size  = rhs.m_read_size;
          m_data_tail  = rhs.m_data_tail;
          m_lock_ctr   = rhs.m_lock_ctr;
          m_enable_bit = rhs.m_enable_bit;
          m_commit_bit = rhs.m_commit_bit;
          rhs.release();
      }
      return *this;
}

/*namespace sys*/ }
