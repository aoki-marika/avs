# libavs (working title)

A game engine based around working with private BEMANI file formats, namely IFS and binary XML.

This is largely a learning project for C++, OpenGL, and working with BEMANI file formats. **It is not actively maintained and is provided as is.**

Uses OpenGL with DispmanX on Raspberry Pi, and X11 on any other Linux distro. Optimised for performance on a Raspberry Pi 1B.

<sub>Yes, it is named after the wrong framework.</sub>

# Requirements

- Universal
  - `libicu-dev` (`57.1`)
  - `libssl-dev` (`1.1.0`)
  - `libfreetype6-dev` (`2.6.3`)
- Linux (X11, not Raspberry Pi)
  - `libgles2-mesa-dev` (`13.0.6`)
