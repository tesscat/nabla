#ifndef CORE_KEY_HPP
#define CORE_KEY_HPP

#include "core/events.hpp"
#include <set>
namespace core {
class KeyEvent : Event {
    friend void submitKeyEvent(const char32_t key, const bool pressed);
    KeyEvent(bool pressed, char32_t key);
public:
    bool pressed;
    char32_t key;
};

class KeyComboEvent : public Event {
    friend void submitKeyEvent(const char32_t key, const bool pressed);
    KeyComboEvent(std::set<char32_t> combo);
public:
    std::set<char32_t> combo;
};

extern EventQueue<KeyEvent> keyQueue;
extern EventQueue<KeyComboEvent> keyComboQueue;

void submitKeyEvent(const char32_t key, const bool pressed);
}

#endif // !CORE_KEY_HPP
