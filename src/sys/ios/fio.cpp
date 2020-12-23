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
#include "fio.h"
#include <unistd.h>
#include <fcntl.h>

      fio::fio() noexcept:
      m_desc(undef),
      m_mode(0),
      m_own(false),
      m_seekable(false)
{
}

      fio::fio(int desc, int mode, int permissions) noexcept:
      m_desc(desc),
      m_mode(mode & 65535),
      m_own(mode & M_ACQUIRE),
      m_seekable(false)
{
      if(desc > undef) {
          if(mode & O_NONBLOCK) {
              set_blocking(false);
          }
          m_seekable = seek(SEEK_CUR, 0) >= 0;
      }
}

      fio::fio(const char* name, int mode, int permissions) noexcept:
      fio()
{
      open(name, mode, permissions);
}

      fio::fio(const fio& copy) noexcept:
      fio()
{
      assign(copy);
}

      fio::fio(fio&& copy) noexcept:
      fio()
{
      assign(std::move(copy));
}

      fio::~fio()
{
      close(false);
}

void  fio::assign(const fio& copy) noexcept
{
      if(this != std::addressof(copy)) {
          close();
          if(copy.m_desc > undef) {
              m_desc     = ::dup(copy.m_desc);
              m_mode     = copy.m_mode;
              m_own      = true;
              m_seekable = copy.m_seekable;
          } else
              close(true);
      }
}

void  fio::assign(fio&& copy) noexcept
{
      if(this != std::addressof(copy)) {
          close();
          if(copy.m_desc > undef) {
              m_desc     = copy.m_desc;
              m_mode     = copy.m_mode;
              m_own      = copy.m_own;
              m_seekable = copy.m_seekable;
              copy.release();
          } else
              close(true);
      }
}

bool  fio::open(const char* name, int mode, int permissions) noexcept
{
      reset();
      if(name && name[0]) {
          m_desc = ::open(name, mode, permissions);
          if(m_desc > undef) {
              m_mode = mode & 65535;
              m_own = true;
              m_seekable = seek(SEEK_CUR, 0) >= 0;
              if(mode & O_NONBLOCK) {
                  set_blocking(false);
              }
              return true;
          } else
              return false;
      } else
          return false;
}

bool  fio::copy(int desc, int mode) noexcept
{
      reset();
      m_desc     = dup(desc);
      m_mode     = mode & 65535;
      m_own      = true;
      m_seekable = seek(SEEK_CUR, 0) >= 0;
      if(mode & O_NONBLOCK) {
          set_blocking(false);
      }
      return m_desc != undef;
}

bool  fio::move(int& desc, int mode) noexcept
{
      reset();
      m_desc     = desc;
      m_mode     = mode & 65535;
      m_own      = true;
      m_seekable = seek(SEEK_CUR, 0) >= 0;
      if(m_desc > undef) {
          if(mode & O_NONBLOCK) {
              set_blocking(false);
          }
          desc = undef;
          return true;
      }
      return false;
}

int  fio::get_char() noexcept
{
      // really inefficient, don't ever use
      char l_result;
      if(::read(m_desc, std::addressof(l_result), 1) == 1) {
          return l_result;
      } else
          return EOF;
}

unsigned int  fio::get_byte() noexcept
{
      // really inefficient, don't ever use
      unsigned char l_result;
      if(::read(m_desc, std::addressof(l_result), 1) == 1) {
          return l_result;
      } else
          return EOF;
}

std::int32_t fio::seek(std::int32_t offset, std::int32_t whence) noexcept
{
      return ::lseek(m_desc, offset, whence);
}

std::int32_t fio::read(std::size_t count) noexcept
{
      return ::lseek(m_desc, count, SEEK_CUR);
}

std::int32_t fio::read(std::size_t count, char* memory) noexcept
{
      return ::read(m_desc, memory, count);
}

std::int32_t fio::put_char(char value) noexcept
{
      return write(1, std::addressof(value));
}

std::int32_t fio::put_byte(unsigned char value) noexcept
{
      return write(1, reinterpret_cast<char*>(std::addressof(value)));
}

std::int32_t fio::write(std::size_t size, const char* data) noexcept
{
      return ::write(m_desc, data, size);
}

std::int32_t fio::get_size() noexcept
{
      if(m_desc > undef) {
          off_t l_pos = ::lseek(m_desc, 0, SEEK_CUR);
          if(l_pos >= 0) {
              off_t l_size = ::lseek(m_desc, 0, SEEK_END);
              if(l_pos != l_size) {
                  ::lseek(m_desc, l_pos, SEEK_SET);
              }
              if(l_size < std::numeric_limits<std::int32_t>::max()) {
                  return l_size;
              }
          }
      }
      return 0;
}

bool  fio::set_blocking(bool value) noexcept
{
      if(int l_get_flags = fcntl(m_desc, F_GETFL, 0); l_get_flags >= 0) {
          int l_set_flags = l_get_flags;
          if(value == true) {
              l_set_flags &= ~O_NONBLOCK;
          } else
          if(value == false) {
              l_set_flags |=  O_NONBLOCK;
          }
          if(l_set_flags != l_get_flags) {
              if(int l_fnctl = fcntl(m_desc, F_SETFL, l_set_flags); l_fnctl >= 0) {
                  return true;
              }
          } else
              return true;
      }
      return false;
}

bool  fio::set_nonblocking() noexcept
{
      return set_blocking(false);
}

int   fio::get_fd() const noexcept
{
      return m_desc;
}

bool  fio::is_seekable() const noexcept
{
      return m_seekable;
}

bool  fio::is_readable() const noexcept
{
      return true;
}

bool  fio::is_writable() const noexcept
{
      return m_mode & (O_WRONLY | O_RDWR);
}

void  fio::reset() noexcept
{
      close(true);
}

int   fio::release() noexcept
{
      int l_result = m_desc;
      if(m_desc > undef) {
          m_desc = undef;
      }
      return l_result;
}

void  fio::close(bool reset) noexcept
{
      if(m_desc > undef) {
          if(m_own) {
              ::close(m_desc);
          }
          m_desc = undef;
          m_own  = false;
          if(reset) {
              m_mode     = 0;
              m_seekable = false;
          }
      }
}

      fio::operator int() const noexcept
{
      return m_desc;
}

      fio::operator sys::ios*() noexcept
{
      return this;
}

      fio::operator bool() const noexcept
{
      return m_desc > undef;
}

fio&  fio::operator=(const fio& rhs) noexcept
{
      assign(rhs);
      return *this;
}

fio&  fio::operator=(fio&& rhs) noexcept
{
      assign(std::move(rhs));
      return *this;
}
