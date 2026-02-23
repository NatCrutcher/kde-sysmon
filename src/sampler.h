#pragma once
#include <string>
#include <cstdint>

struct CpuJiffies {
    uint64_t active = 0;
    uint64_t total = 0;
};

struct Sample {
    double cpu_pct = 0;
    double mem_pct = 0;
    double temp_c = 0;
};

class Sampler {
public:
    bool init(const std::string& hwmon_name);
    Sample read();

private:
    CpuJiffies read_cpu_jiffies();
    double read_mem_pct();
    double read_temp();

    std::string temp_path_;
    CpuJiffies prev_;
    bool first_ = true;
};
