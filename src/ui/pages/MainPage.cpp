#include "ui/pages/MainPage.hpp"
#include "ui/widgets/Label.hpp"
#include "ui/widgets/Button.hpp"
#include <raylib.h>
#include <raygui.h>

MainPage::MainPage() {
    grid.set_widget({0, 0}, new Label("Main Page"), SizePolicy{SizePolicyValue::Expand, SizePolicyValue::Expand});
    Button* exit = new Button("Exit");
    exit->set_callback([] () {
    	CloseWindow();
     	std::exit(0);
    });
    grid.set_widget({1, 1}, exit);
}

MainPage::~MainPage() {

}

void MainPage::draw() {
    grid.draw();
}
