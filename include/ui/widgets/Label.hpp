#pragma once

#include "Widget.hpp"
#include <string>

class Label: public Widget {
protected:
    std::string text = "";
    Vector2 calculate_size_for_raygui_control(int control);

public:
	Label(Rectangle bounds = {0, 0, 0, 0});
    Label(const std::string& text = "", Rectangle bounds = {0, 0, 0, 0});
    ~Label() override;

    void calculate_size() override;

    const std::string& get_text();
    void set_text(const std::string& new_text);

    void draw() override;
};
