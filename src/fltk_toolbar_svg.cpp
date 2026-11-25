#include "fltk_toolbar_svg.h"
#include <FL/Fl_SVG_Image.H>
#include <FL/Fl_Window.H>
#include <algorithm>
#include <cmath>
#include <stdexcept>

namespace fltk_toolbar
{
namespace
{
// Compose straight RGBA pixels over the current button color. Opaque RGB avoids
// the platform alpha-blending path, including remote Windows display drivers.
std::unique_ptr<Fl_Image> button_image(const Fl_RGB_Image &raster, Fl_Color background, int size,
                                       bool disabled)
{
    const int w = raster.data_w(), h = raster.data_h();
    const int stride = raster.ld() ? raster.ld() : w * 4;
    unsigned char back[3];
    Fl::get_color(background, back[0], back[1], back[2]);
    auto pixels = std::make_unique<unsigned char[]>(static_cast<std::size_t>(w) * h * 3);
    for (int y = 0; y < h; ++y)
        for (int x = 0; x < w; ++x)
        {
            const auto *source = raster.array + y * stride + x * 4;
            const int alpha = disabled ? source[3] / 2 : source[3];
            const int gray = (31 * source[0] + 61 * source[1] + 8 * source[2]) / 100;
            for (int c = 0; c < 3; ++c)
            {
                const int foreground = disabled ? gray : source[c];
                pixels[(static_cast<std::size_t>(y) * w + x) * 3 + c] =
                    static_cast<unsigned char>((foreground * alpha + back[c] * (255 - alpha) + 127) / 255);
            }
        }
    auto image = std::make_unique<Fl_RGB_Image>(pixels.get(), w, h, 3);
    image->alloc_array = 1;
    pixels.release();
    image->scale(size, size);
    return image;
}
} // namespace
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
    const auto scale = screen_scale();
    const int pixels = static_cast<int>(std::ceil(size * scale));
    source.resize(pixels, pixels);
    source.normalize();
    // Qualified copy produces independent, materialized pixels rather than a lazy
    // SVG copy.
    std::unique_ptr<Fl_RGB_Image> raster(
        static_cast<Fl_RGB_Image *>(source.Fl_RGB_Image::copy(source.data_w(), source.data_h())));
    if (!raster || raster->fail() || !raster->array || raster->d() != 4)
        throw std::runtime_error("Toolbar SVG image could not be created");
    const auto background = value() ? selection_color() : color();
    auto active = button_image(*raster, background, size, false);
    auto disabled = button_image(*raster, background, size, true);
    std::string saved_markup = markup;
    image(active.get());
    deimage(disabled.get());
    active_ = std::move(active);
    disabled_ = std::move(disabled);
    raster_ = std::move(raster);
    markup_ = std::move(saved_markup);
    size_ = size;
    scale_ = scale;
    background_ = Fl::get_color(background);
    redraw();
}
float SvgButton::screen_scale()
{
    auto *owner = window();
    return std::clamp(Fl::screen_scale(owner ? owner->screen_num() : 0), 0.5f, 8.0f);
}
// CSS applies the current hover/pressed background before calling this draw
// method.
void SvgButton::update_images(Fl_Color background)
{
    auto active = button_image(*raster_, background, size_, false);
    auto disabled = button_image(*raster_, background, size_, true);
    image(active.get());
    deimage(disabled.get());
    active_ = std::move(active);
    disabled_ = std::move(disabled);
    background_ = Fl::get_color(background);
}
void SvgButton::draw()
{
    if (raster_)
    {
        if (screen_scale() != scale_)
            svg(markup_, size_);
        else
        {
            const auto background = value() ? selection_color() : color();
            if (background_ != Fl::get_color(background))
                update_images(background);
        }
    }
    Fl_Button::draw();
}
} // namespace fltk_toolbar
