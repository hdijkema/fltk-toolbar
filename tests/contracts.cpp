#include "fltk_toolbar.h"
#include "fltk_toolbar_svg.h"
#include <FL/Fl_Image_Surface.H>
#include <FL/Fl_RGB_Image.H>
#include <iostream>
#include <stdexcept>

void check(bool condition, const char *message)
{
    if (!condition)
        throw std::runtime_error(message);
}
// Check real pixels, rather than accepting a lazy image that merely reports a
// size.
void check_pixels(const Fl_Image *image, Fl_Color background)
{
    auto *rgb = dynamic_cast<const Fl_RGB_Image *>(image);
    check(rgb && rgb->array && rgb->d() == 3, "Button images contain eager opaque RGB pixels");
    unsigned char r, g, b;
    Fl::get_color(background, r, g, b);
    check(rgb->array[0] == r && rgb->array[1] == g && rgb->array[2] == b,
          "Transparent SVG corners match the button background");
    bool visible = false;
    for (int i = 0; i < rgb->data_w() * rgb->data_h(); ++i)
        if (rgb->array[i * 3] != r || rgb->array[i * 3 + 1] != g || rgb->array[i * 3 + 2] != b)
            visible = true;
    check(visible, "SVG strokes produce visible pixels");
}
// Exercise the button's native draw path without opening a desktop window.
void render(Fl_Widget &widget)
{
    Fl_Image_Surface surface(widget.w(), widget.h());
    Fl_Surface_Device::push_current(&surface);
    surface.draw(&widget);
    Fl_Surface_Device::pop_current();
}
int main()
{
    try
    {
        fltk_toolbar::Toolbar toolbar(10, 20, 400, 34);
        auto *first = new Fl_Button(0, 0, 34, 34, "A");
        auto *second = new Fl_Button(0, 0, 60, 34, "B");
        toolbar.add_button(*first);
        toolbar.separator();
        toolbar.add_button(*second);
        check(first->parent() == &toolbar && second->x() == 64, "Ownership and separator layout");
        int calls = 0;
        first->callback([](Fl_Widget *, void *data) { ++*static_cast<int *>(data); }, &calls);
        first->do_callback();
        check(calls == 1, "Native callbacks remain intact");
        toolbar.resize(40, 50, 600, 40);
        check(first->x() == 40 && first->y() == 50 && first->h() == 40 && second->w() == 60,
              "Resizing preserves button widths");
        first->hide();
        toolbar.layout();
        check(second->x() == 56, "Hidden buttons release their layout space");
        auto *svg = new fltk_toolbar::SvgButton(0, 0, 34, 34);
        toolbar.add_button(*svg);
        svg->color(fl_rgb_color(12, 34, 56));
        svg->svg("<svg xmlns='http://www.w3.org/2000/svg' width='24' "
                 "height='24'><path d='M4 12H20' stroke='blue'/></svg>");
        check(svg->image() && svg->deimage() && svg->image()->w() == 20, "Owned active and disabled SVGs");
        check_pixels(svg->image(), svg->color());
        check_pixels(svg->deimage(), svg->color());
        svg->color(fl_rgb_color(70, 80, 90));
        render(*svg);
        check_pixels(svg->image(), svg->color());
        check_pixels(svg->deimage(), svg->color());
        svg->selection_color(fl_rgb_color(100, 110, 120));
        svg->value(1);
        render(*svg);
        check_pixels(svg->image(), svg->selection_color());
        check_pixels(svg->deimage(), svg->selection_color());
        svg->value(0);
        svg->color(FL_BACKGROUND_COLOR);
        Fl::background(15, 35, 55);
        render(*svg);
        check_pixels(svg->image(), FL_BACKGROUND_COLOR);
        check_pixels(svg->deimage(), FL_BACKGROUND_COLOR);
        const auto *previous = svg->image();
        bool rejected = false;
        try
        {
            svg->svg("", 0);
        }
        catch (const std::invalid_argument &)
        {
            rejected = true;
        }
        check(rejected && svg->image() == previous, "Invalid replacement preserves the current image");
        std::cout << "Toolbar contracts passed\n";
    }
    catch (const std::exception &error)
    {
        std::cerr << error.what() << '\n';
        return 1;
    }
}
