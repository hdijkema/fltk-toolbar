#include "fltk_toolbar_svg.h"
#include <FL/Fl_SVG_Image.H>
#include <stdexcept>

namespace fltk_toolbar
{
SvgButton::SvgButton(int x, int y, int w, int h, const char *label) : Fl_Button(x, y, w, h, label) {}
SvgButton::~SvgButton()
{
    image(nullptr);
    deimage(nullptr);
}
void SvgButton::svg(const std::string &markup, int size)
{
    if (size < 1 || size > 256 || markup.empty())
        throw std::invalid_argument("SVG buttons need nonempty markup and an icon size from 1 to 256");
    Fl_SVG_Image source(nullptr, markup.c_str());
    if (source.fail())
        throw std::runtime_error("Toolbar SVG could not be rendered");
    std::unique_ptr<Fl_Image> active(source.copy(size, size));
    if (!active || active->fail())
        throw std::runtime_error("Toolbar SVG image could not be created");
    std::unique_ptr<Fl_Image> disabled(active->copy());
    if (!disabled || disabled->fail())
        throw std::runtime_error("Disabled toolbar image could not be created");
    disabled->inactive();
    image(active.get());
    deimage(disabled.get());
    active_ = std::move(active);
    disabled_ = std::move(disabled);
    redraw();
}
} // namespace fltk_toolbar
