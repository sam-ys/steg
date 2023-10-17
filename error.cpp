/* error.cpp -- v1.0 -- includes a singleton class used for logging to stderr
   Author: Sam Y. 2021 */

#include <cstdio>
#include <gcrypt.h>

#include "error.hpp"

void steg::error::print(const char* str) {
    ::fprintf(stderr, "%s", str);
}

/*! ctor.
 */
steg::error::error() {
    instance_ = nullptr;
}

/*! Pointer to instance
 */
steg::error* steg::error::instance_ = nullptr;
