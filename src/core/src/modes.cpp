#include "core/events.hpp"
#include <core/modes.hpp>

namespace core {
ModeChangeEvent::ModeChangeEvent(std::string oldMode_, std::string newMode_) : oldMode{oldMode_}, newMode{newMode_} {}
EventQueue<ModeChangeEvent> modeChangeQueue(&rootQueue);
std::string mode;

std::string getMode() {
    return mode;
}

void setMode(const std::string newMode) {
    ModeChangeEvent event(mode, newMode);
    mode = newMode;
    modeChangeQueue.fireEvent(event);
}
}
