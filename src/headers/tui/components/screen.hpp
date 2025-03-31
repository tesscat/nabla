#ifndef TUI_COMPONENTS_SCREEN_HPP
#define TUI_COMPONENTS_SCREEN_HPP

#include <termios.h>
#include <memory>
#include "tui/tui.hpp"

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
}  // namespace tui

#endif  // !TUI_COMPONENTS_SCREEN_HPP
