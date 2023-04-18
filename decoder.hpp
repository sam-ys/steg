/* decoder.hpp -- v1.0 -- libgcrypt message decoding
   Author: Sam Y. 2021 */

#ifndef _DECODER_HPP
#define _DECODER_HPP

namespace steg {
    // Fwd. decl.
    struct cipher;

    /// @class    decoder
    /// base class
    class decoder {
    public:

        /// dtor.
        virtual ~decoder();

        /// ctor.
        /// @param cph    the cipher implementation
        inline explicit decoder(cipher* cph) : cph_(cph) {  }

        /// Decodes data in-place
        /// @param data    input buffer, padded to a multiple of the cipher block length
        /// @param size    size of input buffer
        /// @return        true on succss, false otherwise
        bool decode(char* const data, const std::size_t size);

        /// Decodes data
        /// @param data       input buffer, padded to a multiple of the cipher block length
        /// @param size       size of input buffer
        /// @param out        output buffer, will contain result of decoding operation
        /// @param outSize    size of output buffer
        /// @return           true on succss, false otherwise
        bool decode(const char* const data, const std::size_t size, char* const out, const std::size_t outSize);

        /// @return    encapsulated cipher
        inline cipher* get() {
            return cph_;
        }

        /// @return    encapsulated cipher
        inline const cipher* get() const {
            return cph_;
        }

    private:

        cipher* cph_; // > the cipher implementation
    };
}

#endif
