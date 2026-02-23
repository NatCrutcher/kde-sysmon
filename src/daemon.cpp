#include "daemon.h"
#include "sampler.h"
#include "averager.h"
#include "writer.h"
#include <cstdio>
#include <csignal>
#include <ctime>
#include <chrono>

static volatile sig_atomic_t g_running = 1;
static volatile sig_atomic_t g_reload = 0;

static void on_term(int) { g_running = 0; }
static void on_hup(int) { g_reload = 1; }

int run_daemon(Config& cfg) {
    signal(SIGTERM, on_term);
    signal(SIGINT, on_term);
    signal(SIGHUP, on_hup);

    Sampler sampler;
    if (!sampler.init(cfg.hwmon_name)) return 1;

    int n = cfg.ring_size();
    Averager avg_cpu(n), avg_mem(n), avg_temp(n);

    long interval_ns = static_cast<long>(cfg.sample_interval * 1e9);
    struct timespec next;
    clock_gettime(CLOCK_MONOTONIC, &next);

    std::string status_path = cfg.run_dir + "/status";
    std::string display_path = cfg.run_dir + "/display";

    fprintf(stderr, "sysmon: daemon started (interval=%.2fs, window=%.2fs, ring=%d)\n",
            cfg.sample_interval, cfg.average_window, n);

    while (g_running) {
        // Advance to next tick
        next.tv_nsec += interval_ns;
        while (next.tv_nsec >= 1000000000L) {
            next.tv_nsec -= 1000000000L;
            next.tv_sec++;
        }
        clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &next, nullptr);

        if (!g_running) break;

        if (g_reload) {
            g_reload = 0;
            // Could reload config here in the future
        }

        auto s = sampler.read();
        avg_cpu.push(s.cpu_pct);
        avg_mem.push(s.mem_pct);
        avg_temp.push(s.temp_c);

        double cpu = avg_cpu.mean();
        double mem = avg_mem.mean();
        double temp = avg_temp.mean();

        // Get timestamp
        auto now = std::chrono::system_clock::now();
        auto epoch = std::chrono::duration<double>(now.time_since_epoch()).count();

        char buf[256];

        // Machine-readable status
        snprintf(buf, sizeof(buf), "cpu=%.1f\nmem=%.1f\ntemp=%.0f\ntimestamp=%.3f\n",
                 cpu, mem, temp, epoch);
        atomic_write(status_path, buf);

        // Human-readable display
        snprintf(buf, sizeof(buf), "CPU %.1f%% | RAM %.1f%% | %.0f°C", cpu, mem, temp);
        atomic_write(display_path, buf);
    }

    // Clean up files
    remove(status_path.c_str());
    remove(display_path.c_str());
    fprintf(stderr, "sysmon: daemon stopped\n");
    return 0;
}
