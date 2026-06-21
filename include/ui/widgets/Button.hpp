#pragma once

#include "Label.hpp"
#include "raylib.h"

enum class ButtonStates {
    Normal,
    Hovered,
    Pressed,
    Disabled // when in this state we don't automatically update the button_state
};

class Button : public Label {
    ButtonStates button_state = ButtonStates::Normal;
public:
    Button(Rectangle bounds = {0, 0, 0, 0});
    Button(const std::string& text = "", Rectangle bounds = {0, 0, 0, 0});
    ~Button() override;

    const ButtonStates get_button_state();
    void update_button_state();

    void set_button_state(ButtonStates button_state);

    void calculate_size() override;
    
    void draw() override;
};
