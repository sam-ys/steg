/* image.hpp -- v1.0 -- used for loading, saving, reading, and writing to a source image
   Author: Sam Y. 2021 */

#ifndef _IMAGE_HPP
#define _IMAGE_HPP

namespace steg {
    /// @class
    class image {
    public:

        /// Type of image file
        enum image_type { NIL, PNG, BMP, TGA };

        /// dtor.
        ~image();

        /// ctor.
        image();

        /// ctor.
        /// No copy ctor. defined, this is a non-copyable object
        explicit image(image&& other);

        /// Assignment operator
        /// No copy assignment operator defined, this is a non-copyable object
        image& operator=(image&& other);

        /// @return    the size of the message
        std::size_t size() const;

        /// Saves file
        /// @param path    output image path
        /// @param type    output image file type
        /// @return        true on success, false otherwise
        bool save(const char* path, const image_type type) const;

        /// Loads file
        /// @param path    path/to/image/file
        /// @return        image size
        std::size_t open(const char* path);

        /// Reads message from image
        /// @param buff[out]    unallocated output buffer
        /// @param buffSize     size of buff 
        /// @return             number of bytes read
        std::size_t read(char* buff, const std::size_t buffSize);

        /// Writes message to image
        /// @param buff        input message [in]
        /// @param buffSize    input message size [in]
        /// @return            number of bytes written
        std::size_t write(const char* buff, const std::size_t buffSize);

    private:

        // Non-copyable
        explicit image(image&) = delete;
        explicit image(const image&) = delete;

        // Image data
        unsigned char* data_;

        // Image width, height, no. of channels
        int w_, h_, nchanns_;
    };
}

#endif
