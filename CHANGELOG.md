# Changelog

# 0.3.0
- CMake
  - New option `SOYA_CORE`. When `ON`, builds entire Soya as a framework. When `OFF`, builds only the libs so that Soya can be used as a library without dependencies.
- Examples
  - [3d-drawing][3d-eg]
  - [callbacks][callbacks-eg]
  - [camera][cam-eg]
- Features
  - More callbacks: `onMousePress`, `onMouseRelease`, `onMouseMove`, `onExit`
  - New functions: `syAppDisableCursor`
  - [First person camera][first-person-cam]
  - 3D Drawing: `syCube`, `sySphere`, `syDrawCube`, `syDrawSphere`
- Improvements
  - Logging
- Breaking Changes
  - Renamed `syFboDraw` to `syDrawFbo`
  - Renamed `onKeyPressed` callback to `onKey`

[3d-eg]:./examples/3d-drawing.c
[callbacks-eg]:./examples/callbacks.c
[cam-eg]:./examples/camera.c
[first-person-cam]:./soya/core/camera.h

# 0.2.0

- added doxyfile
- fixes rgb to hsv conversion and add hsv to rgb conversion
- added [macros for writing glsl inline][sysl]
- extras
  - added [polyline][syPl]
  - added [glsl snippets][snippets]
- breaking changes
  - macro define `SY_NO_CONFIGURE` renamed to `SOYA_NO_CONFIGURE`

# 0.1.0

The first version of Soya. Current features:
- Drawing Api
  - 2D drawing API
    - `syDrawLine` 
    - `syDrawTriangle` 
    - `syDrawQuad` 
    - `syDrawPolygon` 
  - 3D drawing API
    - `syDrawUnindexed`
    - `syDrawIndexed`
  - Transformations
    - `syTranslate`
    - `syRotate`
  - Offscreen rendering with framebuffers `syFbo`
  - Custom shaders with `syBeginShader`


[syPl]:./soya/extras/polyline.h
[snippets]:./soya/extras/glslsnippets.h
[sysl]:./soya/lib/sl.h
