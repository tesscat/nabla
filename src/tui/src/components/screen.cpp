#include <sys/ioctl.h>
#include <termios.h>
#include <cstdio>
#include <iostream>
#include <tui/components/screen.hpp>
#include "tui/gettermsize.hpp"
#include "tui/tui.hpp"
#include "utils.hpp"
#include <tui/ansi.hpp>
#include <csignal>
#include <tui/input.hpp>

// sigwinch (window resize) handler
// TODO: make this global threadsafe

namespace tui {

tui::RenderExtent currentSize {.row=0, .col=0, .width=80, .height=30};
bool hasSizeChanged = false;
extern "C" void sigwinch(int sig) {
    UNUSED(sig);
    hasSizeChanged = true;
}
void updateSize() {
    auto newSize = getTermSize();
    if (newSize.has_value()) {
        auto value = newSize.value();
        currentSize.width = value.first;
        currentSize.height = value.second;
        // std::cout << "resized to " << value.first << 'x' << value.second << std::endl;
    }
}

Screen::Screen() {
    tcgetattr(0, &old_termios);
    struct termios new_termios = old_termios;
    // disable buffered input
    // disable echo
    // disable input preprocessing (so now we handle things like ctrl-v)
    new_termios.c_lflag &= ~(ICANON | ECHO | IEXTEN);
    // disable software control flow (we handle ctrl-s and ctrl-q)
    // disable turning carriage returns to newlines (we handle ctrl-j and ctrl-m)
    new_termios.c_iflag &= ~(IXON | ICRNL);
    // disable output preprocessing (e.g. adding carriage returns to newlines)
    new_termios.c_oflag &= ~OPOST;
    // reads timeout instantly (if there's nothing to be read)
    new_termios.c_cc[VTIME] = 0;
    // minimum number of bytes we can read (less important than timeout)
    new_termios.c_cc[VMIN] = 1;
    tcsetattr(0, TCSAFLUSH, &new_termios);
    // hide the cursor
    std::cout << ansi::CSI << "?25l";
    // save current cursor position
    std::cout << ansi::CSI << 's';
    // save screen (this one isn't listed on wikipedia)
    std::cout << ansi::CSI << "?47h";
    // enter alternative buffer
    std::cout << ansi::CSI << "?1049h";
    // enable kitty's input protocol
    // set the flags we want
    // disambiguate escape codes (into CSI u sequences)
    // report event types (press/repeat/release)
    // send both basic + shifted + physical layout key
    // and treat all keys as escape codes, so we don't get text anymore
    std::cout << ansi::CSI << '=' << (0b1 | 0b10 | 0b100 | 0b1000) << ";1u";
    // flush cout
    std::cout << std::endl;
    // bind sigwinch
    signal(SIGWINCH, sigwinch);
    // set size
    updateSize();
}

void Screen::render() {
    // reset position
    if (hasSizeChanged) {
        hasSizeChanged = false;
        updateSize();
    }
    auto toDraw = rootComponent->render(ctx, currentSize);
    for (size_t i = 0; i < currentSize.height; i++) {
        std::cout << ansi::CSI << (i+1) << ";1H" << std::endl;
        toDraw[i]->apply(std::cout);
    }
}

Screen::~Screen() {
    tcsetattr(0, TCSANOW, &old_termios);
    // disable input protocol (pop flags)
    std::cout << ansi::CSI << "<u";
    // exit alternative buffer
    std::cout << ansi::CSI << "?1049l";
    // restore saved screen
    std::cout << ansi::CSI << "?47l";
    // restore cursor position
    std::cout << ansi::CSI << 'u';
    // re-show the cursor
    std::cout << ansi::CSI << "?25h";
    // flush cout
    std::cout << std::endl;
}
}  // namespace tui
