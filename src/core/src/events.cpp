#include "core/events.hpp"
namespace core {
EventQueue<Event> rootQueue{};

Event::Event() {}
Event::~Event() {}
}  // namespace core
