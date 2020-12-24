#include "test-ios.h"
#include <fcntl.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/ios/sio.h>
#include <sys/ios/bio.h>
#include <sys/asio.h>
#include <poll.h>
#include <cstring>

/* sys::sio tests
*/
bool  test_01() noexcept
{
      sio  l_sio;
      if(l_sio.get_char() == EOF) {
          return true;
      }
      return false;
}

bool  test_02() noexcept
{
      sio  l_sio;
      if(l_sio.read(0) == 0) {
          return true;
      }
      return false;
}

bool  test_03() noexcept
{
      sio  l_sio;
      if(l_sio.read(256) == 0) {
          return true;
      }
      return false;
}

bool  test_04() noexcept
{
      sio  l_sio;
      l_sio.reserve(256);
      if(l_sio.read(256) == 0) {
          return true;
      }
      return false;
}

bool  test_05() noexcept
{
      sio  l_sio;
      bool l_reserve = l_sio.reserve(256);
      if(l_reserve) {
          l_sio.load("0123456789");
          if(l_sio.read(256) == 11) {
              return true;
          }
      }
      return false;
}

bool  test_06() noexcept
{
      sio  l_sio;
      char l_text[256];
      bool l_reserve = l_sio.reserve(256);
      if(l_reserve) {
          l_sio.load("0123456789");
          if(l_sio.read(256, l_text) == 11) {
              if(std::strncmp(l_text, "0123456789", 256) == 0) {
                  return true;
              }
          }
      }
      return false;
}

bool  test_07() noexcept
{
      sio  l_sio("0123456789");
      char l_text[256];
      if(l_sio.read(256, l_text) == 11) {
          if(std::strncmp(l_text, "0123456789", 256) == 0) {
              return true;
          }
      }
      return false;
}

bool  test_08() noexcept
{
      bio l_bio;
      l_bio.read(32);
      l_bio.read();
      return true;
}

bool  test_09() noexcept
{
      bio l_bio;
      l_bio.write(16, "ohai");
      return true;
}

/* sys::bio tests
*/
bool  test_11() noexcept
{
      bio  l_bio;
      if(l_bio.get_char() == EOF) {
          return true;
      }
      return false;
}

bool  test_12() noexcept
{
      bio  l_bio;
      if(l_bio.read(0) == 0) {
          return true;
      }
      return false;
}

bool  test_13() noexcept
{
      bio  l_bio;
      if(l_bio.read(256) == 0) {
          return true;
      }
      return false;
}

bool  test_14() noexcept
{
      bio  l_bio;
      l_bio.reserve(8192);
      if(l_bio.read(256) == 0) {
          if(l_bio.get_capacity() >= 256) {
              return true;
          }
      }
      return false;
}

bool  test_15() noexcept
{
      sio  l_sio;
      bio  l_bio(l_sio);
      l_sio.write(16, "0123456789ABCDEF");
      if(l_bio.read(8) == 8) {
          if(l_bio.read(4) == 4) {
              // capacity shall be 0, the read optimisation bypassed it
              if(l_bio.get_capacity() == 0) {
                  return true;
              }
          }
      }
      return false;
}

bool  test_16() noexcept
{
      sio  l_sio;
      bio  l_bio(l_sio);
      l_sio.write(16, "0123456789ABCDEF");
      if(l_bio.read(8) == 8) {
          auto l_char_0 = l_bio.get_char();
          if(l_char_0 != '8') {
              return false;
          }
          auto l_char_1 = l_bio.get_char();
          if(l_char_1 != '9') {
              return false;
          }
          return true;
      }
      return false;
}

bool  test_17() noexcept
{
      sio  l_sio;
      bio  l_bio(l_sio);
      l_sio.write(16, "0123456789ABCDEF");
      if(l_bio.read(14) == 14) {
          auto l_char_0 = l_bio.get_char();
          if(l_char_0 != 'E') {
              return false;
          }
          auto l_char_1 = l_bio.get_char();
          if(l_char_1 != 'F') {
              return false;
          }
          auto l_char_2 = l_bio.get_char();
          if(l_char_2 != -1) {
              return false;
          }
          return true;
      }
      return false;
}

bool  test_18() noexcept
{
      sio  l_sio;
      bio  l_bio(l_sio);
      l_sio.write(16, "0123456789ABCDEF");
      if(l_bio.read(14) == 14) {
          auto l_char_0 = l_bio.get_char();
          if(l_char_0 != 'E') {
              return false;
          }
          auto l_char_1 = l_bio.get_char();
          if(l_char_1 != 'F') {
              return false;
          }
          l_sio.write(4, "UVXY");
          auto l_char_2 = l_bio.get_char();
          if(l_char_2 != 'U') {
              return false;
          }
          return true;
      }
      return false;
}

