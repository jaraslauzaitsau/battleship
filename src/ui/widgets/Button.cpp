#include "ui/widgets/Button.hpp"
#include "raylib.h"
#include "raygui.h"
#include "ui/widgets/Label.hpp"

Button::Button(Rectangle bounds) : Label(bounds) { }
Button::Button(const std::string& text, Rectangle bounds) : Label(text, bounds) { }
Button::~Button() { }

const ButtonStates Button::get_button_state() {
    return button_state;
}

void Button::update_button_state() {
    if(button_state == ButtonStates::Disabled)
        return;
    
    Vector2 cursor_position = GUI_POINTER_POSITION;

    // Checking if the cursor is currently hovering over the button
    if(!CheckCollisionPointRec(cursor_position, bounds))
    {
        button_state = ButtonStates::Normal;
        return;
    }

    // This for will return True because GUI_BUTTON_DOWN returns true every frame the button is pressed
    if(GUI_BUTTON_DOWN) 
        button_state = ButtonStates::Pressed;
    else
        button_state = ButtonStates::Hovered;
}

void Button::set_button_state(ButtonStates button_state) {
    this->button_state = button_state;
}


void Button::calculate_size() 
{ 
    Label::calculate_size();
}

void Button::draw() { 
    Label::draw();
}   