/* cipher_ctl.hpp -- v1.0 -- used for libgcrypt cipher generation
   Author: Sam Y. 2021 */

#ifndef _CIPHER_CTL_HPP
#define _CIPHER_CTL_HPP

namespace steg {
    // Fwd. decl.
    struct cipher;

    /// Closes cipher and deallocates memory
    /// @param cipher    class
    void cipher_close(cipher& cipher);

    /// Factory method, used for cipher initialization
    /// @param key        AES key string
    /// @param initvec    initialization vector string
    /// @return           on success, returns a non-null pointer to an initialized cipher
    cipher* cipher_init(const char* const key, const char* const initvec);
}

#endif
