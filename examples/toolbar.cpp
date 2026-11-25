#include "fltk_toolbar.h"
#include "fltk_toolbar_svg.h"
#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
int main()
{
    Fl_Double_Window window(500, 240, "Toolbar example");
    auto *toolbar = new fltk_toolbar::Toolbar(10, 10, 480, 34);
    auto *button = new Fl_Button(0, 0, 60, 34, "New");
    button->tooltip("Create a document");
    toolbar->add_button(*button);
    toolbar->separator();
    auto *svg = new fltk_toolbar::SvgButton(0, 0, 34, 34);
    svg->svg("<svg xmlns='http://www.w3.org/2000/svg' width='24' height='24'><path d='M4 12H20M12 4V20' stroke='#236baa' stroke-width='2'/></svg>");
    svg->tooltip("Add an item");
    toolbar->add_button(*svg);
    window.end();
    window.show();
    return Fl::run();
}
