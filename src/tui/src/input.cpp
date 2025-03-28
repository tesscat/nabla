#include <sys/types.h>
#include <cstdio>
#include <cstdlib>
#include <optional>
#include <stdexcept>
#include <tui/input.hpp>
#include <utility>
#include <vector>
#include "core/events.hpp"
#include "tui/ansi.hpp"

std::vector<char> current_output;
char getchar_wrapped() {
    char c = getchar();
    current_output.push_back(c);
    return c;
}

namespace tui {
std::pair<uint, char> readNumber(char first) {
    char buff[8] = {first, 0, 0, 0, 0, 0, 0, 0};
    char curr = getchar_wrapped();
    int idx = 1;
    while (std::isdigit(curr)) {
        buff[idx] = curr;
        idx++;
        curr = getchar_wrapped();
    }
    return std::pair(std::atoi(buff), curr);
}
std::pair<uint, char> readNumber() {
    return readNumber(getchar_wrapped());
}

KeyEvent::KeyEvent(
    char32_t unicode_,
    std::optional<char32_t> shifted_,
    std::optional<char32_t> physical_,
    uint modifiers_,
    EventType eventType_,
    std::vector<char32_t> textAsCodepoints_
) : unicode{unicode_}, shifted{shifted_}, physical{physical_}, modifiers{modifiers_},
    eventType{eventType_}, textAsCodepoints{textAsCodepoints_} {}
KeyEvent::KeyEvent() {}

uint decodeKeyCode(char c) {
    switch (c) {
        case 'D': return LEFT;
        case 'C': return RIGHT;
        case 'A': return UP;
        case 'B': return DOWN;
        case 'H': return HOME;
        case 'F': return END;
        case 'P': return F1;
        case 'Q': return F2;
        case 'S': return F4;
        case 'E': return KP_BEGIN;
        default: return 0;
    }
}

bool decodePotentiallyTerminatingCharacter(char c, KeyEvent& ke) {
    if (c == 'u' || c == '~') return true;
    else if ('A' <= c && c <= 'Z') {
        ke.unicode = decodeKeyCode(c);
        return true;
    } else return false;
}

KeyEvent readInput() {
    current_output.clear();
    // while (getchar_wrapped() != 'u') {}
    // return KeyEvent{};

    // we expect:
    // CSI 
    // unicode : [alt-code] : phys-loc ;
    // modifiers [: event-type] ;
    // text-as-codepoint [:second-codepoint:...] u
    char first = getchar_wrapped();
    char second = getchar_wrapped();
    if (first != ansi::CSI[0] || second != ansi::CSI[1])
        throw std::runtime_error("tui: input: Non-CSI initialiser on input");

    KeyEvent ke {};

    char curr = getchar_wrapped();
    if (decodePotentiallyTerminatingCharacter(curr, ke)) return ke;

    auto read_result = readNumber(curr);
    ke.unicode = read_result.first;
    curr = read_result.second;
    if (decodePotentiallyTerminatingCharacter(curr, ke)) return ke;
    if (curr != ';') {
        // we have bonus key info
        // skip the known :
        curr = getchar_wrapped();
        if (curr != ':') {
            // we have shift-key info!
            auto read_result = readNumber(curr);
            ke.shifted = read_result.first;
            curr = read_result.second;
            if (decodePotentiallyTerminatingCharacter(curr, ke)) return ke;
        }


        if (curr != ';') {
            // base key info!
            // skip the :
            auto read_result = readNumber(curr);
            ke.physical = read_result.first;
            curr = read_result.second;
            if (decodePotentiallyTerminatingCharacter(curr, ke)) return ke;
        }

        if (decodePotentiallyTerminatingCharacter(curr, ke)) return ke;
    }
    // we have read the key code + alt key codes
    // modifiers time
    // skip the ;
    read_result = readNumber();
    ke.modifiers = read_result.first - 1;
    curr = read_result.second;
    if (decodePotentiallyTerminatingCharacter(curr, ke)) return ke;
    if (curr != ';') {
        auto read_result = readNumber();
        ke.eventType = static_cast<EventType>(read_result.first);
        curr = read_result.second;
        if (decodePotentiallyTerminatingCharacter(curr, ke)) return ke;
    }

    // skip the ;
    curr = getchar_wrapped();
    // codepoints time!
    while (decodePotentiallyTerminatingCharacter(curr, ke)) {
        auto read_result = readNumber(curr);
        ke.textAsCodepoints.push_back(read_result.first);
        curr = read_result.second;
    }

    return ke;
}

core::EventQueue<KeyEvent> keyQueue {&core::rootQueue};

}
