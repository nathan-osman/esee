## esee

esee provides a UI for editing the EXIF data in JPG files.

### Screenshot

Here's a screenshot of esee in action on Linux:

![esee window](https://i.stack.imgur.com/wgbTz.png)

### Building

To build esee, ensure that the following are installed:

- C++ compiler with C++11 support
- CMake 3.2+
- Qt 5.4+
- libexif

Open a terminal and run the following commands:

    mkdir build
    cd build
    cmake ..
    make install

If all goes well, you should be able to run `esee <filename>` to edit EXIF data.
