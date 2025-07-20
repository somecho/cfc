# Changelog

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
