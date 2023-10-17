/* block_encoder.hpp -- v1.0 -- facade for encoding message and writing result to output
   Author: Sam Y. 2021 */

#ifndef _BLOCK_ENCODER_HPP
#define _BLOCK_ENCODER_HPP

#include <cmath>
#include <utility>
#include <type_traits>

#include "base64.hpp"
#include "cipher.hpp"
#include "cipher_ctl.hpp"

#include "decoder.hpp"
#include "encoder.hpp"

namespace steg {
    // @class
    template <bool b64,
              typename Talloc>
    class block_encoder : private encoder {
    public:

        /// Factory method, returns a block_encoder
        /// @param key        AES key string
        /// @param initvec    initialization vector string
        inline static block_encoder* create(const char* const key, const char* const initvec);

        /// @dtor.
        block_encoder() : encoder(nullptr) {  }

        /// Encrypts input message and writes resulting image to output
        /// @param inp    input stream
        /// @param out    output stream
        /// @return       boolean flag indicating success or failure
        template <typename Tinp,
                  typename Tout>
        inline bool run(Tinp& inp, Tout& out);

    private:

        /*! Helper
         * Padds the buffer size to a multiple of the digest length
         */
        inline std::size_t calc_digest_size(std::size_t size) const {

            std::size_t mod;
            if ((mod = (size % (encoder::get())->length)) == 0) {
                return size;
            }

            else {
                return (size + (encoder::get()->length - mod));
            }
        }

        /* Helper
         * Generates a digest from the message
         */
        template <typename Tout,
                  bool vvb64 = b64>
        inline bool encode_digest(Tout& out,
                                  char* const buff,
                                  typename std::enable_if<!vvb64, std::size_t>::type size) {

            // Encode raw data to digest
            if (encoder::encode(buff, size))
            {
                // Pipe to output and return 
                if (out.write(buff, size)) {
                    return true;
                }
            }

            return false;
        }

        /* Helper
         * Generates a digest from the message and encodes the result to base64
         */
        template <typename Tout,
                  bool vvb64 = b64>
        inline bool encode_digest(Tout& out,
                                  char* const buff,
                                  typename std::enable_if<vvb64, std::size_t>::type size) {
            // Encode raw data to digest
            if (!encoder::encode(buff, size)) {
                return false;
            }

            // Encode digest to base64...
            // Calculate base64 size
            std::size_t b64size;

            if (size % 3) {
                b64size = (size + 3 - (size % 3)) * 4/3;
            }

            else {
                b64size = size * 4/3;
            }

            // Encode to base64
            char* const b64buff = Talloc::allocate(b64size);
            base64_encode(buff, size, b64buff);

            // Pipe to output
            out.write(b64buff, b64size);

            // Cleanup & return
            return (Talloc::deallocate(b64buff), true);
        }

        /*! ctor. Private, use factory method create() instead
         */
        inline explicit block_encoder(cipher*&& cph) : encoder(cph) {  }
    };

    /*! Factory method
     */
    template <bool b64,
              typename Talloc>
    block_encoder<b64, Talloc>* block_encoder<b64, Talloc>::create(const char* const key,
                                                                   const char* const initvec)
    {
        // Use gcrypt to initialize cipher before passing it to the encoder
        cipher* cph;
        if ((cph = cipher_init(key, initvec)) == nullptr)
            return nullptr;
        return new block_encoder(std::move(cph));
    }

    /*! Encrypts input message and writes resulting image to output
     */
    template <bool b64,
              typename Talloc>
    template <typename Tinp,
              typename Tout>
    bool block_encoder<b64, Talloc>::run(Tinp& inp, Tout& out)
    {
        // Input read size
        std::size_t inpSize = inp.size();
        if (inpSize == 0) {
            inpSize = 100000; // Default size for unknown size input
        }

        // padded to a multiple of the block size
        char* const buff = Talloc::allocate(calc_digest_size(inpSize) + 1);

        // Run...
        bool ret = false;

        std::size_t size;
        if ((size = inp.read(buff, inpSize)) != 0) {
            ret = encode_digest(out, buff, calc_digest_size(size));
        }

        // Clean up & return
        return (Talloc::deallocate(buff), ret);
    }
}

#endif
