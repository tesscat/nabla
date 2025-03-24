#include "core/events.hpp"
#include "loader/app.hpp"
#include <iostream>

class MyEvent : public core::events::Event {
public:
    std::string name;
    MyEvent(std::string name_) : name{name_} {}
    ~MyEvent() override {}
};

extern "C" void preinit(loader::App app) {
    std::cout << "hello from core at preinit!!\n";
}

extern "C" void init(loader::App app) {
    std::cout << "hello from core at init!!\n";
    core::events::EventQueue<MyEvent> queue {&core::events::rootQueue};
    core::events::rootQueue.registerHandler([](core::events::Event& e) {std::cout << "root event!\n";}, std::string("root event!!-er"));
    queue.registerHandler([](MyEvent& e) {std::cout << "my event!" << e.name << "\n";}, std::string("event!!-er"));
    MyEvent event {std::string("hii")};
    queue.fireEvent(event);
}
