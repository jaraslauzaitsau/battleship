#pragma once

#include <functional>
#include "Label.hpp"
#include "raylib.h"

enum class ButtonState {
    Normal,
    Hovered,
    Pressed,
    Disabled // when in this state we don't automatically update the button_state
};

class Button : public Label {
    ButtonState button_state = ButtonState::Normal;
    std::function<void()> callback;
    bool clicked = false;
public:
    Button(Rectangle bounds = {0, 0, 0, 0});
    Button(const std::string& text = "", Rectangle bounds = {0, 0, 0, 0});
    ~Button() override;

    ButtonState get_button_state();
    /**
     * @brief Returns true if the button is currently pressed.
     */
    inline bool is_pressed() {return get_button_state() == ButtonState::Pressed;}
    /**
     * @brief Returns true if the button was clicked (pressed and released) since the last time this function was called.
     */
    bool is_clicked();

    /**
     * @brief Sets button state to a specified state. Can be used to disable a button if passed ButtonState::Disabled.
     */
    void set_button_state(ButtonState button_state);
    /**
     * @brief Updates the button state based on the current mouse and button state.
     * If state is `ButtonState::Disabled`, this function does nothing.
     */
    void update_button_state();

    template <typename ArgType> void set_callback(std::function<void(Button* button, ArgType arg)> callback, ArgType arg);
    void set_callback(std::function<void(Button* button)> callback);
    void set_callback(std::function<void()> callback);

    void calculate_size() override;
    
    void draw() override;
};
