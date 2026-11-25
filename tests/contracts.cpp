#include "fltk_toolbar.h"
#include "fltk_toolbar_svg.h"
#include <stdexcept>
#include <iostream>

void check(bool condition, const char *message)
{
    if (!condition)
        throw std::runtime_error(message);
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
        svg->svg("<svg xmlns='http://www.w3.org/2000/svg' width='24' height='24'><path d='M4 12H20' stroke='blue'/></svg>");
        check(svg->image() && svg->deimage() && svg->image()->w() == 20, "Owned active and disabled SVGs");
        const auto *previous = svg->image();
        bool rejected = false;
        try { svg->svg("", 0); } catch (const std::invalid_argument &) { rejected = true; }
        check(rejected && svg->image() == previous, "Invalid replacement preserves the current image");
        std::cout << "Toolbar contracts passed\n";
    }
    catch (const std::exception &error)
    {
        std::cerr << error.what() << '\n';
        return 1;
    }
}
