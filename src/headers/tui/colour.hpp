#ifndef TUI_COLOUR_HPP
#define TUI_COLOUR_HPP

#include <cstdint>
#include <ostream>
namespace tui {
class Colour {
public:
    uint8_t red = 0, green = 0, blue = 0;
    Colour(uint8_t red, uint8_t green, uint8_t blue);
    void applyForeground(std::ostream& output);
    void applyBackground(std::ostream& output);
};
}  // namespace tui

#endif  // !TUI_COLOUR_HPP
