#ifndef sys_bio_h
#define sys_bio_h
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

/* bio
   buffer for buffered io
*/
class bio: public sys::ios
{
  ios*          m_io;
  fragment*     m_resource;

  protected:
  char*         m_data_head;
  std::int32_t  m_file_pos;         // actual file pointer, before the last read operation
  std::int32_t  m_read_pos;         // internal file pointer (buffer base)
  std::int32_t  m_read_iter;        // current position, relative to m_read_pos
  std::int32_t  m_read_size;        // size of the read data, relative to m_read_pos
  std::int32_t  m_save_size;        // size of the data pending write
  char*         m_data_tail;
  char          m_eol;
  unsigned int  m_lock_ctr:8;       // lock the buffer
  bool          m_enable_bit:1;
  bool          m_commit_bit:1;     // commit the buffer before discarding

  public:
  static constexpr int undef = -1;

  protected:
          bool  load(std::size_t, bool) noexcept;
          void  unload() noexcept;

  public:
          bio() noexcept;
          bio(ios*) noexcept;
          bio(fragment*, ios*) noexcept;
          bio(const bio&) noexcept;
          bio(bio&&) noexcept;
  virtual ~bio();

          void          set_source(ios*) noexcept;

  virtual std::int32_t  seek(std::int32_t, std::int32_t) noexcept override;

          std::int32_t  lock() noexcept;
  
  virtual int           get_char() noexcept override;
  virtual unsigned int  get_byte() noexcept override;

          std::int32_t  get_line(char*&) noexcept;
   
          std::int32_t  read() noexcept;
  virtual std::int32_t  read(std::size_t) noexcept override;
  virtual std::int32_t  read(std::size_t, char*) noexcept override;

  virtual std::int32_t  put_char(char) noexcept override;
  virtual std::int32_t  put_byte(unsigned char) noexcept override;
  virtual std::int32_t  write(std::size_t, const char*) noexcept override;

          void          flush() noexcept;

          char*         get_data() noexcept;
  virtual std::int32_t  get_size() noexcept override;
          void          unlock() noexcept;

  virtual bool  is_seekable() const noexcept override;
  virtual bool  is_readable() const noexcept override;
  virtual bool  is_writable() const noexcept override;
          bool  is_locked() const noexcept;

          bool  reserve(std::size_t) noexcept;
          void  reset(bool = true) noexcept;
          void  release() noexcept;

          std::int32_t  get_capacity() const noexcept;

          operator ios*() noexcept;
          operator bool() const noexcept;

          bio& operator=(const bio&) noexcept;
          bio& operator=(bio&&) noexcept;
};
#endif
