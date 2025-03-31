#include <iostream>
#include <memory>
#include "editor/buffer.hpp"
#include "loader/app.hpp"
#include "tui/components/screen.hpp"
#include "tui/components/simpletext.hpp"
#include "tui/input.hpp"
#include "utils.hpp"
#include <sstream>

std::shared_ptr<tui::SimpleTextComponent> tc =
    std::make_shared<tui::SimpleTextComponent>(std::string("hello\nthere"));
std::shared_ptr<editor::Buffer> b = std::make_shared<editor::Buffer>(std::string("./compile_commands.json"));
std::vector<std::string> buff{10, std::string()};

void keyconsumer(const tui::KeyEvent& event) {
    std::stringstream ss;
    const char* ets[4]{"unknown", "press", "repeat", "release"};
    ss << "KeyEvent:";
    ss << "\nunicode: " << (uint)event.unicode;
    ss << "\nmods: " << event.modifiers;
    ss << "\nevent: " << ets[event.eventType];
    buff.insert(buff.begin(), ss.str());
    buff.resize(10);
    std::stringstream ss2;
    for (auto& s : buff) {
        ss2 << s << '\n';
    }
    tc->setContents(ss2.str());
}

extern "C" void init(loader::App& app) {
    UNUSED(app);
    std::cout << "main init" << std::endl;

    tui::Screen screen;
    screen.rootComponent = b;

    tui::keyQueue.registerHandler(keyconsumer, "keyconsumer");

    screen.render();
    while (true) {
        auto inp = tui::readInput();
        // tui::keyQueue.fireEvent(inp);
        if (inp.eventType == tui::Press || inp.eventType == tui::Repeat) {
            if (inp.unicode == 'q' ||
                (inp.unicode == 'c' && inp.modifiers == tui::Ctrl))
                break;
            if (inp.unicode == tui::DOWN) {
                b->scroll(1);
            }
            if (inp.unicode == tui::UP)
                b->scroll(-1);
        }
        screen.render();
    };
}


