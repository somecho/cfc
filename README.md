# Soya - A Lightweight Creative Coding Framework in C

Soya is a OpenGL-based single-header library for creating 2D/3D graphics.

## Using Soya

Soya depends on the following libraries:
- [GLFW][glfw] for window management
- [Libepoxy][epoxy] for GL extension loading
- [Cglm][cglm] for vector and matrix math
- [libpng][libpng] for PNG import and export

## Running Examples
For now, if you would like to learn how to use Soya, have a look at the [examples][examples]. 

### On Linux
```sh
cd examples
./RunExample basic-drawing
```
#### Using the `RunExample` script
The script has the following structure.
```sh
./RunExample EXAMPLE_NAME OPTIONAL_ARGS
```
- `EXAMPLE_NAME` - the name of the example in the examples folder follows the `NN-EXAMPLE_NAME.C` structure, where `NN` is the number. You only need to provide `EXAMPLE_NAME`.
- `OPTIONAL_ARGS` - you can leave this out, but you can also add additional arguments like `-O3`.
- The examples are built to a directory called `bin` in the root directory.

[glfw]:https://github.com/glfw/glfw
[epoxy]:https://github.com/anholt/libepoxy
[cglm]:https://github.com/recp/cglm
[libpng]:https://github.com/pnggroup/libpng
[examples]:./examples
