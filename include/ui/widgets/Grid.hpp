#pragma once

#include "Widget.hpp"
#include <vector>

struct Coords2 {
    union {int x; int width;};
    union {int y; int height;};

    Coords2 operator-() {
        return {-x, -y};
    }
    Coords2 operator+(Coords2 other) {
        return {x + other.x, y + other.x};
    }
    Coords2 operator-(Coords2 other) {
        return *this + (-other);
    }
};

class Grid: public Widget {
    std::vector<std::vector<Widget*>> widgets;
    std::vector<float> column_size;
    std::vector<float> row_size;

    void calculate_column_size(int col);
    void calculate_row_size(int row);

public:
    Grid();
    ~Grid() override;

    void calculate_size() override;

    Coords2 get_dimensions();
    void set_dimensions(Coords2 dimensions);
    void shrink_to_fit();

    Widget* get_widget(Coords2 place);
    void set_widget(Coords2 place, Widget* widget = nullptr, SizePolicy size_policy = SizePolicy::Expand);
    void set_widget_size_policy(Coords2 place, SizePolicy size_policy);

    void draw(Rectangle bounds) override;
};
