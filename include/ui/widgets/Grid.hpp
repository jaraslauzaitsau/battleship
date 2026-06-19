#pragma once

#include "Widget.hpp"
#include <cstddef>
#include <vector>

struct Coords2 {
    union {int x; int width;};
    union {int y; int height;};

    Coords2 operator-() {
        return {-x, -y};
    }
    Coords2 operator+(Coords2 other) {
        return {x + other.x, y + other.y};
    }
    Coords2 operator-(Coords2 other) {
        return *this + (-other);
    }
};

class Grid: public Widget {
    std::vector<std::vector<Widget*>> widgets;

    std::vector<float> column_size;
    std::vector<float> row_size;

    std::vector<SizePolicyValue> column_size_policy;
    std::vector<SizePolicyValue> row_size_policy;

    std::vector<float> column_size_real;
    std::vector<float> row_size_real;

    void calculate_column_size(size_t col);
    void calculate_row_size(size_t row);

    void calculate_column_size_policy(size_t col);
    void calculate_row_size_policy(size_t row);

    void set_widget_bounds();
    void calculate_real_sizes();

    void draw_widgets(const std::vector<float>& column_size_real, const std::vector<float>& row_size_real);

public:
    Grid(Rectangle bounds = {0, 0, 0, 0});
    ~Grid() override;

    void calculate_size() override;
    
    void calculate_size_policies();

    Coords2 get_dimensions();
    void set_dimensions(Coords2 dimensions);
    void shrink_to_fit();

    Widget* get_widget(Coords2 place);
    void set_widget(Coords2 place, Widget* widget = nullptr, SizePolicy size_policy = SizePolicy{});
    void set_widget_size_policy(Coords2 place, SizePolicy size_policy);

    void draw() override;
};
