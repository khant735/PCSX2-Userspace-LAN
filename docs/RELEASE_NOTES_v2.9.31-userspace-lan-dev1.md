# PCSX2 v2.9.31 – Userspace LAN (Linux x64) – Development Build 1

**Experimental pre-release.** This is an unofficial Linux x86-64 fork of PCSX2 v2.9.31, adding a userspace Ethernet backend for DEV9 networking.

## Added features

- Unprivileged DEV9 virtual Ethernet adapter using Unix-domain datagram sockets.
- Local Python Ethernet switch; no TAP/TUN interface or root access required.
- Separate instance A/B MAC addresses and socket paths.
- SMAP RX frame-count diagnostic correction and receive-pointer tracing.

## Test results (11 October 2026)

Two PCSX2 instances running the PS2 MultiScreen diagnostic application demonstrated bidirectional Ethernet forwarding, UDP peer discovery, TCP connections and successful parsing of `S1` synchronization messages (162 received per instance in the recorded test).

Dynamic controller/player movement synchronization and compatibility with commercial PS2 LAN games **have not been established**.

## Requirements

Linux x86-64, Python 3, compatible runtime libraries, a legally obtained PS2 BIOS and a network-capable PS2 application. This ZIP is **not a standalone AppImage**.

See the [README](https://github.com/khant735/PCSX2-Userspace-LAN/blob/master/README.md) and [Linux development setup guide](https://github.com/khant735/PCSX2-Userspace-LAN/blob/master/docs/LINUX_DEVELOPMENT.md).

## Known limitations

- Experimental two-instance A/B configuration; broader MAC assignment and multi-peer support remain unfinished.
- Verbose DEV9 diagnostic logging.
- Runtime dependencies vary by Linux distribution.
- The existing release executable was built outside GitHub Actions and is not independently proven reproducible from the latest repository commits.

## Credits and licence

Based on [PCSX2](https://github.com/PCSX2/pcsx2), developed by the PCSX2 Development Team and contributors. This is an independent, unofficial fork, not endorsed by upstream PCSX2. Original copyright and applicable GPL v3-or-later / third-party licence requirements remain in force.

No PS2 BIOS files, proprietary Sony firmware or commercial game images are included.
