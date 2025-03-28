#ifndef CORE_MODES_HPP
#define CORE_MODES_HPP

#include <string>
#include "core/events.hpp"
namespace core {

class ModeChangeEvent : public Event {
    friend void setMode(const std::string);
    ModeChangeEvent(std::string oldMode, std::string newMode);

public:
    const std::string oldMode;
    const std::string newMode;
};

extern EventQueue<ModeChangeEvent> modeChangeQueue;

std::string getMode();
void setMode(const std::string mode);

}  // namespace core

#endif
