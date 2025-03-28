#ifndef TUI_INPUT_HPP
#define TUI_INPUT_HPP

#include <sys/types.h>
#include <optional>
#include <vector>
#include "core/events.hpp"

namespace tui {
enum Modifier : uint {
    Shift = 0b1,
    Alt = 0b10,
    Ctrl = 0b100,
    Super = 0b1000,
    Hyper = 0b10000,
    Meta = 0b100000,
    CapsLock = 0b1000000,
    NumLock = 0b10000000,
};
enum EventType : uint {
    Press = 1,
    Repeat = 2,
    Release = 3,
};

// based on https://sw.kovidgoyal.net/kitty/keyboard-protocol/#id9
enum SpecialKey : uint {
    ESCAPE = 27,
    ENTER = 13,
    TAB = 9,
    BACKSPACE = 127,
    INSERT = 2,
    DELETE = 3,
    LEFT = 1,
    RIGHT = 4,
    UP = 10,
    DOWN = 16,
    PAGE_UP = 5,
    PAGE_DOWN = 6,
    HOME = 7,
    END = 8,
    CAPS_LOCK = 57358,
    SCROLL_LOCK = 57359,
    NUM_LOCK = 57360,
    PRINT_SCREEN = 57361,
    PAUSE = 57362,
    MENU = 57363,
    F1 = 11,
    F2 = 12,
    F3 = 13,
    F4 = 14,
    F5 = 15,
    F6 = 17,
    F7 = 18,
    F8 = 19,
    F9 = 20,
    F10 = 21,
    F11 = 23,
    F12 = 24,
    F13 = 57376,
    F14 = 57377,
    F15 = 57378,
    F16 = 57379,
    F17 = 57380,
    F18 = 57381,
    F19 = 57382,
    F20 = 57383,
    F21 = 57384,
    F22 = 57385,
    F23 = 57386,
    F24 = 57387,
    F25 = 57388,
    F26 = 57389,
    F27 = 57390,
    F28 = 57391,
    F29 = 57392,
    F30 = 57393,
    F31 = 57394,
    F32 = 57395,
    F33 = 57396,
    F34 = 57397,
    F35 = 57398,
    KP_0 = 57399,
    KP_1 = 57400,
    KP_2 = 57401,
    KP_3 = 57402,
    KP_4 = 57403,
    KP_5 = 57404,
    KP_6 = 57405,
    KP_7 = 57406,
    KP_8 = 57407,
    KP_9 = 57408,
    KP_DECIMAL = 57409,
    KP_DIVIDE = 57410,
    KP_MULTIPLY = 57411,
    KP_SUBTRACT = 57412,
    KP_ADD = 57413,
    KP_ENTER = 57414,
    KP_EQUAL = 57415,
    KP_SEPARATOR = 57416,
    KP_LEFT = 57417,
    KP_RIGHT = 57418,
    KP_UP = 57419,
    KP_DOWN = 57420,
    KP_PAGE_UP = 57421,
    KP_PAGE_DOWN = 57422,
    KP_HOME = 57423,
    KP_END = 57424,
    KP_INSERT = 57425,
    KP_DELETE = 57426,
    KP_BEGIN = 57427,
    MEDIA_PLAY = 57428,
    MEDIA_PAUSE = 57429,
    MEDIA_PLAY_PAUSE = 57430,
    MEDIA_REVERSE = 57431,
    MEDIA_STOP = 57432,
    MEDIA_FAST_FORWARD = 57433,
    MEDIA_REWIND = 57434,
    MEDIA_TRACK_NEXT = 57435,
    MEDIA_TRACK_PREVIOUS = 57436,
    MEDIA_RECORD = 57437,
    LOWER_VOLUME = 57438,
    RAISE_VOLUME = 57439,
    MUTE_VOLUME = 57440,
    LEFT_SHIFT = 57441,
    LEFT_CONTROL = 57442,
    LEFT_ALT = 57443,
    LEFT_SUPER = 57444,
    LEFT_HYPER = 57445,
    LEFT_META = 57446,
    RIGHT_SHIFT = 57447,
    RIGHT_CONTROL = 57448,
    RIGHT_ALT = 57449,
    RIGHT_SUPER = 57450,
    RIGHT_HYPER = 57451,
    RIGHT_META = 57452,
    ISO_LEVEL3_SHIFT = 57453,
    ISO_LEVEL5_SHIFT = 57454,
};

std::pair<uint, char> readNumber(char first);
std::pair<uint, char> readNumber();

class KeyEvent : public core::Event {
    friend KeyEvent readInput();
    KeyEvent(
        char32_t unicode,
        std::optional<char32_t> shifted,
        std::optional<char32_t> physical,
        uint modifiers,
        EventType eventType,
        std::vector<char32_t> textAsCodepoints
    );
    KeyEvent();
public:
    char32_t unicode = 0;
    std::optional<char32_t> shifted;
    std::optional<char32_t> physical;
    uint modifiers = 0;
    EventType eventType = Press;
    std::vector<char32_t> textAsCodepoints;
};

KeyEvent readInput();

extern core::EventQueue<KeyEvent> keyQueue;
}

#endif // !TUI_INPUT_HPP
