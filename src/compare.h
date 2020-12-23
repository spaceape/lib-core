#ifndef core_compare_h
#define core_compare_h
/**
    Copyright (c) 2019, wicked systems

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
#include <global.h>
#include <cstring>

constexpr int compare(int lhs, int rhs) noexcept
{
    if(lhs < rhs) {
        return -1;
    } else
    if(lhs > rhs) {
        return +1;
    } else
        return  0;
}

constexpr int compare(unsigned int lhs, unsigned int rhs) noexcept
{
    if(lhs < rhs) {
        return -1;
    } else
    if(lhs > rhs) {
        return +1;
    } else
        return  0;
}

constexpr int compare(long int lhs, long int rhs) noexcept
{
    if(lhs < rhs) {
        return -1;
    } else
    if(lhs > rhs) {
        return +1;
    } else
        return  0;
}

constexpr int compare(unsigned long int lhs, unsigned long int rhs) noexcept
{
    if(lhs < rhs) {
        return -1;
    } else
    if(lhs > rhs) {
        return +1;
    } else
        return  0;
}

constexpr int compare(long long int lhs, long long int rhs) noexcept
{
    if(lhs < rhs) {
        return -1;
    } else
    if(lhs > rhs) {
        return +1;
    } else
        return  0;
}

constexpr int compare(unsigned long long int lhs, unsigned long long int rhs) noexcept
{
    if(lhs < rhs) {
        return -1;
    } else
    if(lhs > rhs) {
        return +1;
    } else
        return  0;
}

constexpr int compare(const char* lhs, const char* rhs) noexcept
{
    if(lhs && lhs[0]) {
        if(rhs && rhs[0]) {
            if(lhs == rhs) {
                return 0;
            }
            return std::strcmp(lhs, rhs);
        } else
            return lhs[0];
    } else
    if(rhs) {
        if(rhs[0]) {
            return -rhs[0];
        } else
            return 0;
    } else
        return 0;
}

template<typename Ct>
constexpr int compare(Ct lhs, Ct rhs) noexcept
{
    if(lhs < rhs) {
        return -1;
    } else
    if(lhs > rhs) {
        return +1;
    } else
        return  0;
}
#endif
