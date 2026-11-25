# Development

`Toolbar` is an `Fl_Group` that reparents and owns buttons and separators.
`layout()` walks visible children in order, retaining widths and assigning
toolbar-height rectangles with four-pixel gaps. `resize()` updates the group
rectangle without stretching child widths, then repeats that layout.

`SvgButton` is an ordinary `Fl_Button`. Its `svg()` method parses markup through
`Fl_SVG_Image`, materializes an independent RGBA raster, then composites owned
opaque RGB active/disabled images before assigning them to FLTK. This avoids the
platform alpha-blending path. `draw()` updates composites when the native/CSS
button background changes and rerasterizes the source when screen scale changes.
The destructor clears FLTK's raw image pointers before its owned images die.
Keep callbacks and application command state outside the library.

All widget operations require the GUI thread. There is no event-loop, CSS or
settings dependency. Include `fltk_toolbar.h` for layout and
`fltk_toolbar_svg.h` for SVG buttons; both are supplied by one CMake target.

Contract tests cover ownership, layout after moving/resizing/hiding, callback
forwarding, visible opaque pixels, background/pressed colors, SVG ownership and
replacement failure. Memory-surface drawing requires a graphics backend (Xvfb
on Linux), but opens no windows and runs no interactive event loop. Tests use no
application files or credentials. The workflow also builds the example.
Actual visual appearance and platform interactions still need native validation.
