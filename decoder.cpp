/* decoder.cpp -- v1.0 -- libgcrypt message decoding
   Author: Sam Y. 2021 */

#include <cstddef>
#include <gcrypt.h>

#include "cipher.hpp"
#include "cipher_ctl.hpp"
#include "decoder.hpp"
#include "error.hpp"

/*! dtor.
 */
steg::decoder::~decoder()
{
    if (cph_)
    {
        cipher_close(*cph_);
        delete cph_;
    }
}

/*! Decodes data in-place
 */
bool steg::decoder::decode(char* const data, const std::size_t size)
{
    cipher& cph = *cph_;
    // Do an in-place decryption
    std::size_t ret;
    if ((ret = gcry_cipher_decrypt(reinterpret_cast<gcry_cipher_hd_t>(cph.hd), data, size, nullptr, 0)) == 0) {
        return true;
    }

    const char* const strerror = gcry_strerror(ret);
    const char* const strsource = gcry_strsource(ret);
    // Report error
    return ((error::get())->log("Error: ", strsource, ", ", strerror), false);
}

/*! Decodes data
 */
bool steg::decoder::decode(const char* const data,
                           const std::size_t size,
                           char* const out,
                           const std::size_t outSize)
{
    cipher& cph = *cph_;
    // Do an in-place decryption
    std::size_t ret;
    if ((ret = gcry_cipher_decrypt(reinterpret_cast<gcry_cipher_hd_t>(cph.hd), out, outSize, data, size)) == 0) {
        return true;
    }

    const char* const strerror = gcry_strerror(ret);
    const char* const strsource = gcry_strsource(ret);
    // Report error
    return ((error::get())->log("Error: ", strsource, ", ", strerror), false);
}
