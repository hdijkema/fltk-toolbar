#include "fltk_toolbar.h"
#include <FL/fl_draw.H>
#include <algorithm>

namespace fltk_toolbar
{
namespace
{
class Separator : public Fl_Widget
{
  public:
    Separator() : Fl_Widget(0, 0, 12, 24) {}
    void draw() override
    {
        fl_color(fl_color_average(parent()->labelcolor(), parent()->color(), 0.35f));
        fl_line(x() + w() / 2, y() + 4, x() + w() / 2, y() + h() - 5);
    }
};
} // namespace
Toolbar::Toolbar(int x, int y, int w, int h) : Fl_Group(x, y, w, h)
{
    box(FL_NO_BOX);
    clip_children(1);
    resizable(nullptr);
    end();
}
void Toolbar::add_button(Fl_Button &button)
{
    Fl_Group::add(button);
    layout();
}
void Toolbar::separator()
{
    auto *line = new Separator;
    Fl_Group::add(*line);
    layout();
}
void Toolbar::layout()
{
    int left = x();
    for (int i = 0; i < children(); ++i)
    {
        auto *item = child(i);
        if (!item->visible())
            continue;
        item->resize(left, y(), item->w(), std::max(1, h()));
        left += item->w() + 4;
    }
    redraw();
}
void Toolbar::resize(int x, int y, int w, int h)
{
    Fl_Widget::resize(x, y, w, h);
    layout();
}
} // namespace fltk_toolbar
