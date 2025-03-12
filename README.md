A fractal renderer written in C++ with OpenGL.

Currently supports: DragonCurve (up to order 20), Sierpinski triangle (generated with random numbers).

# Examples

![Dragon Curve](https://github.com/wanderingaura/fractal_gl/blob/main/example_images/dragon.png?raw=true)
![Sierpinski Triangle (using RNG)](https://github.com/wanderingaura/fractal_gl/blob/main/example_images/rsierpinski.png?raw=true)

# Build

Dependencies: OpenGL, CMake.

Make sure the above dependencies are installed. There may be other dependencies on Windows e.g. MSVC.

The project can be built simply by executing the following commands in the project root directory.

```cmake
mkdir build
cd build
cmake ..
make
```

If the build was successful, then there should be an executable `fractals` in the cwd.

# Usage

`fractals {fractal_name} {N}`

Supported names:
- dragon: Dragon curve. N is the order of the curve.
- rsierpinski: Sierpinski triangle (randomly generated). N is the number of points sampled.
