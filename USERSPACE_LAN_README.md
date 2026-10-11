# PCSX2 Userspace LAN (experimental Linux backend)

This fork adds an experimental, unprivileged DEV9 Ethernet transport based on Unix-domain datagram sockets, selected through environment variables. It is derived from PCSX2 v2.9.31 development source.

## Components

- `pcsx2/DEV9/userspace_lan.h`: userspace Ethernet adapter for POSIX/Linux.
- `pcsx2/DEV9/net.cpp`: selects the custom adapter when the switch socket is configured.
- `pcsx2/DEV9/smap.cpp`: recognises valid RX frame-count reads and logs RX FIFO pointer writes for diagnostics.
- `tools/userspace-lan/switch.py`: a simple two-endpoint local Ethernet switch, without TUN/TAP or administrator privileges.

## Launch

Start the switch:

```bash
python3 tools/userspace-lan/switch.py --socket /tmp/ps2-ether-switch.sock
```

Launch two separately configured PCSX2 instances with networking enabled, choosing different local socket paths and instance identifiers:

```bash
PCSX2_USERSPACE_LAN_SOCKET=/tmp/ps2-ether-switch.sock \
PCSX2_USERSPACE_LAN_LOCAL=/tmp/pcsx2-ether-a.sock \
PCSX2_USERSPACE_LAN_INSTANCE=A ./pcsx2-qt
```

```bash
PCSX2_USERSPACE_LAN_SOCKET=/tmp/ps2-ether-switch.sock \
PCSX2_USERSPACE_LAN_LOCAL=/tmp/pcsx2-ether-b.sock \
PCSX2_USERSPACE_LAN_INSTANCE=B ./pcsx2-qt
```

Different PCSX2 configuration/data directories are recommended for the two instances.

## Validation

Tests on 11 October 2026 with a PS2 MultiScreen diagnostic ELF confirmed bidirectional Ethernet forwarding, UDP discovery, TCP connections, and parsing of S1 synchronization messages (162 received at each end in the corresponding test). Dynamic controller state changes remain to be tested.

**Development quality only.** The backend currently prints verbose diagnostic traffic logs; MAC assignment is implemented only for A/B; and error handling, resource cleanup, security, packaging and general compatibility need further review. Do not interpret the PS2 MultiScreen diagnostic application as part of the emulator distribution.

## Distribution

The custom Linux executable is a locally built development binary. Source commits in this repository are not a downloadable GitHub Release asset. A release asset must be uploaded separately. BIOS images, user keys, game ISOs and proprietary PS2 components are not distributed.

PCSX2 code remains subject to its upstream licences and third-party notices. Retain the original PCSX2 credits and licence texts.
