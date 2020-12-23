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
#include "sio.h"
#include <cstring>

      sio::sio() noexcept:
      m_resource(fragment::get_default()),
      m_data_head(nullptr),
      m_read_iter(nullptr),
      m_read_size(0),
      m_data_size(0)
{
}

      sio::sio(fragment* resource) noexcept:
      m_resource(resource),
      m_data_head(nullptr),
      m_read_iter(nullptr),
      m_read_size(0),
      m_data_size(0)
{
}

      sio::sio(const char* text, std::size_t size) noexcept:
      sio()
{
      load(text, size);
}

      sio::sio(fragment* resource, const char* text, std::size_t size) noexcept:
      sio(resource)
{
      load(text, size);
}

      sio::sio(const sio& copy) noexcept:
      sio()
{
      assign(copy);
}

      sio::sio(sio&& copy) noexcept:
      sio()
{
      assign(std::move(copy));
}

      sio::~sio()
{
      reset(false);
}

void  sio::assign(const sio& copy) noexcept
{
      if(this != std::addressof(copy)) {
          reset(false);
          m_resource  = copy.m_resource;
          m_data_head = copy.m_data_head;
          m_read_iter = copy.m_read_iter;
          m_read_size = copy.m_read_size;
          m_data_size = copy.m_data_size;
      }
}

void  sio::assign(sio&& copy) noexcept
{
      if(this != std::addressof(copy)) {
          reset(false);
          m_resource  = copy.m_resource;
          m_data_head = copy.m_data_head;
          m_read_iter = copy.m_read_iter;
          m_read_size = copy.m_read_size;
          m_data_size = copy.m_data_size;
          copy.reset(true);
      }
}

int   sio::get_char() noexcept
{
      int l_result = EOF;
      if(m_read_iter - m_data_head < m_read_size) {
          l_result =*m_read_iter;
          m_read_iter++;
      }
      return l_result;
}

unsigned int sio::get_byte() noexcept
{
      unsigned int l_result = static_cast<unsigned int>(EOF);
      if(m_read_iter - m_data_head < m_read_size) {
          l_result = static_cast<unsigned int>(*m_read_iter);
          m_read_iter++;
      }
      return l_result;
}

std::int32_t sio::load(const char* data, std::size_t size) noexcept
{
      if(data) {
          if(size == 0) {
              size = std::strlen(data) + 1;
              if(size > static_cast<std::size_t>(std::numeric_limits<std::int32_t>::max())) {
                  return 0;
              }
          }
          if(reserve(size)) {
              std::memcpy(m_data_head, data, size);
              m_read_iter = m_data_head;
              m_read_size = size;
              return size;
          }
      }
      return 0;
}

std::int32_t sio::seek(std::int32_t offset, std::int32_t whence) noexcept
{
      if(whence == SEEK_SET) {
          if(offset < m_read_size) {
              m_read_iter = m_data_head + offset;
          } else
              return -1;
      } else
      if(whence == SEEK_CUR) {
          std::int32_t l_pos = m_read_iter - m_data_head;
          if(offset > 0) {
              if(l_pos > 0) {
                  if(offset > m_read_size - l_pos) {
                      return -1;
                  }
              }
          } else
          if(offset < 0) {
              if(l_pos < 0) {
                  if(offset < 0 - l_pos) {
                      return -1;
                  }
              }
          }
          m_read_iter = m_read_iter + offset;
      } else
      if(whence == SEEK_END) {
          if(offset > 0) {
              return -1;
          } else
          if(offset < 0 - (m_read_iter - m_data_head)) {
              return -1;
          }
          m_read_iter = m_read_iter + m_read_size + offset;
      }
      return m_read_iter - m_data_head;
}

std::int32_t sio::read(std::size_t count) noexcept
{
      if(count > 0) {
          std::int32_t l_result;
          std::int32_t l_offset_0 = m_read_iter - m_data_head;
          std::int32_t l_offset_1 = l_offset_0 + count;
          if(l_offset_1 <= m_read_size) {
              l_result = count;
          } else
              l_result = m_read_size - l_offset_0;
          m_read_iter += l_result;
          return l_result;
      }
      return 0;
}

std::int32_t sio::read(std::size_t count, char* memory) noexcept
{
      if(memory != nullptr) {
          char* l_copy_ptr = memory;
          char* l_copy_pos = memory;
          int   l_char;
          while(count) {
              l_char = get_char();
              if(l_char != EOF) {
                  *l_copy_pos = l_char;
                  l_copy_pos++;
              } else
                  break;
              count--;
          }
          return l_copy_pos - l_copy_ptr;
      }
      return read(count);
}

std::int32_t sio::put_char(char value) noexcept
{
      return write(1, std::addressof(value));
}

std::int32_t sio::put_byte(unsigned char value) noexcept
{
      return write(1, reinterpret_cast<char*>(std::addressof(value)));
}

std::int32_t sio::write(std::size_t size, const char* data) noexcept
{
      if(data) {
          if(size) {
              std::int32_t l_read_size = m_read_size + size;
              if(reserve(l_read_size)) {
                  std::memcpy(m_data_head + m_read_size, data, size);
                  m_read_size = l_read_size;
                  return size;
              }
          }
      }
      return 0;
}

std::int32_t sio::get_size() noexcept
{
      return m_read_size;
}

std::int32_t  sio::get_capacity() const noexcept
{
      return m_data_size;
}

bool  sio::is_seekable() const noexcept
{
      return true;
}

bool  sio::is_readable() const noexcept
{
      return true;
}

bool  sio::is_writable() const noexcept
{
      return false;
}

bool  sio::reserve(std::size_t size) noexcept
{
      char*         l_data_head;
      std::uint32_t l_size_next = 0;
      std::uint32_t l_size_prev = m_data_size;
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
                      m_read_iter = m_data_head + (m_read_iter - l_data_head);
                      m_data_size = l_size_new;
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
                      m_read_iter = m_data_head + (m_read_iter - l_data_head);
                      m_data_size = l_size_new;
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

void  sio::reset(bool clear) noexcept
{
      if(m_data_head) {
          m_resource->deallocate(m_data_head, m_data_size, alignof(std::size_t));
          m_data_head = nullptr;
          if(clear) {
              m_read_iter = nullptr;
              m_read_size = 0;
              m_data_size = 0;
          }
      }
}

void  sio::release() noexcept
{
      m_data_head = nullptr;
}

      sio::operator sys::ios*() noexcept
{
      return this;
}

      sio::operator bool() const noexcept
{
      return m_data_head;
}

sio&  sio::operator=(const sio& rhs) noexcept
{
      assign(rhs);
      return *this;
}

sio&  sio::operator=(sio&& rhs) noexcept
{
      assign(std::move(rhs));
      return *this;
}
