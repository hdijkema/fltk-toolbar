#pragma once
#include <FL/Fl_Button.H>
#include <FL/Fl_Image.H>
#include <memory>
#include <string>
class Fl_RGB_Image;

namespace fltk_toolbar
{
// An ordinary FLTK button with owned active/disabled RGB images rendered from
// SVG. It also works outside a Toolbar and can be combined with
// Fl_Css_Widget<SvgButton>.
class SvgButton : public Fl_Button
{
  public:
    SvgButton(int x, int y, int w, int h, const char *label = nullptr);
    ~SvgButton() override;
    // Render SVG markup at 1..256 logical pixels. Throws on invalid size/image;
    // previous images remain usable on failure. Call on the GUI thread.
    void svg(const std::string &markup, int size = 20);

  protected:
    void draw() override;

  private:
    std::unique_ptr<Fl_Image> active_, disabled_;
    std::unique_ptr<Fl_RGB_Image> raster_;
    std::string markup_;
    int size_ = 20;
    float scale_ = 1;
    Fl_Color background_ = FL_BACKGROUND_COLOR;
    void update_images(Fl_Color background);
    float screen_scale();
};
} // namespace fltk_toolbar