bool  test_19() noexcept
{
      sio  l_sio;
      bio  l_bio(l_sio);
      l_sio.write(16, "0123456789ABCDEF");
      if(l_bio.read(14) == 14) {
          auto l_char_0 = l_bio.get_char();
          if(l_char_0 != 'E') {
              return false;
          }
          auto l_char_1 = l_bio.get_char();
          if(l_char_1 != 'F') {
              return false;
          }
          l_sio.write(4, "UVXY");
          auto l_char_2 = l_bio.get_char();
          if(l_char_2 != 'U') {
              return false;
          }
          if(l_bio.seek(2, SEEK_CUR) != 19) {
              return false;
          }
          auto l_char_3 = l_bio.get_char();
          if(l_char_3 != 'Y') {
              return false;
          }
          return true;
      }
      return false;
}

bool  test_20() noexcept
{
      sio  l_sio;
      bio  l_bio(l_sio);
      l_sio.write(16, "0123456789ABCDEF");
      if(l_bio.read(14) == 14) {
          auto l_char_0 = l_bio.get_char();
          if(l_char_0 != 'E') {
              return false;
          }
          auto l_char_1 = l_bio.get_char();
          if(l_char_1 != 'F') {
              return false;
          }
          if(l_bio.read(0) != 0) {
              return false;
          }
          return true;
      }
      return false;
}

bool  test_21() noexcept
{
      sio  l_sio;
      bio  l_bio(l_sio);
      l_sio.write(16, "0123456789ABCDEF");
      if(l_bio.read(14) == 14) {
          auto l_char_0 = l_bio.get_char();
          if(l_char_0 != 'E') {
              return false;
          }
          auto l_char_1 = l_bio.get_char();
          if(l_char_1 != 'F') {
              return false;
          }
          l_sio.write(4, "UVXY");
          auto l_char_2 = l_bio.get_char();
          if(l_char_2 != 'U') {
              return false;
          }
          if(l_bio.read(2) != 2) {
              return false;
          }
          auto l_char_3 = l_bio.get_char();
          if(l_char_3 != 'Y') {
              return false;
          }
          return true;
      }
      return false;
}

bool  test_22() noexcept
{
      sio  l_sio;
      bio  l_bio(l_sio);
      l_sio.write(16, "0123456789ABCDEF");
      if(l_bio.read(14) == 14) {
          auto l_char_0 = l_bio.get_char();
          if(l_char_0 != 'E') {
              return false;
          }
          auto l_char_1 = l_bio.get_char();
          if(l_char_1 != 'F') {
              return false;
          }
          l_sio.write(4, "UVXY");
          auto l_char_2 = l_bio.get_char();
          if(l_char_2 != 'U') {
              return false;
          }
          // NOTE: read() should return 3, if it weren't simulated
          // Finding the actual file limits would require a seek and perhaps defeating the reason for even having
          // the read optimisation
          if(l_bio.read(64) < 3) {
              return false;
          }
          auto l_char_3 = l_bio.get_char();
          if(l_char_3 != -1) {
              return false;
          }
          return true;
      }
      return false;
}

bool  test_31() noexcept
{
      sio  l_sio;
      bio  l_bio(l_sio);
      std::size_t l_memory_baseline;

      l_bio.reserve(5);
      l_memory_baseline = l_bio.get_capacity();
      if(l_memory_baseline == 0) {
          return false;
      }

      for(int x = 0; x < 4096; x++) {
          if((x & 15) != 0) {
              std::int32_t  l_size;
              char*         l_line = l_bio.get_line(l_size);
              if((x & 15) == 1) {
                  if(l_size != 4) {
                      return false;
                  }
                  if(strncmp(l_line, "ohai", 5)) {
                      return false;
                  }
                  printf(".");
              } else
              if(l_size >= 0) {
                  return false;
              }
          } else
          if(l_sio.write(5, "ohai\n") != 5) {
              return false;
          }
      }
      if(l_bio.get_capacity() > l_memory_baseline) {
          return false;
      }
      printf("\n");
      return true;
}

bool  test_32() noexcept
{
      sio  l_sio;
      bio  l_bio(l_sio);
      l_sio.write(16, "0123456789ABCDEF");
      if(l_bio.read(8) == 8) {
          auto l_char_0 = l_bio.get_char();
          if(l_char_0 != '8') {
              return false;
          }
          l_sio.write(8, "YXWVUTSR");
          l_bio.lock();
          l_bio.read(11);
          l_bio.unlock();
          auto l_char_2 = l_bio.get_char();
          if(l_char_2 != 'U') {
              return false;
          }
          return true;
      }
      return false;
}

