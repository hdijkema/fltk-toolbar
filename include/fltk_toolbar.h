#pragma once
#include <FL/Fl_Group.H>
#include <FL/Fl_Button.H>

namespace fltk_toolbar
{
// A horizontal toolbar. Owns registered heap buttons and separators as FLTK children.
// Buttons retain their native callbacks, shortcuts, toggle behavior and tooltips.
class Toolbar : public Fl_Group
{
  public:
    Toolbar(int x, int y, int w, int h);
    // Reparent and own a heap button. Its width is retained; height follows the toolbar.
    // Requires a live button that is not owned by another application component.
    void add_button(Fl_Button &);
    // Insert an owned separator after the current buttons.
    void separator();
    // Reposition children after their visibility or widths have changed.
    void layout();
    void resize(int, int, int, int) override;
};
} // namespace fltk_toolbar
