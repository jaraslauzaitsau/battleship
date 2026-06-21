#include "ui/widgets/Label.hpp"
#include "raygui.h"

Label::Label(Rectangle bounds): Widget(bounds) {}
Label::Label(const std::string& text, Rectangle bounds): Label(bounds) {
    set_text(text);
}
Label::~Label() {}

void Label::calculate_size() {
    const float font_size = (float)GuiGetStyle(DEFAULT, TEXT_SIZE);
    const float spacing = (float)GuiGetStyle(DEFAULT, TEXT_SPACING);
    const int border = GuiGetStyle(LABEL, BORDER_WIDTH);
    const int padding = GuiGetStyle(LABEL, TEXT_PADDING);

    size = MeasureTextEx(
        GuiGetFont(),
        text.c_str(),
        font_size,
        spacing
    );
    
    // raygui might have its own opinion on the width of this text
    float width = GuiGetTextWidth(text.c_str());
    if (width > size.x) size.x = width;

    size.x += 2.0f * (float)(border + padding);
    size.y += 2.0f * (float)(border + padding);

    Widget::calculate_size();
}

const std::string& Label::get_text() {
    return text;
}
void Label::set_text(const std::string& new_text) {
    text = new_text;
    calculate_size();
}

void Label::draw() {
    GuiLabel(bounds, text.c_str());
}
