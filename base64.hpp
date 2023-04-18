/* base64.hpp -- v1.0 -- base64 encoding & decoding
   Author: Sam Y. 2021 */

#ifndef _BASE64_HPP
#define _BASE64_HPP

namespace steg {
    /// Encodes string to base64
    /// @param value    padded input string [in]
    /// @param size     data size [in]
    /// @param output   output string, padded to necessary base64 expansion [out]
    void base64_encode(const char* const value, const std::size_t size, char* output);

    /// Decodes string (in-place) from base64
    /// @param value    input string [in/out]
    /// @param size     data size [in]
    /// @return         number of bytes decoded
    std::size_t base64_decode(char* const value, const std::size_t size);
}

#endif
