/* encoder.hpp -- v1.0 -- libgcrypt message encoding
   Author: Sam Y. 2021 */

#ifndef _ENCODER_HPP
#define _ENCODER_HPP

namespace steg {
    // Fwd. decl.
    struct cipher;

    /// @class: base
    class encoder {
    public:

        /// dtor.
        virtual ~encoder();

        /// ctor.
        /// @param cph    the cipher implementation
        inline explicit encoder(cipher* cph) : cph_(cph) {  }


        /// Encodes data in-place
        /// @param data    input buffer, padded to a multiple of the cipher block length
        /// @param size    size of input buffer
        /// @return        true on succss, false otherwise
        bool encode(char* const data, std::size_t size);

        /// Encodes data
        /// @param data       input buffer, padded to a multiple of the cipher block length
        /// @param size       size of input buffer
        /// @param out        output buffer, will contain result of encoding operation
        /// @param outSize    size of output buffer
        /// @return           true on succss, false otherwise
        bool encode(const char* const data, const std::size_t size, char* const out, const std::size_t outSize);

        /// @return    encapsulated cipher
        inline cipher* get() {
            return cph_;
        }

        /// @return    encapsulated cipher
        inline const cipher* get() const {
            return cph_;
        }

    private:

        // The cipher implementation
        cipher* cph_;
    };
}

#endif
