# Development

`Toolbar` is an `Fl_Group` that reparents and owns buttons and separators.
`layout()` walks visible children in order, retaining widths and assigning
toolbar-height rectangles with four-pixel gaps. `resize()` updates the group
rectangle without stretching child widths, then repeats that layout.

`SvgButton` is an ordinary `Fl_Button`. Its `svg()` method parses markup through
`Fl_SVG_Image`, prepares owned active/disabled images, then assigns them to FLTK.
The destructor clears FLTK's raw image pointers before its owned images die.
Keep callbacks and application command state outside the library.

All widget operations require the GUI thread. There is no event-loop, CSS or
settings dependency. Include `fltk_toolbar.h` for layout and
`fltk_toolbar_svg.h` for SVG buttons; both are supplied by one CMake target.

Contract tests cover ownership, layout after moving/resizing/hiding, callback
forwarding, SVG ownership and replacement failure. They open no windows and use
no application files or credentials. The workflow also builds the example.
Actual visual appearance and platform interactions still need native validation.
