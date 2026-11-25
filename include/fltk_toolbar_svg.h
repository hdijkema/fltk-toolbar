#pragma once
#include <FL/Fl_Button.H>
#include <FL/Fl_Image.H>
#include <memory>
#include <string>

namespace fltk_toolbar
{
// An ordinary FLTK button with owned active/disabled SVG images. It also works
// outside a Toolbar and can be combined with Fl_Css_Widget<SvgButton>.
class SvgButton : public Fl_Button
{
  public:
    SvgButton(int x, int y, int w, int h, const char *label = nullptr);
    ~SvgButton() override;
    // Render SVG markup at 1..256 logical pixels. Throws on invalid size/image;
    // previous images remain usable on failure. Call on the GUI thread.
    void svg(const std::string &markup, int size = 20);

  private:
    std::unique_ptr<Fl_Image> active_, disabled_;
};
} // namespace fltk_toolbar
