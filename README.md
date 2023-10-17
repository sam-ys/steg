# steg
A command line steganography program

Steganography is a technique for hiding data inside other data. This program
encodes an encrypted payload (using a 128-bit AES block cipher) into a source
image by modifying each pixel to include the payload's binary form.

The modification is slight, such that the resulting image looks the same to the
unaided human eye. In principle, any type of file can be encoded provided that
the source image has enough pixels to encapsulate every bit.

This program is a command line utility that runs on Linux and uses cmake for the
build process.


Usage
--------------------------------------------------------------------------------
The easiest way to run this program is by modifying the included bash scripts
'encode.sh' and 'decode.sh' using your own parameters.

<strong>IMPORTANT</strong>: users must provide their own AES key and initialization vectors in
separate files.


<pre>
Usage: steg {--encode|--decode|-h}
              -f&lt;encoded-image-source&gt;
             [-o&lt;output-file&gt;]
             [-t&lt;output-file-type&gt;]
              -k&lt;crypt-key-file&gt;
              -v&lt;init-vec-file&gt;
             [-i&lt;message-file&gt;]
             [-b]

  --encode                     Encoding mode
  --decode                     Decoding mode
  --help (-h)                  Prints this message
</pre>

Encode Mode
--------------------------------------------------------------------------------
<pre>
  -f&lt;image-source&gt;             Source file for image that the message will be encoded to
  -o&lt;output-file&gt;              Outputs to this file
  -t&lt;output-file-type&gt;         Accepted types: png, bmp, or tga

  -k&lt;crypt-key-file&gt;           AES cryptographic key file
  -v&lt;init-vec-file&gt;            Initialization vector file

  -i&lt;message-file&gt;             Source file of message; if left unspecified, source is the terminal (stdin)
  -b                           Encodes the encrypted output as a base64 string
</pre>

Decode Mode
--------------------------------------------------------------------------------
<pre>
  -f&lt;encoded-image&gt;            Source file of encoded message
  -o&lt;output-file&gt;              Outputs to this filel if left unspecified, outpts to the terminal (stdout)

  -k&lt;crypt-key-file&gt;           AES cryptographic key file

  -v&lt;init-vec-file&gt;            Initialization vector file
  -b                           Required if the encryption output was a base64 string
</pre>

Build
--------------------------------------------------------------------------------
<pre>
cd steg
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE={release | debug} ..
make
make install
</pre>

The final command should output the utility to the local folder bin. For the
fourth line, either select the release or the debug build.


Sources and acknowledgements
--------------------------------------------------------------------------------
Steganography\
<https://wikipedia.org/wiki/Steganography>

This software uses the stb library for loading and saving images.\
<https://github.com/nothings/stb>

This software uses libgcrypt to perform encryption.\
<https://github.com/gpg/libgcrypt>

AES encryption\
<https://wikipedia.org/wiki/Advanced_Encryption_Standard>

Initialization vector\
<https://wikipedia.org/wiki/Initialization_vector>


--------------------------------------------------------------------------------
This software is entirely in the public domain and is provided as is, without
restricitions. See the LICENSE for more information.

No warranty implied; use at your own risk.

Copyright (c) 2021, Sam Y.
No Rights Reserved.
