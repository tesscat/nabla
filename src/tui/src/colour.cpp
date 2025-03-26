#include "tui/ansi.hpp"
#include <cstdint>
#include <tui/colour.hpp>

namespace tui {
Colour::Colour(uint8_t red_, uint8_t green_, uint8_t blue_) : red{red_}, green{green_}, blue{blue_} {}
void Colour::applyBackground(std::ostream& output) {
    // ESC[48;2;⟨r⟩;⟨g⟩;⟨b⟩m
    output << ansi::CSI << "48;2;" << red << ';' << green << ';' << blue << 'm';
}
void Colour::applyForeground(std::ostream& output) {
    // ESC[38;2;⟨r⟩;⟨g⟩;⟨b⟩m
    output << ansi::CSI << "38;2;" << red << ';' << green << ';' << blue << 'm';
}
}
