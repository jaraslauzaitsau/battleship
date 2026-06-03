#include "ui/widgets/Grid.hpp"
#include "ui/widgets/Widget.hpp"

Grid::Grid(): Widget() {}
Grid::~Grid() {
    for (std::vector<Widget*>& row: widgets) {
        for (Widget* widget: row) {
            delete widget;
        }
    }
}

void Grid::calculate_column_size(int col) {
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
void Grid::calculate_row_size(int row) {
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

Coords2 Grid::get_dimensions() {
    return {(int)column_size.size(), (int)row_size.size()};
}
void Grid::set_dimensions(Coords2 dimensions) {
    if (dimensions.width > -1) {
        column_size.resize(dimensions.width);
    }
    if (dimensions.height > -1) {
        row_size.resize(dimensions.height);
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
    else if (place.x > get_dimensions().width) {
        set_dimensions({place.x + 1, -1});
    }
    else if (place.y > get_dimensions().height) {
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

void Grid::draw(Rectangle bounds) {
    std::vector<float> column_size_real(column_size.size());
    std::vector<float> row_size_real(row_size.size());

    float width_ratio = bounds.width / size.x;
    float height_ratio = bounds.height / size.y;

    for (int x = 0; x < column_size_real.size(); x++) {
        column_size_real.at(x) = column_size.at(x) * width_ratio;
    }
    for (int y = 0; y < row_size_real.size(); y++) {
        row_size_real.at(y) = row_size.at(y) * height_ratio;
    }

    float pos_y = 0;
    for (int y = 0; y < row_size_real.size(); y++) {
        float pos_x = 0;
        for (int x = 0; x < column_size_real.size(); x++) {
            if (widgets.at(y).at(x)) {
                widgets.at(y).at(x)->draw(
                    {bounds.x + pos_x, bounds.y + pos_y,
                    column_size_real.at(x), row_size_real.at(y)}
                );
            }
            pos_x += column_size_real.at(x);
        }
        pos_y += row_size_real.at(y);
    }
}
