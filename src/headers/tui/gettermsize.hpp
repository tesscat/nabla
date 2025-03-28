#ifndef TUI_GETTERMSIZE_HPP
#define TUI_GETTERMSIZE_HPP

#include <optional>
#include <utility>
namespace tui {
std::optional<std::pair<unsigned short, unsigned short>> getTermSize();
}

#endif // !TUI_GETTERMSIZE_HPP
