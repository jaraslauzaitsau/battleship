#include "ui/widgets/Button.hpp"
#include "raylib.h"
#include "raygui.h"
#include "ui/widgets/Label.hpp"

Button::Button(Rectangle bounds) : Label(bounds) {}
Button::Button(const std::string& text, Rectangle bounds) : Label(text, bounds) {}
Button::~Button() {}

ButtonState Button::get_button_state() {
    return button_state;
}

void Button::set_button_state(ButtonState button_state) {
    this->button_state = button_state;
}

void Button::update_button_state() {
    if(button_state == ButtonState::Disabled)
        return;
    
    Vector2 cursor_position = GUI_POINTER_POSITION;

    // Checking if the cursor is currently hovering over the button
    if(!CheckCollisionPointRec(cursor_position, bounds))
    {
        button_state = ButtonState::Normal;
        return;
    }

    // This for will return True because GUI_BUTTON_DOWN returns true every frame the button is pressed
    if(GUI_BUTTON_DOWN) {
        button_state = ButtonState::Pressed;
    }
    else {
    	if (button_state == ButtonState::Pressed) {
     		clicked = true;
			if(callback) callback();
     	}
        button_state = ButtonState::Hovered;
    }
}

template <typename ArgType> void Button::set_callback(std::function<void(Button* button, ArgType arg)> callback, ArgType arg) {
	this->callback = std::bind(callback, this, arg);
}
void Button::set_callback(std::function<void(Button* button)> callback) {
	this->callback = std::bind(callback, this);
}
void Button::set_callback(std::function<void()> callback) {
	this->callback = callback;
}

void Button::calculate_size() {
	size = calculate_size_for_raygui_control(LABEL);
	Widget::calculate_size();
}

void Button::draw() {
    GuiButton(bounds, text.c_str());
}