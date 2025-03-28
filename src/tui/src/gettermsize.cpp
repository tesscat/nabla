#include <tui/gettermsize.hpp>
#include <asm/termbits.h>
#include <sys/ioctl.h>

namespace tui {
std::optional<std::pair<unsigned short, unsigned short>> getTermSize() {
    struct winsize size;
    if (ioctl(0, TIOCGWINSZ, &size) == 0) {
        return std::optional(std::pair(size.ws_col, size.ws_row));
    } else return std::optional<std::pair<unsigned short, unsigned short>>();
}
}
