#include "core/events.hpp"
#include <core/key.hpp>
#include <set>

namespace core {
KeyEvent::KeyEvent(bool pressed_, char32_t key_) : pressed{pressed_}, key{key_} {}
KeyComboEvent::KeyComboEvent(std::set<char32_t> combo_) : combo{combo_} {}

EventQueue<KeyEvent> keyQueue;
EventQueue<KeyComboEvent> keyComboQueue;

bool wasLastPressed = false;
std::set<char32_t> currentlyPressed {};

// TODO: render this threadsafe
void submitKeyEvent(const char32_t key, const bool pressed) {
    KeyEvent keyEvent (pressed, key);
    keyQueue.fireEvent(keyEvent);
    if (pressed) {
        currentlyPressed.insert(key);
    } else {
        if (wasLastPressed) {
            KeyComboEvent keyComboEvent(currentlyPressed);
            keyComboQueue.fireEvent(keyComboEvent);
        }
        currentlyPressed.erase(key);
    }
    wasLastPressed = pressed;
}
}
