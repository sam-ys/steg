/* cipher.hpp -- v1.0 -- datatype that includes the handle to the cipher and the cipher length
   Author: Sam Y. 2021 */

#ifndef _CIPHER_HPP
#define _CIPHER_HPP

namespace steg {
    /// @class cipher
    struct cipher {
        // Handle to cipher
        void* hd;
        // Digest length
        std::size_t length;
    };
}

#endif
