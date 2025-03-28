#include <core/modes.hpp>
#include "core/events.hpp"

namespace core {
ModeChangeEvent::ModeChangeEvent(std::string oldMode_, std::string newMode_)
    : oldMode{oldMode_}, newMode{newMode_} {}
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
}  // namespace core