bool  make_server_socket(int& desc, const char* location) noexcept
{
      // open an UNIX socket
      int  l_socket = socket(AF_UNIX, SOCK_STREAM, 0);
      // setup the socket
      if(l_socket >= 0) {
          struct sockaddr_un l_address;
          // setup socket address structure
          l_address.sun_family = AF_UNIX;
          if(location) {
              if(location[0]) {
                  std::strncpy(l_address.sun_path, location, sizeof(l_address.sun_path) - 1);
              }
          }
          // enable the socket
          if(bind(l_socket, reinterpret_cast<struct sockaddr*>(std::addressof(l_address)), sizeof(struct sockaddr_un)) == 0) {
              if(listen(l_socket, 16) == 0) {
                  // set the socket to nonblocking mode
                  if(int l_flags = fcntl(l_socket, F_GETFL, 0); l_flags >= 0) {
                      if(int l_fnctl = fcntl(l_socket, F_SETFL, l_flags | O_NONBLOCK); l_fnctl >= 0) {
                          desc = l_socket;
                          return true;
                      }
                  }
              }
          }
          close(l_socket);
      }
      return false;
}

bool  make_client_socket(int& desc, const char* location) noexcept
{
      int  l_socket = socket(AF_UNIX, SOCK_STREAM, 0);
      // setup the socket
      if(l_socket >= 0) {
          struct sockaddr_un l_address;
          // setup socket address structure
          l_address.sun_family = AF_UNIX;
          if(location) {
              if(location[0]) {
                  std::strncpy(l_address.sun_path, location, sizeof(l_address.sun_path) - 1);
              }
          }
          // connect the socket
          if(connect(l_socket, reinterpret_cast<struct sockaddr*>(std::addressof(l_address)), sizeof(struct sockaddr_un)) == 0) {
              desc = l_socket;
              return true;
          }
          close(l_socket);
      }
      return false;
}

bool  free_socket(int& desc) noexcept
{
      close(desc);
      return true;
}

bool  test_91() noexcept
{
      int     l_sd;
      int     l_cd;
      int     l_rc;
      pollfd  l_pfd[8];
      char    l_data[2048];
      bool    l_result = true;

      unlink("ios.socket");
      if(make_server_socket(l_sd, "ios.socket") == false) {
          return false;
      }
      if(make_client_socket(l_cd, "ios.socket") == false) {
          return false;
      }

      l_pfd[0].fd = STDIN_FILENO;
      l_pfd[0].events = POLLIN;
      l_pfd[1].fd = l_cd;
      l_pfd[1].events = POLLIN | POLLHUP;
      l_pfd[2].fd = l_sd;
      l_pfd[2].events = POLLIN;
      l_pfd[3].fd = -1;
      l_pfd[3].events = POLLIN;

      for(int x = 0; x != sizeof(l_data); x++) {
          l_data[x] = 'a';
      }
      fflush(stdout);
      fflush(stderr);

      // testing zone
      if(true) {
          sys::asio l_stdin(STDIN_FILENO);  // stdin stream
          sys::asio l_cdio(l_cd);           // client stream
          l_cdio.set_blocking(false);
          sys::asio l_sdio(l_sd);           // server stream
          sys::asio l_gdio;                 // guest stream
          do {
              l_rc = poll(l_pfd, 4, 100);
              if(l_rc > 0) {
                  if(l_pfd[0].revents & POLLIN) {
                  }
                  if(l_pfd[1].revents & POLLIN) {
                      std::int32_t l_size;
                      char*        l_line;
                      do {
                          l_line = l_cdio.get_line(l_size);
                          if(l_line) {
                              printf("%s\n", l_line);
                          }
                      }
                      while(l_size >= 0);
                  }
                  if(l_pfd[2].revents & POLLIN) {
                      if(int l_guest_fd = accept(l_sd, nullptr, nullptr); l_guest_fd > 2) {
                          l_gdio = std::move(sys::asio(l_guest_fd));
                          l_gdio.set_blocking(false);
                          l_pfd[3].fd = l_guest_fd;
                          l_cdio.write(sizeof(l_data), l_data);
                          l_cdio.flush();
                      }
                  }
                  if(l_pfd[3].revents & POLLIN) {
                      if(std::strncmp(l_data, l_gdio.get_data(), sizeof(l_data))) {
                          l_result = false;
                      }
                      break;
                  }
              }
          }
          while(l_rc >= 0);
      }

      free_socket(l_cd);
      free_socket(l_sd);
      return l_result;
}

