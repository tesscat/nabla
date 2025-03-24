#include "core/events.hpp"
namespace core::events {
EventQueue<Event> rootQueue {};

Event::Event() {}
Event::~Event() {}
}
