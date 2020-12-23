#include "test-ios.h"
#include <sys/ios/sio.h>
#include <sys/ios/bio.h>
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
      l_sio.reserve(16384);
      for(int x = 0; x != 8; x++) {
          l_sio.write(16, "01234567ABCDEF");
      }
      if(l_bio.read(8) == 8) {
          if(l_bio.read(8) == 8) {
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
      l_sio.reserve(16384);
      for(int x = 0; x != 1024; x++) {
          l_sio.write(16, "01234567ABCDEF");
      }
      if(l_bio.seek(20, SEEK_SET) == 20) {
          if(l_bio.get_char() == '4') {
              return true;
          }
      }
      return false;
}

// test sys::bio with source and read size of 0

int   main(int, char**)
{
      test::scenario<basic> t01(test_01, "[01] sys::sio empty get_char()");
      test::scenario<basic> t02(test_02, "[02] sys::sio empty read(0)");
      test::scenario<basic> t03(test_03, "[03] sys::sio empty read(256)");
      test::scenario<basic> t04(test_04, "[04] sys::sio empty read(256) with reserve()");
      test::scenario<basic> t05(test_05, "[05] sys::sio read(256) with load() of 11 characters");
      test::scenario<basic> t06(test_06, "[06] sys::sio read(256) with load() of 11 characters with buffer compare");

      test::scenario<basic> t11(test_11, "[11] sys::bio empty get_char()");
      test::scenario<basic> t12(test_12, "[12] sys::bio empty read(0)");
      test::scenario<basic> t13(test_13, "[13] sys::bio empty read(256)");
      test::scenario<basic> t14(test_14, "[14] sys::bio empty read(256) with reserve()");
      test::scenario<basic> t15(test_15, "[15] sys::bio optimised read");
      test::scenario<basic> t16(test_16, "[16]");

      return test::run_all();
}
