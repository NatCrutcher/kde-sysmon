#include "config.h"
#include <fstream>
#include <cstdlib>
#include <cmath>
#include <sys/stat.h>
#include <unistd.h>

void Config::load_file(const std::string& path) {
    std::ifstream f(path);
    if (!f) return;

    std::string line;
    while (std::getline(f, line)) {
        if (line.empty() || line[0] == '#') continue;
        auto eq = line.find('=');
        if (eq == std::string::npos) continue;
        auto key = line.substr(0, eq);
        auto val = line.substr(eq + 1);

        if (key == "sample_interval") sample_interval = std::stod(val);
        else if (key == "average_window") average_window = std::stod(val);
        else if (key == "hwmon_name") hwmon_name = val;
    }
}

void Config::init_run_dir() {
    uid_t uid = getuid();
    run_dir = "/run/user/" + std::to_string(uid) + "/sysmon";
    mkdir(run_dir.c_str(), 0700);
}

int Config::ring_size() const {
    int n = static_cast<int>(std::round(average_window / sample_interval));
    return n < 1 ? 1 : n;
}
