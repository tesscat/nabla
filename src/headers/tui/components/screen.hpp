#ifndef TUI_COMPONENTS_SCREEN_HPP
#define TUI_COMPONENTS_SCREEN_HPP

#include <memory>
#include "tui/tui.hpp"
#include <termios.h>


namespace tui {
class Screen {
    struct termios old_termios;
    RenderContext ctx;
public:
    Screen();
    ~Screen();
    void render();
    std::shared_ptr<Component> rootComponent;
};
}

#endif // !TUI_COMPONENTS_SCREEN_HPP
