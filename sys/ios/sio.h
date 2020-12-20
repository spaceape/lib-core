#ifndef sys_sio_h
#define sys_sio_h
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
#include <sys/ios.h>

namespace sys {

/* sio
   static data buffer
*/
class sio: public ios
{
  fragment*     m_resource;

  protected:
  char*         m_data_head;
  char*         m_read_iter;
  std::int32_t  m_read_size;
  std::int32_t  m_data_size;

  protected:
          void   assign(const sio&) noexcept;
          void   assign(sio&&) noexcept;

  public:
          sio() noexcept;
          sio(fragment*) noexcept;
          sio(const char*, std::size_t = 0) noexcept;
          sio(fragment*, const char*, std::size_t = 0) noexcept;
          sio(const sio&) noexcept;
          sio(sio&&) noexcept;
  virtual ~sio();

  virtual int           get_char() noexcept override;
  virtual unsigned int  get_byte() noexcept override;

          std::int32_t  load(const char*, std::size_t = 0) noexcept;
  virtual std::int32_t  seek(std::int32_t, std::int32_t) noexcept override;
  virtual std::int32_t  read(std::size_t) noexcept override;
  virtual std::int32_t  read(std::size_t, char*) noexcept override;

  virtual std::int32_t  put_char(char) noexcept override;
  virtual std::int32_t  put_byte(unsigned char) noexcept override;
  virtual std::int32_t  write(std::size_t, const char*) noexcept override;

  virtual std::int32_t  get_size() noexcept override;
          std::int32_t  get_capacity() const noexcept;

  virtual bool  is_seekable() const noexcept override;
  virtual bool  is_readable() const noexcept override;
  virtual bool  is_writable() const noexcept override;

          bool  reserve(std::size_t) noexcept;
          void  reset(bool = true) noexcept;
          void  release() noexcept;
          operator ios*() noexcept;
          operator bool() const noexcept;
          sio& operator=(const sio&) noexcept;
          sio& operator=(sio&&) noexcept;
};
/*namespace sys*/ }
#endif
