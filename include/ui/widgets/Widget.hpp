#pragma once

#include <raylib.h>

enum class SizePolicy {
    Fit,
    Expand
};

class Widget {
protected:
    Widget* parent = nullptr;
    SizePolicy size_policy = SizePolicy::Expand;
    Vector2 size = {0, 0};

public:
    Widget();
    virtual ~Widget();

    Widget* get_parent();
    void set_parent(Widget* parent);

    Vector2 get_size() const;
    virtual void calculate_size();

    SizePolicy get_size_policy();
    void set_size_policy(SizePolicy size_policy);

    virtual void draw(Rectangle bounds) = 0;
};
