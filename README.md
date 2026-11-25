# fltk-toolbar

A small C++17 toolbar for FLTK 1.4.5. One library provides ordinary tool buttons,
separators, horizontal placement and SVG buttons with owned normal/disabled
images. Native callbacks, shortcuts, toggle buttons and tooltips keep working.
Styling and persistence belong to the application.

```cpp
#include <fltk_toolbar.h>
#include <fltk_toolbar_svg.h>

auto* toolbar = new fltk_toolbar::Toolbar(8, 34, 800, 34);
auto* button = new fltk_toolbar::SvgButton(0, 0, 34, 34);
button->svg(svg_markup); // SVG text, not a filename
button->tooltip("Run query");
button->callback(run_query, application);
toolbar->add_button(*button);
toolbar->separator();
```

The toolbar takes ownership of registered heap buttons through FLTK parenting.
Do not register stack objects or independently delete registered buttons. Button
widths are retained; toolbar resizing adjusts their positions and heights.
After hiding/showing a button or changing its width, call `layout()`.

`SvgButton` owns its active and disabled images. `svg()` renders a replacement
before discarding the previous one. It accepts SVG markup and a logical icon
size from 1 to 256 pixels, throwing on failure. Trusted application SVGs can be
embedded at compile time. SVG sanitization is outside this library's scope.

SVG support is part of `fltk-toolbar`, using FLTK's image library. There is no
separate SVG target. CSS is optional application integration: for example,
`Fl_Css_Widget<fltk_toolbar::SvgButton>` combines it with fltk-css.

## Build and use

```sh
cmake -S . -B build/release -DCMAKE_BUILD_TYPE=Release -DFLTK_DIR=/path/to/FLTK/CMake
cmake --build build/release --config Release
ctest --test-dir build/release -C Release --output-on-failure
cmake --install build/release --config Release --prefix /path/to/install
```

If no FLTK package is installed, CMake fetches the named `release-1.4.5` tag.
An installed consumer uses:

```cmake
find_package(fltk-toolbar 0.1 CONFIG REQUIRED)
target_link_libraries(myapp PRIVATE fltk-toolbar::fltk-toolbar)
```

`FLTK_TOOLBAR_BUILD_TESTS` and `FLTK_TOOLBAR_BUILD_EXAMPLE` default to ON for
standalone builds and OFF when embedded. The example has text and SVG buttons.

## Scope

The first version is horizontal, with clipped contents and no overflow menu,
customization dialog, draggable rows or command registry. Windows/MSVC Release
contracts are validated locally; macOS/Linux require their hosted builds.
See [development notes](docs/DEVELOPMENT.md). Licensed under LGPL 2.1; see [LICENSE](LICENSE).
