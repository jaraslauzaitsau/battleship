#pragma once

#include <raylib.h>

enum class SizePolicyValue {
    Fit,
    Expand
};
struct SizePolicy {
    inline SizePolicy(SizePolicyValue horizontal, SizePolicyValue vertical):
        horizontal(horizontal), vertical(vertical) {}
    inline SizePolicy(SizePolicyValue all_directions = SizePolicyValue::Fit):
        SizePolicy(all_directions, all_directions) {}
    
    SizePolicyValue horizontal = SizePolicyValue::Expand;
    SizePolicyValue vertical = SizePolicyValue::Expand;
};

class Widget {
protected:
    Widget* parent = nullptr;
    SizePolicy size_policy = SizePolicy{};
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
