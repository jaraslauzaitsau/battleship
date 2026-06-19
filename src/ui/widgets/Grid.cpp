#include "ui/widgets/Grid.hpp"
#include "ui/widgets/Widget.hpp"

Grid::Grid(Rectangle bounds): Widget(bounds) {}
Grid::~Grid() {
    for (std::vector<Widget*>& row: widgets) {
        for (Widget* widget: row) {
            delete widget;
        }
    }
}

void Grid::calculate_column_size(size_t col) {
    float size = 0;
    for (int y = 0; y < row_size.size(); y++) {
        Widget* w = widgets.at(y).at(col);
        if (w) {
            float w_size = w->get_size().x;
            if (w_size > size) size = w_size;
        }
    }
    column_size.at(col) = size;
}
void Grid::calculate_row_size(size_t row) {
    float size = 0;
    for (int x = 0; x < column_size.size(); x++) {
        Widget* w = widgets.at(row).at(x);
        if (w) {
            float w_size = w->get_size().y;
            if (w_size > size) size = w_size;
        }
    }
    row_size.at(row) = size;
}
void Grid::calculate_size() {
    float width = 0;
    float height = 0;

    for (int x = 0; x < column_size.size(); x++) {
        calculate_column_size(x);
        width += column_size.at(x);
    }
    for (int y = 0; y < row_size.size(); y++) {
        calculate_row_size(y);
        height += row_size.at(y);
    }

    size = {width, height};

    Widget::calculate_size();
}

void Grid::calculate_column_size_policy(size_t col) {
    SizePolicyValue policy = SizePolicyValue::Fit;
    for (int y = 0; y < row_size.size(); y++) {
        Widget* w = widgets.at(y).at(col);
        if (!w) continue;
        SizePolicyValue widget_policy = w->get_size_policy().horizontal;
        if (has_precedence(widget_policy, policy)) policy = widget_policy;
    }
    column_size_policy.at(col) = policy;
}
void Grid::calculate_row_size_policy(size_t row) {
    SizePolicyValue policy = SizePolicyValue::Fit;
    for (int x = 0; x < column_size.size(); x++) {
        Widget* w = widgets.at(row).at(x);
        if (!w) continue;
        SizePolicyValue widget_policy = w->get_size_policy().vertical;
        if (has_precedence(widget_policy, policy)) policy = widget_policy;
    }
    row_size_policy.at(row) = policy;
}
void Grid::calculate_size_policies() {
    for (int x = 0; x < column_size_policy.size(); x++) {
        calculate_column_size_policy(x);
    }
    for (int y = 0; y < row_size_policy.size(); y++) {
        calculate_row_size_policy(y);
    }
}

void Grid::set_widget_bounds() {
    float pos_y = 0;
    for (int y = 0; y < row_size_real.size(); y++) {
        float pos_x = 0;
        for (int x = 0; x < column_size_real.size(); x++) {
            if (widgets.at(y).at(x)) {
                widgets.at(y).at(x)->set_bounds(
           			{bounds.x + pos_x, bounds.y + pos_y, column_size_real.at(x), row_size_real.at(y)}
                );
            }
            pos_x += column_size_real.at(x);
        }
        pos_y += row_size_real.at(y);
    }
}

void Grid::calculate_real_sizes() {
    float fixed_width = 0;
    int expand_width_count = 0;
    for (size_t i = 0; i < column_size.size(); ++i) {
        if (column_size_policy.at(i) == SizePolicyValue::Fit) {
            fixed_width += column_size.at(i);
        } else {
            expand_width_count++;
        }
    }
    float available_for_expand_width = std::max(0.0f, bounds.width - fixed_width);
    float expand_width = (expand_width_count > 0) ? (available_for_expand_width / expand_width_count) : 0;

    column_size_real.resize(column_size.size());
    for (size_t i = 0; i < column_size.size(); ++i) {
        if (column_size_policy.at(i) == SizePolicyValue::Fit) {
            column_size_real.at(i) = column_size.at(i);
        } else {
            column_size_real.at(i) = expand_width;
        }
    }

    float fixed_height = 0;
    int expand_height_count = 0;
    for (size_t i = 0; i < row_size.size(); ++i) {
        if (row_size_policy.at(i) == SizePolicyValue::Fit) {
            fixed_height += row_size.at(i);
        } else {
            expand_height_count++;
        }
    }
    float available_for_expand_height = std::max(0.0f, bounds.height - fixed_height);
    float expand_height = (expand_height_count > 0) ? (available_for_expand_height / expand_height_count) : 0;

    row_size_real.resize(row_size.size());
    for (size_t i = 0; i < row_size.size(); ++i) {
        if (row_size_policy.at(i) == SizePolicyValue::Fit) {
            row_size_real.at(i) = row_size.at(i);
        } else {
            row_size_real.at(i) = expand_height;
        }
    }

    set_widget_bounds();
}

Coords2 Grid::get_dimensions() {
    return {(int)column_size.size(), (int)row_size.size()};
}
void Grid::set_dimensions(Coords2 dimensions) {
    if (dimensions.width > -1) {
        column_size.resize(dimensions.width);
        column_size_policy.resize(dimensions.width);
    }
    if (dimensions.height > -1) {
        row_size.resize(dimensions.height);
        row_size_policy.resize(dimensions.height);
    }

    widgets.resize(row_size.size(), std::vector<Widget*>(column_size.size(), nullptr));
    for (std::vector<Widget*>& row: widgets) {
        row.resize(column_size.size(), nullptr);
    }
}
void Grid::shrink_to_fit() {
    for (int y = row_size.size() - 1; y > 0; y--) {
        bool empty = true;
        for (int x = 0; x < column_size.size(); x++) {
            if (widgets[y][x] != nullptr) {
                empty = false;
            }
        }
        if (empty) {
            continue;
        }
        else {
            widgets.resize(y + 1);
            break;
        }
    }

    for (int x = column_size.size() - 1; x > 0; x--) {
        bool empty = true;
        for (int y = 0; y < row_size.size(); y++) {
            if (widgets[y][x] != nullptr) {
                empty = false;
            }
        }
        if (empty) {
            continue;
        }
        else {
            for (std::vector<Widget*>& row: widgets) {
                row.resize(x + 1);
            }
            break;
        }
    }
}

Widget* Grid::get_widget(Coords2 place) {
    return widgets.at(place.y).at(place.x);
}
void Grid::set_widget(Coords2 place, Widget* widget, SizePolicy size_policy) {
    if (place.x >= get_dimensions().width && place.y >= get_dimensions().height) {
        set_dimensions(place + Coords2{1, 1});
    }
    else if (place.x >= get_dimensions().width) {
        set_dimensions({place.x + 1, -1});
    }
    else if (place.y >= get_dimensions().height) {
        set_dimensions({-1, place.y + 1});
    }

    delete widgets.at(place.y).at(place.x);
    widgets.at(place.y).at(place.x) = widget;
    if (widget) widget->set_size_policy(size_policy);

    calculate_size();
}
void Grid::set_widget_size_policy(Coords2 place, SizePolicy size_policy) {
    widgets.at(place.y).at(place.x)->set_size_policy(size_policy);
}

void Grid::draw_widgets(const std::vector<float>& column_size_real, const std::vector<float>& row_size_real) {
    for (int y = 0; y < row_size_real.size(); y++) {
        for (int x = 0; x < column_size_real.size(); x++) {
            if (widgets.at(y).at(x)) {
                widgets.at(y).at(x)->draw();
            }
        }
    }
}

void Grid::draw() {
    calculate_size_policies();
    calculate_real_sizes();
    
    draw_widgets(column_size_real, row_size_real);
}
