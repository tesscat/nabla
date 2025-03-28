#ifndef TUI_ANSI_HPP
#define TUI_ANSI_HPP

namespace tui::ansi {
const char ESCAPE = '\e';
const char CSI[3] = {ESCAPE, '[', 0};
}  // namespace tui::ansi

#endif  // !TUI_ANSI_HPP
