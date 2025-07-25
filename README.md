# Image Formatter

A simple command-line utility written in C++ for performing basic image manipulations. This tool can read an image file, apply a series of transformations in sequence, and save the result as a new PNG file.

## Features

* Load various image formats (PNG, JPG, BMP, etc.).
* Apply multiple operations in a single command.
* Save the output image in PNG format.
* Supported operations:
    * Darken
    * Invert colors
    * Reflect horizontally (flip vertically)
    * Reflect vertically (flip horizontally)
    * Rotate by $n$ degrees, where $n = 90 \cdot k,\; k \in \mathbb{Z}$

## Dependencies

The project is self-contained and includes the necessary single-header libraries:

* [cxxopts](https://github.com/jarro2783/cxxopts) for command-line argument parsing.
* [stb_image](https://github.com/nothings/stb/blob/master/stb_image.h) for loading images.
* [stb_image_write](https://github.com/nothings/stb/blob/master/stb_image_write.h) for saving images.

## Building

To build the project, you need a C++ compiler that supports at least C++11 (C++17 is recommended). Navigate to the project's root directory and run the following command. This command assumes you have placed `image.hpp` and `cxxopts.hpp` inside a `headers/` subdirectory.

```bash
g++ main.cpp image.cpp -o ImageFormatter -std=c++17 -I.
```

If your header files are in the same directory as the `.cpp` files, you will need to adjust the include paths in `main.cpp` and `image.cpp` from `headers/...` to `...` and then compile with:

```bash
g++ main.cpp image.cpp -o ImageFormatter -std=c++17
```

## Usage

The program is operated via the command line with the following syntax:

```bash
./ImageFormatter -i <input_file> -o <output_file> -- [operation1 [params]] [operation2 [params]] ...
```

### Arguments
- `-i`, `--input`: (Required) Path to the input image file.
- `-o`, `--output`: (Required) Path for the output PNG image file.
- `operations`: A sequence of one or more operations to apply to the image. These are positional arguments that follow the input/output flags.

### Note
-  `--`: is the separator between positional and non-positional arguments. Don't forget it!

### Available Operations
The following operations can be applied in the order they are provided on the command line.
- `darken <percentage>`
    - Reduces the brightness of the image. `<percentage>` is an integer from 0 to 100 representing the desired final brightness level. For example, `darken 50` makes the image half as bright.
- `invert`
    - Inverts the colors of the image (creates a photo negative effect).
- `ref-hor`
    - Reflects the image across the horizontal axis (flips it vertically).
- `ref-ver`
    - Reflects the image across the vertical axis (flips it horizontally).
- `rotate <degree>`
    - Rotates the image by `<degree>` degrees. If positive - clockwise, if negative - counterclockwise 

## Examples
1. Invert the colors of `input.jpg` and save it as inverted.png:
    `./ImageFormatter -i input.jpg -o inverted.png -- invert`
2. Darken `photo.png` to 70% brightness and then reflect it vertically:
    `./ImageFormatter -i photo.png -o processed.png -- darken 70 ref-ver`
3. Apply multiple reflections to original.bmp:
    `./ImageFormatter -i original.bmp -o reflected.png -- ref-hor ref-ver`
