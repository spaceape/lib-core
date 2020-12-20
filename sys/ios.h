#ifndef sys_ios_h
#define sys_ios_h
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
#include <os.h>
#include <sys.h>
#include <cstring>

namespace sys {

/* ios
   abstract base class for data streams;
*/
class ios
{
  protected:
  inline  std::int32_t lsb_get() noexcept {
          return 0;
  }

  inline  std::int32_t msb_get() noexcept {
          return 0;
  }

  public:
          ios() noexcept;
          ios(const ios&) noexcept;
          ios(ios&&) noexcept;
  virtual ~ios();

  virtual std::int32_t seek(std::int32_t, std::int32_t) noexcept = 0;
 
  template<typename... Args>
  inline  std::int32_t lsb_get(char& value, Args&&... next) noexcept {
          value = get_char();
          return  lsb_get(std::forward<Args>(next)...) + 1;
  }

  template<typename... Args>
  inline  std::int32_t msb_get(char& value, Args&&... next) noexcept {
          value = get_char();
          return  msb_get(std::forward<Args>(next)...) + 1;
  }

  /* le_get<char>
     get one char from stream in little endian
  */
  template<typename... Args>
  inline  std::int32_t lsb_get(unsigned char& value, Args&&... next) noexcept {
          value = get_byte();
          return  lsb_get(std::forward<Args>(next)...) + 1;
  }

  /* be_get<char>
     get one char from stream in big_endian
  */
  template<typename... Args>
  inline  std::int32_t msb_get(unsigned char& value, Args&&... next) noexcept {
          value = get_byte();
          return  msb_get(std::forward<Args>(next)...) + 1;
  }

  /* le_get<type>
     get an object of given <type> in little endian; this is meant for arithmetic types,
     use read() and alike for structs.
  */
  template<typename Xt, typename... Args>
  inline  std::int32_t lsb_get(Xt* value, Args&&... next) noexcept {    
          int   l_size = sizeof(Xt);
          auto  l_data = reinterpret_cast<char*>(value);
          if constexpr (os::is_lsb) {
              for(int l_index = 0; l_index < l_size; l_index++) {
                  l_data[l_index] = get_byte();
              }
          } else
          if constexpr (os::is_msb) {
              for(int l_index = l_size - 1; l_index >= 0; l_index--) {
                  l_data[l_index] = get_byte();
              }
          }
          return lsb_get(std::forward<Args>(next)...) + l_size;
  }

  /* msb_get<type>
     get an object of given <type> in big endian; this is meant for arithmetic types,
     use read() and alike for structs.
  */
  template<typename Xt, typename... Args>
  inline  std::int32_t msb_get(Xt* value, Args&&... next) noexcept {    
          int   l_size = sizeof(Xt);
          auto  l_data = reinterpret_cast<char*>(value);
          if constexpr (os::is_lsb) {
              for(int l_index = l_size - 1; l_index >= 0; l_index--) {
                  l_data[l_index] = get_byte();
              }
          } else
          if constexpr (os::is_msb) {
              for(int l_index = 0; l_index < l_size; l_index++) {
                  l_data[l_index] = get_byte();
              }
          }
          return msb_get(std::forward<Args>(next)...) + l_size;
  }

  template<typename Xt, typename... Args>
  inline  std::int32_t lsb_get(Xt& value, Args&&... next) noexcept {
          return lsb_get(std::addressof(value), std::forward<Args>(next)...);
  }

  template<typename Xt, typename... Args>
  inline  std::int32_t msb_get(Xt& value, Args&&... next) noexcept {
          return msb_get(std::addressof(value), std::forward<Args>(next)...);
  }

  template<typename Xt, typename... Args>
  inline  std::int32_t get(Xt& value, Args&&... next) noexcept {
          if constexpr (os::is_lsb) {
              return lsb_get(value, std::forward<Args>(next)...);
          } else
              return msb_get(value, std::forward<Args>(next)...);
  }

  /* get_char()
     extract signed char from the stream and return its value or -1 if unable to
  */
  virtual int          get_char() noexcept = 0;

  /* get_byte()
     extract unsigned char from the stream and return its value or UINT_MAX if unable to
  */
  virtual unsigned int get_byte() noexcept = 0;
  
  virtual std::int32_t read(std::size_t) noexcept = 0;
  virtual std::int32_t read(std::size_t, char*) noexcept = 0;

  inline  std::int32_t lsb_put() noexcept {
          return 0;
  }

  template<typename... Args>
  inline  std::int32_t lsb_put(char value, Args&&... next) noexcept {
          return put_char(value) + lsb_put(std::forward<Args>(next)...);
  }

  template<typename... Args>
  inline  std::int32_t msb_put(char value, Args&&... next) noexcept {
          return put_char(value) + msb_put(std::forward<Args>(next)...);
  }

