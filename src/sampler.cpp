#include "sampler.h"
#include <fstream>
#include <sstream>
#include <filesystem>
#include <cstdio>

namespace fs = std::filesystem;

bool Sampler::init(const std::string& hwmon_name) {
    // Discover hwmon path by scanning /sys/class/hwmon/*/name
    for (auto& entry : fs::directory_iterator("/sys/class/hwmon")) {
        std::ifstream nf(entry.path() / "name");
        std::string name;
        if (nf && std::getline(nf, name) && name == hwmon_name) {
            temp_path_ = entry.path() / "temp1_input";
            break;
        }
    }
    if (temp_path_.empty()) {
        fprintf(stderr, "sysmon: hwmon '%s' not found\n", hwmon_name.c_str());
        return false;
    }

    // Take initial CPU reading
    prev_ = read_cpu_jiffies();
    first_ = true;
    return true;
}

CpuJiffies Sampler::read_cpu_jiffies() {
    CpuJiffies j;
    std::ifstream f("/proc/stat");
    std::string tag;
    uint64_t user, nice, system, idle, iowait, irq, softirq, steal;
    f >> tag >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal;
    j.active = user + nice + system + irq + softirq + steal;
    j.total = j.active + idle + iowait;
    return j;
}

double Sampler::read_mem_pct() {
    std::ifstream f("/proc/meminfo");
    std::string line;
    uint64_t total = 0, available = 0;
    while (std::getline(f, line)) {
        uint64_t val;
        if (sscanf(line.c_str(), "MemTotal: %lu kB", &val) == 1) total = val;
        else if (sscanf(line.c_str(), "MemAvailable: %lu kB", &val) == 1) available = val;
        if (total && available) break;
    }
    if (total == 0) return 0;
    return static_cast<double>(total - available) / total * 100.0;
}

double Sampler::read_temp() {
    std::ifstream f(temp_path_);
    int millideg = 0;
    f >> millideg;
    return millideg / 1000.0;
}

Sample Sampler::read() {
    Sample s;

    auto cur = read_cpu_jiffies();
    if (!first_) {
        uint64_t d_active = cur.active - prev_.active;
        uint64_t d_total = cur.total - prev_.total;
        s.cpu_pct = d_total > 0 ? static_cast<double>(d_active) / d_total * 100.0 : 0;
    }
    prev_ = cur;
    first_ = false;

    s.mem_pct = read_mem_pct();
    s.temp_c = read_temp();
    return s;
}
