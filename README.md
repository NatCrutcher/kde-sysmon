# kde-sysmon

Lightweight system monitor daemon + CLI for Linux. Continuously samples CPU%, memory% (excluding cache), and CPU temperature, maintains rolling averages, and exposes the result for KDE's Command Output widget.

## Build

```sh
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

## Install

```sh
cp build/kde-sysmon ~/.local/bin/
cp systemd/kde-sysmon.service ~/.config/systemd/user/
systemctl --user daemon-reload
systemctl --user enable --now kde-sysmon
```

## Update

After rebuilding, stop the daemon before replacing the binary:

```sh
systemctl --user stop kde-sysmon
cp build/kde-sysmon ~/.local/bin/
systemctl --user start kde-sysmon
```

## Usage

```sh
# Run daemon in foreground (for testing)
kde-sysmon --daemon

# Human-readable output (for Command Output widget)
kde-sysmon --display
# → CPU 12.3% | RAM 18.7% | 35.1C

# Machine-readable output
kde-sysmon --query
# → cpu=12.3
#   mem=18.7
#   temp=35.1
#   timestamp=1708646400.123
```

## Configuration

Optional config file: `~/.config/kde-sysmon/kde-sysmon.conf`

```ini
sample_interval=0.1
average_window=1.0
# hwmon_name=k10temp
```

CLI overrides:

```sh
kde-sysmon --daemon --interval 0.2 --window 2.0 --hwmon k10temp
```

## KDE Command Output Widget

1. Add a "Command Output" widget to your panel
2. Set command to: `cat /run/user/1000/kde-sysmon/display`
3. Set interval to 1 second