  /* lsb_put<char>
     place one char onto stream in little endian
  */
  template<typename... Args>
  inline  std::int32_t lsb_put(unsigned char value, Args&&... next) noexcept {
          return  put_byte(value) + lsb_put(std::forward<Args>(next)...);
  }

  /* msb_put<char>
     place one char onto stream in big_endian
  */
  template<typename... Args>
  inline  std::int32_t msb_put(unsigned char value, Args&&... next) noexcept {
          return  put_byte(value) + msb_put(std::forward<Args>(next)...);
  }

  /* lsb_put()
     place string onto the stream
  */
  template<typename... Args>
  inline  std::int32_t lsb_put(char* value, Args&&... next) noexcept {
          return  put_str(value) + lsb_put(std::forward<Args>(next)...);
  }

  /* msb_put()
     place string onto the stream
  */
  template<typename... Args>
  inline  std::int32_t msb_put(char* value, Args&&... next) noexcept {
          return  put_str(value) + msb_put(std::forward<Args>(next)...);
  }

  /* lsb_put()
     place string onto the stream
  */
  template<typename... Args>
  inline  std::int32_t lsb_put(const char* value, Args&&... next) noexcept {
          return  put_str(value) + lsb_put(std::forward<Args>(next)...);
  }

  /* msb_put()
     place string onto the stream
  */
  template<typename... Args>
  inline  std::int32_t msb_put(const char* value, Args&&... next) noexcept {
          return  put_str(value) + msb_put(std::forward<Args>(next)...);
  }

  /* lsb_put<type>
     place an object of given <type> in little endian onto the stream
  */
  template<typename Xt, typename... Args>
  inline  std::int32_t lsb_put(Xt* value, Args&&... next) noexcept {    
          int   l_size = sizeof(Xt);
          auto  l_data = reinterpret_cast<char*>(value);
          if constexpr (os::is_lsb) {
                write(l_size, l_data);
          } else
          if constexpr (os::is_msb) {
              for(int l_index = l_size - 1; l_index >= 0; l_index--) {
                  put_byte(l_data[l_index]);
              }
          }
          return lsb_put(std::forward<Args>(next)...) + l_size;
  }

  /* msb_get<type>
     get an object of given <type> in big endian; this is meant for arithmetic types,
     use read() and alike for structs.
  */
  template<typename Xt, typename... Args>
  inline  std::int32_t msb_put(Xt* value, Args&&... next) noexcept {    
          int   l_size = sizeof(Xt);
          auto  l_data = reinterpret_cast<char*>(value);
          if constexpr (os::is_lsb) {
              for(int l_index = l_size - 1; l_index >= 0; l_index--) {
                  put_byte(l_data[l_index]);
              }
          } else
          if constexpr (os::is_msb) {
              write(l_size, l_data);
          }
          return msb_put(std::forward<Args>(next)...) + l_size;
  }

  template<typename Xt, typename... Args>
  inline  std::int32_t lsb_put(Xt& value, Args&&... next) noexcept {
          return lsb_put(std::addressof(value), std::forward<Args>(next)...);
  }

  template<typename Xt, typename... Args>
  inline  std::int32_t msb_put(Xt& value, Args&&... next) noexcept {
          return msb_put(std::addressof(value), std::forward<Args>(next)...);
  }

  template<typename Xt, typename... Args>
  inline  std::int32_t put(Xt& value, Args&&... next) noexcept {
          if constexpr (os::is_lsb) {
              return lsb_put(value, std::forward<Args>(next)...);
          }
          if constexpr (os::is_msb) {
              return msb_put(value, std::forward<Args>(next)...);
          }
  }

  /* put_str()
     place an object of given <type> in little endian onto the stream
  */
  inline  std::int32_t put_str(const char* value, std::int32_t length = 0) noexcept {   
          std::int32_t l_length = length;
          if(length == 0) {
              if(value) {
                  l_length = std::strlen(value);
              } else
                  l_length = 0;
          } else
              l_length = length;
          return write(l_length, value);
  }

  /* put_char()
     place a signed char onto the stream
  */
  virtual std::int32_t put_char(char) noexcept = 0;

  /* put_byte()
     place an unsigned signed char onto the stream
  */
  virtual std::int32_t put_byte(std::uint8_t) noexcept = 0;

  virtual std::int32_t write(std::size_t, const char*) noexcept = 0;

  virtual bool is_seekable() const noexcept = 0;
          bool is_serial() const noexcept;
  virtual bool is_readable() const noexcept = 0;
  virtual bool is_writable() const noexcept = 0;
  virtual std::int32_t get_size() noexcept = 0;

          ios& operator=(const ios&) noexcept;
          ios& operator=(ios&&) noexcept;
};

/*namespace sys*/ }
#endif
