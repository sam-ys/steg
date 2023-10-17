/* cipher_ctl.cpp -- v1.0 -- used for libgcrypt cipher generation and clean up
   Author: Sam Y. 2021 */

#include <gcrypt.h>

#include "cipher.hpp"
#include "cipher_ctl.hpp"
#include "error.hpp"

namespace {
    // Helper
    inline void log(const int ret) {
        (steg::error::get())->log("Error: ", gcry_strsource(ret), ", ", gcry_strerror(ret));
    }
}

/*! Initializes cipher
 */
struct steg::cipher* steg::cipher_init(const char* const key, const char* const initv)
{
    gcry_cipher_hd_t hd;
    const std::size_t keylen = gcry_cipher_get_algo_blklen(GCRY_CIPHER_AES128);
    const std::size_t blklen = gcry_cipher_get_algo_keylen(GCRY_CIPHER_AES128);

    // Create a handle for algorithm ALGO to be used in MODE.  FLAGS may
    // be given as an bitwise OR of the gcry_cipher_flags values.
    int ret;
    if ((ret = gcry_cipher_open(&hd, GCRY_CIPHER_AES128, GCRY_CIPHER_MODE_ECB, 0)) != 0) {
        return (log(ret), nullptr);
    }

    // Retrieve the key length in bytes used with algorithm A.
    // Set KEY of length KEYLEN bytes for the cipher handle HD.
    if ((ret = gcry_cipher_setkey(hd, key, keylen)) != 0)
    {
        gcry_cipher_close(hd);
        return (log(ret), nullptr);
    }

    // Retrieve the block length in bytes used with algorithm A.
    // Set initialization vector IV of length IVLEN for the cipher handle HD.
    if ((ret = gcry_cipher_setiv(hd, initv, blklen)) != 0)
    {
        gcry_cipher_close(hd);
        return (log(ret), nullptr);
    }

    // Allocate and return
    return new cipher({
            hd,
            128 // AES 128
        });
}

/*! Closes cipher and deallocates memory
 */
void steg::cipher_close(steg::cipher& cph)
{
    gcry_cipher_hd_t hd = reinterpret_cast<gcry_cipher_hd_t>(cph.hd);
    gcry_cipher_close(hd);
}

