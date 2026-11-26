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

## API documentation

`docs/API.md` documents this repository's public headers, ownership rules and
CMake integration, with a complete C++ example. Update it when the public API
changes. The Penthario website links here rather than maintaining a second copy.

## Library website

`docs/index.html` is the standalone landing page. `docs/assets/logo.svg` is the
library's vector logo and favicon; `docs/style.css` provides responsive layout
and explicit light/dark themes. All assets and fonts are local or system-provided;
the only script is the small theme control; the library itself has no website dependency.

GitHub Pages can publish the default branch's `/docs` directory. Jekyll uses
`docs/_config.yml`, `jekyll-optional-front-matter` and the documentation layout
to render `API.md` and this guide as HTML. Repository-relative source links in
the Markdown are mapped to GitHub by the layout. Documentation stays owned by
this repository. The landing illustration is an HTML/CSS diagram, not a product
screenshot. The logo uses the same rounded-square geometry as the other widget
libraries, with a distinct symbol and accent color.

To build locally with Jekyll 3.10 and `kramdown-parser-gfm` installed:

```sh
jekyll build --source docs --destination build/site/fltk-toolbar
python -m http.server 8000 --directory build/site
```

Open `http://localhost:8000/fltk-toolbar/`. The destination includes the configured
repository base path so API-page assets resolve correctly. Publishing requires the repository's GitHub Pages setting;
adding these source files does not change that setting or publish the site.

## Website theme control

The homepage and documentation layout start with `data-theme="dark"`.
`theme.js` restores a saved light/dark choice before the body paints and attaches
the keyboard-accessible header button. The preference uses the site's own
`data-theme-key` in local storage; storage failures do not prevent switching.
The default is dark even when the operating system prefers light. Without
JavaScript, the pages retain their default dark appearance.
