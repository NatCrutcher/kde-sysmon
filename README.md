# sysmon

Lightweight system monitor daemon + CLI for Linux. Continuously samples CPU%, memory% (excluding cache), and CPU temperature, maintains rolling averages, and exposes the result for KDE's Command Output widget.

## Build

```sh
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

## Install

```sh
cp build/sysmon ~/.local/bin/
cp systemd/sysmon.service ~/.config/systemd/user/
systemctl --user daemon-reload
systemctl --user enable --now sysmon
```

## Usage

```sh
# Run daemon in foreground (for testing)
sysmon --daemon

# Human-readable output (for Command Output widget)
sysmon --display
# → CPU 12.3% | RAM 18.7% | 35.1C

# Machine-readable output
sysmon --query
# → cpu=12.3
#   mem=18.7
#   temp=35.1
#   timestamp=1708646400.123
```

## Configuration

Optional config file: `~/.config/sysmon/sysmon.conf`

```ini
sample_interval=0.1
average_window=1.0
# hwmon_name=k10temp
```

CLI overrides:

```sh
sysmon --daemon --interval 0.2 --window 2.0 --hwmon k10temp
```

## KDE Command Output Widget

1. Add a "Command Output" widget to your panel
2. Set command to: `cat /run/user/1000/sysmon/display`
3. Set interval to 1 second
