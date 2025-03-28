#include <loader/app.hpp>

namespace loader {
App::App(toml::table table_) : table{table_} {}
App::~App() {}
}  // namespace loader
