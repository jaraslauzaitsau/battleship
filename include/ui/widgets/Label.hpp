#pragma once

#include "Widget.hpp"
#include <string>

class Label: public Widget {
    std::string text = "";

public:
	Label(Rectangle bounds = {0, 0, 0, 0});
    Label(const std::string& text = "", Rectangle bounds = {0, 0, 0, 0});
    ~Label() override;

    void calculate_size() override;

    const std::string& get_text();
    void set_text(const std::string& new_text);

    void draw() override;
};
