#pragma once

#include "Page.hpp"
#include "ui/widgets/Grid.hpp"

class MainPage : public Page {
    Grid grid = Grid(Rectangle{0, 0, 800, 600});

public:
    MainPage();
    ~MainPage() override;

    void draw() override;
};
