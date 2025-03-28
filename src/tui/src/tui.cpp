#include "tui/tui.hpp"
#include <unistd.h>
#include <cstdio>
#include <iostream>
#include <memory>
#include <sstream>
#include "loader/app.hpp"
#include "tui/components/screen.hpp"
#include "tui/components/simpletext.hpp"
#include "tui/input.hpp"
#include "utils.hpp"

std::shared_ptr<tui::SimpleTextComponent> tc = std::make_shared<tui::SimpleTextComponent>(std::string("hello\nthere"));
extern std::vector<char> current_output;
std::vector<std::string> buff {15, std::string("          ")};


void keyconsumer(const tui::KeyEvent& event) {
    std::stringstream ss;
    const char* ets[4] {"unknown", "press", "repeat", "release"};
    ss << "KeyEvent:";
    ss << "\nunicode: " << (uint)event.unicode;
    ss << "\nmods: " << event.modifiers;
    ss << "\nevent: " << ets[event.eventType];
    buff.insert(buff.begin(), ss.str());
    buff.resize(5);
    std::stringstream ss2;
    for (auto& s : buff) {
        ss2 << s << '\n';
    }
    tc->setContents(ss2.str());
}

extern "C" void init(loader::App& app) {
    UNUSED(app);
    std::cout << "TUI module init" << std::endl;

    // bool x = true;
    // while (x) {};

    tui::Screen screen;
    screen.rootComponent = tc;

    tui::keyQueue.registerHandler(keyconsumer, "keyconsumer");

    screen.render();
    while (true) {
        // current_output.clear();
        auto inp = tui::readInput();
        // current_output.push_back(0);
        // buff.insert(buff.begin(), std::string(current_output.data()).substr(1));
        // buff.resize(10);
        tui::keyQueue.fireEvent(inp);
        if (inp.unicode == 'q' || (inp.unicode == 'c' && inp.modifiers == tui::CapsLock)) break;
        // char c = getchar();
        // if (c == 'q') break;
        // std::stringstream ss;
        // for (auto s : buff) ss << s << '\n';
        // ss << "Read character: " << c << " (as hex) " << std::uppercase << std::setfill('0') << std::setw(2) << std::hex << static_cast<int>(c);
        // tc->setContents(ss.str());
        screen.render();
        // std::this_thread::sleep_for(std::chrono::milliseconds(250));
    };
}

namespace tui {
TermOutputter::~TermOutputter() {}

Component::~Component() {}
}
