#include "query.h"
#include <fstream>
#include <iostream>
#include <string>

static int read_and_print(const std::string& path) {
    std::ifstream f(path);
    if (!f) {
        fprintf(stderr, "kde-sysmon: cannot read %s (daemon running?)\n", path.c_str());
        return 1;
    }
    std::string line;
    while (std::getline(f, line)) {
        std::cout << line << '\n';
    }
    return 0;
}

int cmd_display(const std::string& run_dir) {
    return read_and_print(run_dir + "/display");
}

int cmd_query(const std::string& run_dir) {
    return read_and_print(run_dir + "/status");
}
