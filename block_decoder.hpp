/* block_decoder.hpp -- v1.0 -- facade for decoding message and writing result to output
   Author: Sam Y. 2021 */

#ifndef _BLOCK_DECODER_HPP
#define _BLOCK_DECODER_HPP

#include "decoder.hpp"

namespace steg {
    //! @class block_decoder
    template <bool b64,
              typename Talloc>
    class block_decoder : private decoder {
    public:

        /// Factory method, returns a block_decoder
        /// @param key       AES key string
        /// @param initvec   initialization vector string
        inline static block_decoder* create(const char* const key, const char* const initvec);

        /// Decodes input message and writes to output
        /// @param inp       input stream
        /// @param out       output stream
        /// @param return    gcrypt return code
        template <typename Tinp,
                  typename Tout>
        inline bool run(Tinp& inp, Tout& out);

    private:

        /*! Helper
         */
        template <typename Tout,
                  bool vvb64 = b64>
        inline bool decode_digest(Tout& out,
                                  char* const buff,
                                  typename std::enable_if<!vvb64, std::size_t>::type size) {
            // Decodes from digest to raw data
            bool ret;
            if ((ret = decoder::decode(buff, size))) {
                ret = out.write(buff, size); // Pipe to output
            }

            return ret;
        }

        /*! Generates a digest from the message and encodes it base64
         */
        template <typename Tout,
                  bool vvb64 = b64>
        inline bool decode_digest(Tout& out,
                                  char* const buff,
                                  typename std::enable_if<vvb64, std::size_t>::type base64Size) {

            // Decode digest from base64
            std::size_t size = base64_decode(buff, base64Size);
            size = size - (size % decoder::get()->length);

            // Decode raw data from digest
            bool ret;
            if ((ret = decoder::decode(buff, size))) {
                ret = out.write(buff, size);
            }

            return ret;
        }

        /*! ctor. Private, use factory method create() instead
         */
        inline explicit block_decoder(cipher*&& cph) : decoder(cph) {  }
    };

    /// Factory method, returns block decoder
    /// @param key        AES key string
    /// @param initvec    initialization vector string
    template <bool b64,
              typename Talloc>
    block_decoder<b64, Talloc>* block_decoder<b64, Talloc>::create(const char* const key,
                                                                   const char* const initvec)
    {
        // Use gcrypt to initialize cipher before passing it to the decoder
        cipher* cph;
        if ((cph = cipher_init(key, initvec)) == nullptr)
            return nullptr;
        return new block_decoder(std::move(cph));
    }

    /// Encrypts input message and writes to output
    /// @param inp    input stream
    /// @param out    output stream
    /// @return       boolean flag indicating success or failure
    template <bool b64,
              typename Talloc>
    template <typename Tinp,
              typename Tout>
    bool block_decoder<b64, Talloc>::run(Tinp& inp, Tout& out)
    {
        // Input read size
        std::size_t inpSize = inp.size();
        if (inpSize == 0) {
            inpSize = 100000; // Default size for unknown size input
        }

        // Generate the read buffer,
        // padded to a multiple of the block size
        char* const buff = Talloc::allocate(inpSize);

        // Run...
        bool ret = false;

        std::size_t size;
        if ((size = inp.read(buff, inpSize)) != 0) {
            ret = decode_digest(out, buff, size);
        }

        // Clean up & return
        return (Talloc::deallocate(buff), ret);
    }
}

#endif