bool  test_99() noexcept
{
      int     l_sd;
      int     l_cd;
      int     l_rc;
      pollfd  l_pfd[8];

      unlink("ios.socket");
      if(make_server_socket(l_sd, "ios.socket") == false) {
          return false;
      }
      if(make_client_socket(l_cd, "ios.socket") == false) {
          return false;
      }

      l_pfd[0].fd = STDIN_FILENO;
      l_pfd[0].events = POLLIN;
      l_pfd[1].fd = l_cd;
      l_pfd[1].events = POLLIN | POLLHUP;
      l_pfd[2].fd = l_sd;
      l_pfd[2].events = POLLIN;
      l_pfd[3].fd = -1;
      l_pfd[3].events = POLLIN;

      printf("\n");
      fflush(stdout);
      fflush(stderr);

      // testing zone
      if(true) {
          sys::asio l_stdin(STDIN_FILENO);  // stdin stream
          sys::asio l_cdio(l_cd);           // client stream
          l_cdio.set_blocking(false);
          sys::asio l_sdio(l_sd);           // server stream
          sys::asio l_gdio;                 // guest stream
          do {
              l_rc = poll(l_pfd, 4, 100);
              if(l_rc > 0) {
                  if(l_pfd[0].revents & POLLIN) {
                      char* l_line = l_stdin.get_line();
                      if(l_line) {
                          if(l_gdio) {
                              l_cdio.put("/", l_line, "\n");
                          }
                      }
                  }
                  if(l_pfd[1].revents & POLLIN) {
                      std::int32_t l_size;
                      char*        l_line;
                      do {
                          l_line = l_cdio.get_line(l_size);
                          if(l_line) {
                              printf("%s\n", l_line);
                          }
                      }
                      while(l_size >= 0);
                  }
                  if(l_pfd[2].revents & POLLIN) {
                      if(int l_guest_fd = accept(l_sd, nullptr, nullptr); l_guest_fd > 2) {
                          l_gdio = std::move(sys::asio(l_guest_fd));
                          l_gdio.set_blocking(false);
                          l_pfd[3].fd = l_guest_fd;
                      }
                  }
                  if(l_pfd[3].revents & POLLIN) {
                      std::int32_t l_size;
                      char*        l_line;
                      do {
                          l_line = l_gdio.get_line(l_size);
                          if(l_line) {
                              printf("--- %s\n", l_line);
                          }
                      }
                      while(l_size >= 0);
                  }
              }
          }
          while(l_rc >= 0);
      }

      free_socket(l_cd);
      free_socket(l_sd);
      return true;
}

int   main(int, char**)
{
      test::scenario<basic> t01(test_01, "[01] sys::sio empty get_char()");
      test::scenario<basic> t02(test_02, "[02] sys::sio empty read(0)");
      test::scenario<basic> t03(test_03, "[03] sys::sio empty read(256)");
      test::scenario<basic> t04(test_04, "[04] sys::sio empty read(256) with reserve()");
      test::scenario<basic> t05(test_05, "[05] sys::sio read(256) with load() of 11 characters");
      test::scenario<basic> t06(test_06, "[06] sys::sio read(256) with load() of 11 characters with buffer compare");
      test::scenario<basic> t07(test_07, "[07]");
      test::scenario<basic> t08(test_08, "[08]");
      test::scenario<basic> t09(test_09, "[09]");

      test::scenario<basic> t11(test_11, "[11] sys::bio empty get_char()");
      test::scenario<basic> t12(test_12, "[12] sys::bio empty read(0)");
      test::scenario<basic> t13(test_13, "[13] sys::bio empty read(256)");
      test::scenario<basic> t14(test_14, "[14] sys::bio empty read(256) with reserve()");
      test::scenario<basic> t15(test_15, "[15] sys::bio optimised read");
      test::scenario<basic> t16(test_16, "[16] sys::bio test get_char()");
      test::scenario<basic> t17(test_17, "[17] sys::bio test get_char() past EOF");
      test::scenario<basic> t18(test_18, "[18] sys::bio test get_char() at EOF, after refill");
      test::scenario<basic> t19(test_19, "[19] sys::bio test get_char() after seek()");
      test::scenario<basic> t20(test_20, "[20] sys::bio test read() of size 0");
      test::scenario<basic> t21(test_21, "[21] sys::bio test get_char() after read()");
      test::scenario<basic> t22(test_22, "[22] sys::bio test short read()");

      test::scenario<basic> t31(test_31, "[31]");
      test::scenario<basic> t32(test_32, "[32]");

      test::scenario<basic> t91(test_91, "[91]");

      return test::run_all();
}
