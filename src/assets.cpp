#include "assets.hpp"

#include <string>
#include <raylib.h>
#include <raygui.h>

size_t ship_start_size = 2;
std::vector<std::vector<Texture>> ships;

std::vector<std::vector<std::string>> ships_filenames {
    {"Patrol.png"},
    {"Destroyer.png", "Rescue.png"},
    {"Cruiser.png", "Submarine.png"},
    {"Carrier.png"},
};

void load_ships() {
    // Load ship textures
    for (size_t length = 0; length < ships_filenames.size(); length++) {
        // separate vectors for each ship size
        ships.push_back({});
        for (size_t i = 0; i < ships_filenames.at(length).size(); i++) {
            ships.at(length).emplace_back(
                LoadTexture(("assets/textures/" + ships_filenames.at(length).at(i)).c_str())
            );
        }
    }
}

void load_font() {
    Font font = GetFontDefault();
    GuiSetFont(font);
    GuiSetStyle(DEFAULT, TEXT_SIZE, 24);
}

void load_assets() {
    load_ships();
    load_font();
}
