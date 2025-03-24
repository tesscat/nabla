#ifndef LOADER_APP_HPP
#define LOADER_APP_HPP

#include <optional>
#include <shared_mutex>
#include <string_view>
#include <toml++/toml.hpp>

namespace loader {
struct App {
    std::shared_mutex rwlock;
    toml::table table;
public:
    App(toml::table table);
    ~App();

    template<typename T>
    T valueOrDefault(std::string_view path, T defaultValue) {
        rwlock.lock_shared();
        std::optional<T> node = table.at_path(path).value<T>();
        rwlock.unlock_shared();
        std::optional<T> value = node.or_else([&]() {
            // node doesn't exist
            rwlock.lock();
            table.insert_or_assign(path, defaultValue);
            rwlock.unlock();
            return std::optional(defaultValue);
        });
        return value.value();
    }

    template<typename T>
    std::vector<T> arrayOrDefault(std::string_view path, std::vector<T> defaultValue) {
        rwlock.lock_shared();
        toml::array* array = table.at_path(path).as_array();
        rwlock.unlock_shared();
        if (array) {
            std::vector<T> output {};
            for (auto& node : *array) {
                std::optional<T> as_type = node.value<T>();
                if (as_type) {
                    output.push_back(as_type.value());
                }
            }
            return output;
        } else {
            rwlock.lock();
            toml::array arr {};
            arr.insert(arr.begin(), defaultValue.begin(), defaultValue.end());
            table.insert_or_assign(path, arr);
            rwlock.unlock();
            return defaultValue;
        }
    }
};
}

#endif
