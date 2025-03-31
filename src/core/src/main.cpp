#include <iostream>
#include "core/events.hpp"
#include "loader/app.hpp"
#include "utils.hpp"

class MyEvent : public core::Event {
public:
    std::string name;
    MyEvent(std::string name_) : name{name_} {}
    ~MyEvent() override {}
};

extern "C" void preinit(loader::App app) {
    UNUSED(app);
    std::cout << "hello from core at preinit!!\n";
}

extern "C" void init(loader::App app) {
    UNUSED(app);
    std::cout << "hello from core at init!!\n";
}
