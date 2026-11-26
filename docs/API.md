# fltk-toolbar API and examples

[README](../README.md) ·
[Source](https://github.com/hdijkema/fltk-toolbar)

The library arranges ordinary FLTK buttons horizontally and renders SVG markup
into owned normal/disabled button images. Styling, callbacks and command state
remain in the application. Namespace: `fltk_toolbar`.

## Headers and linking

Include `<fltk_toolbar.h>` for `Toolbar` and `<fltk_toolbar_svg.h>` for
`SvgButton`. This CMake file builds the complete example below:

```cmake
cmake_minimum_required(VERSION 3.21)
project(toolbar-example LANGUAGES CXX)
find_package(fltk-toolbar 0.1 CONFIG REQUIRED)
add_executable(example main.cpp)
target_compile_features(example PRIVATE cxx_std_17)
target_link_libraries(example PRIVATE fltk-toolbar::fltk-toolbar)
```

SVG support is part of the same target, using FLTK images. No separate SVG
package or target is required.

## Toolbar

Declared in [fltk_toolbar.h](../include/fltk_toolbar.h).
`Toolbar` derives from `Fl_Group`.

| Public declaration | Preconditions and result |
| --- | --- |
| `Toolbar(int x, int y, int w, int h)` | Create a horizontal FLTK group at the supplied geometry. |
| `void add_button(Fl_Button& button)` | Adopt a live heap button and lay out the toolbar. The toolbar owns it through FLTK parenting; retain only a borrowed pointer/reference. |
| `void separator()` | Insert an owned separator after the current buttons. |
| `void layout()` | Reposition children after a visibility or width change. Button widths are retained; heights follow the toolbar. |
| `void resize(int x, int y, int w, int h) override` | Resize the group and relayout its children. |

Registered buttons keep their native `callback`, `shortcut`, `tooltip`, toggle
state and activation behavior. Call `layout()` after `hide()`, `show()` or a
button width change. Contents are clipped; there is no overflow menu or vertical
layout. Do not register stack buttons or independently delete adopted buttons.

## SvgButton

Declared in [fltk_toolbar_svg.h](../include/fltk_toolbar_svg.h).
`SvgButton` derives from `Fl_Button` and can also be used outside a toolbar.

| Declaration | Preconditions and result |
| --- | --- |
| `SvgButton(int x, int y, int w, int h, const char* label = nullptr)` | Create an ordinary button with optional label. |
| `~SvgButton() override` | Release the images owned by the button. |
| `void svg(const std::string& markup, int size = 20)` | On the GUI thread, render SVG **text**, not a filename. Logical size must be 1–256 pixels. Invalid size/image throws; a failed replacement retains the previous usable images. |
| `void draw() override` (protected) | Draw the native button and its owned image; subclasses may override it. |

The button rasterizes SVG into owned pixels, prepares normal and disabled
composites against its background, and uses opaque RGB drawing. Background
changes rebuild the composites; display scale changes rerender the SVG. SVG
sanitization is not part of this library.

## Complete example: a command and a toggle

Save as `main.cpp`. The callback updates the status label; the toggle remains a
native FLTK toggle button. All registered buttons are heap objects owned by the
toolbar, which is owned by the window.

```cpp
#include <FL/Fl.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Double_Window.H>
#include <fltk_toolbar.h>
#include <fltk_toolbar_svg.h>

// Report a command through the status widget supplied as callback data.
void run_command(Fl_Widget*, void* data)
{
    auto* status = static_cast<Fl_Box*>(data);
    status->copy_label("Command executed");
    status->redraw();
}

int main()
{
    Fl_Double_Window window(640, 220, "Toolbar example");
    auto* toolbar = new fltk_toolbar::Toolbar(10, 10, 620, 38);
    auto* run = new fltk_toolbar::SvgButton(0, 0, 38, 38);
    run->svg(R"(<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 24 24">
        <path fill="#007d73" d="M6 3 L21 12 L6 21 Z"/>
    </svg>)");
    run->tooltip("Run command");
    run->shortcut(FL_F + 5);
    toolbar->add_button(*run);
    toolbar->separator();
    auto* pin = new Fl_Button(0, 0, 90, 38, "Pinned");
    pin->type(FL_TOGGLE_BUTTON);
    toolbar->add_button(*pin);
    toolbar->end();

    auto* status = new Fl_Box(10, 70, 620, 100, "Ready");
    run->callback(run_command, status);
    window.end();
    window.resizable(status);
    window.show();
    return Fl::run();
}
```

## CSS integration

With fltk-css linked, include `<fltk_css_widgets.h>` and construct
`Fl_Css_Widget<fltk_toolbar::SvgButton>(css, x, y, w, h)`.
Set `css_type("button")` and `css_classes({"primary"})` to select stylesheet
rules. Register it with `add_button` as usual. The stylesheet must outlive the
window and all its buttons. See the [CSS API](https://github.com/hdijkema/fltk-css/blob/main/docs/API.md).

## Library build options

`FLTK_TOOLBAR_BUILD_EXAMPLE` and `FLTK_TOOLBAR_BUILD_TESTS` default to ON for a
standalone build and OFF when embedded. An installed FLTK 1.4.5 package is used
when found; otherwise the library fetches the `release-1.4.5` sources.

## Building the example

Save the complete C++ program as `main.cpp` and the CMake block as
`CMakeLists.txt` in an empty example directory. Set `CMAKE_PREFIX_PATH` to your
installed library and FLTK prefixes, for example:

```sh
cmake -S . -B build -DCMAKE_PREFIX_PATH="C:/sdk/widgets;C:/sdk/fltk"
cmake --build build --config Release
```

Use your actual installation paths and a Visual Studio developer terminal on
Windows. Match architecture, runtime and Release/Debug configuration. See the
[README](../README.md) for library build and installation instructions.

This reference describes the public headers in this repository. It covers the
library's declared API; inherited FLTK methods and private implementation helpers
are outside its scope. Keep it aligned with header changes. Windows is the local
validation target; this reference does not establish macOS/Linux runtime or
clean-machine distribution validation.
