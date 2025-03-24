#ifndef CORE_EVENTS_HPP
#define CORE_EVENTS_HPP

#include <functional>
#include <optional>
#include <string>
namespace core::events {
class Event {
public:
    bool cancelled = false;
    Event();
    virtual ~Event() =0;
};

template<typename T>
class EventQueue {
public:
    typedef std::function<void(T&)> receiver;
private:
    std::vector<std::pair<receiver, std::string>> receivers {};
    EventQueue<Event>* parent;
public:
    int listeners() {return receivers.size();}
    bool hasListener(std::string name, bool propagate = false) {
        for (auto p : receivers) {
            if (p.second == name) return true;
        }
        return propagate && parent != nullptr && parent->hasListener(name, true);
    }
    // TODO: registerBefore
    void registerHandler(receiver fn, std::string name, int pos = -1) {
        std::pair<receiver, std::string> rcv = std::pair(fn, name);
        if (pos != -1) receivers.insert(receivers.begin() + pos, rcv);
        else receivers.push_back(rcv);
    }
    void fireEvent(T& event) {
        for (auto rcv : receivers) {
            rcv.first(event);
            if (event.cancelled) return;
        }
        if (parent) parent->fireEvent(event);
    }
    
    template<typename U = Event>
    EventQueue(EventQueue<U>* parent_ = nullptr) {
        static_assert(std::is_base_of<U, T>::value);
        parent = parent_;
    }
};

extern EventQueue<Event> rootQueue;
}

#endif
