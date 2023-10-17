/* image.cpp -- v1.0 -- used for loading, saving, reading, and writing to a source image
   Author: Sam Y. 2021 */

#include <algorithm>
#include <utility>

#include "stb.hpp"

#include "error.hpp"
#include "image.hpp"

/*! dtor.
 */
steg::image::~image()
{
    if (data_ != nullptr) {
        stbi_image_free(data_);
    }
}

/*! ctor.
 */
steg::image::image() : data_(nullptr)
                     , w_(0)
                     , h_(0)
                     , nchanns_(0) {  }

/*! ctor.
 */
steg::image::image(image&& other) : data_(other.data_)
                                  , w_(other.w_)
                                  , h_(other.h_)
                                  , nchanns_(other.nchanns_)
{
    other.data_ = nullptr;
    other.w_ = 0;
    other.h_ = 0;
    other.nchanns_ = 0;
}

/*! assignment
 */
steg::image& steg::image::operator=(image&& other)
{
    data_ = other.data_;
    w_ = other.w_;
    h_ = other.h_;
    nchanns_ = other.nchanns_;

    other.data_ = nullptr;
    other.w_ = 0;
    other.h_ = 0;
    other.nchanns_ = 0;

    return *this;
}

/*! Image size
 */
std::size_t steg::image::size() const
{
    // Width x Height x # channels
    return w_ * h_ * nchanns_;
}

/*! Save file
 */
bool steg::image::save(const char* path, const image_type type) const
{
    // Just in case
    if (!path) {
        return ((error::get())->log("Error: invalid save path"), false);
    }

    switch (type)
    {
        case PNG:
        {
            const int stride = nchanns_ * w_;
            bool ret;
            if (!(ret = stbi_write_png(path, w_, h_, nchanns_, data_, stride)))
                (error::get())->log("Error: unable to save image ", path);
            return ret;
        }

        case BMP:
        {
            bool ret;
            if (!(ret = stbi_write_bmp(path, w_, h_, nchanns_, data_)))
                (error::get())->log("Error: unable to save image ", path);
            return ret;
        }

        case TGA:
        {
            bool ret;
            if (!(ret = stbi_write_tga(path, w_, h_, nchanns_, data_)))
                (error::get())->log("Error: unable to save image ", path);
            return ret;
        }

        default: {
            return false;
        }
    }
}

/*! Load file
 */
std::size_t steg::image::open(const char* path)
{
    // Return if image already loaded
    if (data_) {
        return 0;
    }

    // Get the data
    if ((data_ = stbi_load(path, &w_, &h_, &nchanns_, 0)) == nullptr) {
        return ((error::get())->log("Error: unable to load image ", path), 0);
    }

    return (w_ * h_ * nchanns_);
}

/*! Reads message from image
 */
std::size_t steg::image::read(char* buff, const std::size_t buffSize)
{
    // Zero-out buffer
    ::memset(buff, 0, buffSize);

    // Width x height
    const std::size_t size = w_ * h_;

    // Just in case
    // Ensure that buffer is large enough
    if ((buffSize * 8) < size) {
        return ((error::get())->log("Error: output buffer is too small to accomodate message size, exiting"), 0);
    }

    // Unapply steganography
    std::size_t i = 0;
    while (i != size)
    {
        const unsigned char r = data_[(nchanns_ * i)];

        int bit = (r & 0x03);
        // Check for terminating character
        if (bit == 0x02) {
            break; // Reached end of message
        }

        // Parse bit
        *buff |= (bit << (i % 8));
        if ((++i % 8) == 0) {
            ++buff;
        }
    }

    // Terminate and return
    return ((*buff = 0), (i / 8));
}

/*! Writes message to image
 */
std::size_t steg::image::write(const char* buff, std::size_t buffSize)
{
    // Width x height
    const std::size_t size = w_ * h_;

    // Ensure that file size is large enough to hold image
    const char* ptr = buff;
    if ((buffSize * 8) > size) {
        return ((error::get())->log("Error: source image is too small to encode entire message, exiting"), 0);
    }

    // Apply stegonography
    // Insert message
    std::size_t i = 0;
    while(i != size && (static_cast<std::size_t>(ptr - buff) != buffSize))
    {
        const std::size_t j = nchanns_ *  i;

        // Encode single bit
        unsigned char& r = data_[j];
        const int bit = (static_cast<unsigned char>(*ptr >> (i % 8)) & 0x01);

        // Clear the two lower-order bits
        // and add low order bit
        r = ((r & ~0x03) | bit);
        // When done encoding all bits, advance message pointer to next byte
        if ((++i % 8) == 0) {
            ++ptr;
        }
    }

    // "Zero-out" remaining cells using 0x02 as terminating character
    for ( ; i != size; ++i)
    {
        unsigned char& r = data_[(nchanns_ * i)];
        r = ((r & ~0x03) | 0x02);
    }

    return size;
}
