#pragma once

#include "Page.hpp"

class GamePage: public Page {
public:
    GamePage();
    ~GamePage() override;

    void draw() override;
};
