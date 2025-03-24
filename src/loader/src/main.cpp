#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>
#include <toml++/toml.hpp>
#include <loader/app.hpp>
#include <unistd.h>
#include <dlfcn.h>
#include <vector>

typedef void(*void_fn)();

int main() {
    // locate the config file
    char* cfg_path_str = std::getenv("XDG_CONFIG_HOME");
    if (!cfg_path_str) {cfg_path_str = std::strcat(std::getenv("HOME"), "/.config");}
    cfg_path_str = std::strcat(cfg_path_str, "/nabla/nabla.toml");
    std::cout << "loading " << cfg_path_str << "\n";
    std::ifstream f = std::ifstream(cfg_path_str);
    toml::table file {};
    if (f) {
        std::stringstream ss {};
        ss << f.rdbuf();
        file = toml::parse(ss);
    }

    // merge environment variables
    char** env = environ;
    std::stringstream ss {};
    while (*env) {
        std::string var = std::string(*env);
        int eq_idx = var.find("=");
        std::string name = var.substr(0, eq_idx);
        if (name.starts_with("NABLA_")) {
            name = std::regex_replace(name.substr(6, name.size()), std::regex("__"), ".");
            std::string value = var.substr(eq_idx + 1, var.size());
            std::string full = name + " = " + value;
            ss << full << '\n';
        }
        env++;
    }

    toml::table local_file = toml::parse(ss);
    for (auto kv : local_file) {
        file.insert_or_assign(kv.first, kv.second);
    }

    std::cout << "config keys:";
    for (auto x : file) {
        std::cout << ' ' << x.first;
    }
    std::cout << '\n';

    loader::App app {file};

    auto module_search_paths = app.arrayOrDefault("loader.search_path", std::vector<std::string>({"a"}));
    
    std::cout << "Module search path:";
    for (auto path : module_search_paths) {
        std::cout << ' ' << path;
    }
    std::cout << '\n';

    auto modules = app.arrayOrDefault("loader.modules", std::vector<std::string>({}));

    std::cout << "Modules:";
    for (auto module : modules) std::cout << ' ' << module;
    std::cout << '\n';

    std::vector<void*> module_handles {};
    typedef void(*init_t)(loader::App&);

    for (auto module : modules) {
        std::cout << "Loading module `" << module << "`\n";
        bool loaded = false;
        for (auto path : module_search_paths) {
            std::string filepath = path + '/' + module + ".so";
            dlerror();
            void* handle = dlopen(filepath.c_str(), RTLD_LAZY | RTLD_GLOBAL);
            if (handle) {
                init_t preinit = (init_t)dlsym(handle, "preinit");

                if (preinit) {
                    preinit(app);
                }
                module_handles.push_back(handle);
                loaded = true;
                break;
            } else {
                char* error = dlerror();
                if (error) std::cout << "Failed to load from `" << filepath << "` with error: " << error << '\n';
                else std::cout << "Failed to load from `" << filepath << "` with unknown error\n";
            }
        }
        if (!loaded) {
            std::cout << "Failed to find module `" << module << "`, continuing regardless\n";
        }
    }

    // init modules
    for (auto handle : module_handles) {
        init_t init = (init_t)dlsym(handle, "init");
        if (init) init(app);
    }

    return 0;
}
