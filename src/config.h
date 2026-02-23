#pragma once
#include <string>

struct Config {
    double sample_interval = 0.1;   // seconds
    double average_window = 1.0;    // seconds
    std::string hwmon_name = "k10temp";
    std::string run_dir;            // /run/user/$UID/sysmon

    void load_file(const std::string& path);
    void init_run_dir();
    int ring_size() const;
};
