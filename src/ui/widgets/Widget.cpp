#include "ui/widgets/Widget.hpp"

Widget::Widget() {}
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
