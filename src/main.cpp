#include "config.h"
#include "daemon.h"
#include "query.h"
#include <cstdio>
#include <cstring>
#include <string>

static void usage() {
    fprintf(stderr,
        "Usage: kde-sysmon [OPTIONS]\n"
        "\n"
        "Modes:\n"
        "  --daemon       Run as sampling daemon\n"
        "  --display      Print human-readable status line\n"
        "  --query        Print machine-readable status\n"
        "\n"
        "Daemon options:\n"
        "  --interval N   Sample interval in seconds (default: 0.1)\n"
        "  --window N     Averaging window in seconds (default: 1.0)\n"
        "  --hwmon NAME   hwmon sensor name (default: k10temp)\n"
    );
}

int main(int argc, char* argv[]) {
    enum Mode { NONE, DAEMON, DISPLAY, QUERY } mode = NONE;

    Config cfg;

    // Load config file if it exists
    const char* home = getenv("HOME");
    if (home) {
        cfg.load_file(std::string(home) + "/.config/kde-sysmon/kde-sysmon.conf");
    }

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--daemon") == 0) {
            mode = DAEMON;
        } else if (strcmp(argv[i], "--display") == 0) {
            mode = DISPLAY;
        } else if (strcmp(argv[i], "--query") == 0) {
            mode = QUERY;
        } else if (strcmp(argv[i], "--interval") == 0 && i + 1 < argc) {
            cfg.sample_interval = std::stod(argv[++i]);
        } else if (strcmp(argv[i], "--window") == 0 && i + 1 < argc) {
            cfg.average_window = std::stod(argv[++i]);
        } else if (strcmp(argv[i], "--hwmon") == 0 && i + 1 < argc) {
            cfg.hwmon_name = argv[++i];
        } else if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0) {
            usage();
            return 0;
        } else {
            fprintf(stderr, "kde-sysmon: unknown option '%s'\n", argv[i]);
            usage();
            return 1;
        }
    }

    cfg.init_run_dir();

    switch (mode) {
    case DAEMON:
        return run_daemon(cfg);
    case DISPLAY:
        return cmd_display(cfg.run_dir);
    case QUERY:
        return cmd_query(cfg.run_dir);
    case NONE:
        usage();
        return 1;
    }
    return 0;
}
