#pragma once

// Disable strict warnings for this header from the Microsoft Visual C++ compiler.
// #ifdef _MSC_VER
//   #pragma warning (push, 0)
// #endif

#define STB_IMAGE_IMPLEMENTATION
#define STBI_FAILURE_USERMSG
#include "external/stb_image.h"

#include <cstdlib>
#include <iostream>

class RTWImage
{
  public:
    RTWImage() {}

    RTWImage(const char* image_filename)
    {
      // Load image data from the specified file. If the RTW_IMAGE environment variable is
      // define, looks only in that directory for the image file. if the image was not found, 
      // searches for the specified image file first from the current directory, then in the
      // image/ subdirectory, then the _parent's_ images/ subdirectory, and then _that_
      // parent, on so on, for six levels up. Ifthe image was not loaded successfully,
      // width() anf height() will return 0.

      auto filename = std::string(image_filename);
      auto image_directory = getenv("RTW_IMAGES");

      // Hunt for the image file in some likely locations.
      if (image_directory && load(std::string(image_directory) + "/" + image_filename)) return;
      if (load(filename)) return;
      if (load("images/" + filename)) return;
      if (load("../images/" + filename)) return;
      if (load("../../images/" + filename)) return;
      if (load("../../../images/" + filename)) return;
      if (load("../../../../images/" + filename)) return;
      if (load("../../../../../images/" + filename)) return;
      if (load("../../../../../../images/" + filename)) return;

      std::cerr << "ERROR: Could not load image file '" << image_filename << "'.\n";
    }

    ~RTWImage()
    {
      delete[] bdata;
      STBI_FREE(fdata);
    }

    bool load(const std::string& filename)
    {
      // Load the linear (gamma=1) image data from given file name. Returns true if the
      // load succeeded. The resulting data buffer contains the three [0.0, 1.00]
      // floating-point values for the first pixel (red, then green, then blue). Pixels are
      //contiguous, going left to right for the width of the image, followed by the next row
      // bellow, for the full height of the image.

      auto n = bytes_per_pixel; // Dummy out parameter: original component per pixel
      fdata = stbi_loadf(filename.c_str(), &image_width, &image_height, &n, bytes_per_pixel);
      if(fdata == nullptr) return false;

      bytes_per_scanline = image_width * bytes_per_pixel;
      convertToBytes();
      return true;
    }

    int width() const 
    {
      return (fdata == nullptr) ? 0 : image_width;
    }

    int height() const
    {
      return (fdata == nullptr) ? 0 : image_height;
    }

    const unsigned char* pixelData(int x, int y) const
    {
      // Return the address of the three RGB bytes of the pixel at x, y. If there is no image
      // data, returns magenta.
      static unsigned char magenta[] = { 255, 0, 255 };
      if(bdata == nullptr) return magenta;

      x = clamp(x, 0, image_width);
      y = clamp(y, 0, image_height);

      return bdata + y * bytes_per_scanline + x * bytes_per_pixel;
    }

  private:
    const int bytes_per_pixel = 3;
    float *fdata = nullptr; // Linear floating point pixel data
    unsigned char *bdata = nullptr; // Linear 8-bit pixel data
    int image_width = 0; // Loaded image width
    int image_height = 0; // Loaded image height
    int bytes_per_scanline = 0;

    static int clamp(int x, int low, int high)
    {
      // return the value clamped to the range [low, high).
      if(x < low) return low;
      if(x < high) return x;
      return high - 1;
    }

    static unsigned char floatToByte(float value)
    {
      if(value <= 0.0)
        return 0;
      if(1.0 <= value)
        return 255;
      return static_cast<unsigned char>(256.0 * value);
    }

    void convertToBytes()
    {
      // Convert the linear floating point pixel data to bytes, storing the resulting byte
      // data in the 'bdata' member.

      int total_bytes = image_width * image_height * bytes_per_pixel;
      bdata = new unsigned char[total_bytes];

      // Iterate through all pixel components, converting from [0.0, 1.0] float values to
      // unsigned [0, 255] byte values.

      auto *bptr = bdata;
      auto *fptr = fdata;
      for(auto i = 0; i < total_bytes; i++, fptr++, bptr++)
      {
        *bptr = floatToByte(*fptr);
      }
    }
};

// Restore MSVC compiler warnings
// #ifdef _MSC_VER
//   #pragma warning (pop)
// #endif