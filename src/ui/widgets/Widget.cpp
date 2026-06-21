#include "ui/widgets/Widget.hpp"

bool has_precedence(SizePolicyValue value, SizePolicyValue base) {
    if ((int)value > (int)base) return true;
    return false;
}

Widget::Widget(Rectangle bounds): bounds(bounds) {}
Widget::~Widget() {}

Widget* Widget::get_parent() {
    return parent;
}
void Widget::set_parent(Widget* parent) {
    this->parent = parent;
}

Vector2 Widget::get_size() const {
    return size;
}
void Widget::calculate_size() {
    if (parent) parent->calculate_size();
}

SizePolicy Widget::get_size_policy() {
    return size_policy;
}
void Widget::set_size_policy(SizePolicy size_policy) {
    this->size_policy = size_policy;
}

Rectangle Widget::get_bounds() {
    return bounds;
}
void Widget::set_bounds(Rectangle bounds) {
    this->bounds = bounds;
}
