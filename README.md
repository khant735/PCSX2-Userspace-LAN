# PCSX2 Userspace LAN

**Experimental Linux x86-64 development fork of [PCSX2](https://github.com/PCSX2/pcsx2), based on PCSX2 v2.9.31.**

This independent fork extends PlayStation 2 DEV9 networking with an unprivileged userspace virtual Ethernet backend. It permits local communication between separate emulator instances over Unix-domain datagram sockets, without kernel TAP/TUN devices, physical network interfaces, or elevated privileges.

> **Unofficial fork / development build.** This project is neither an official PCSX2 release nor endorsed by the PCSX2 development team. The currently tested scope is two instances under Linux; wider game compatibility is not established.

## Project goals

- Enable LAN communication between multiple PCSX2 instances.
- Forward virtual Ethernet frames using a userspace switch.
- Support research, debugging and testing of PS2 homebrew and LAN applications.
- Keep upstream PCSX2 compatibility where practical, without modifying the normal backend unless selected.

## Added components and DEV9 changes

| File | Purpose |
| --- | --- |
| `pcsx2/DEV9/userspace_lan.h` | Experimental userspace DEV9 Ethernet adapter |
| `pcsx2/DEV9/net.cpp` | Selects the additional adapter when the switch environment variable is set |
| `pcsx2/DEV9/smap.cpp` | Corrects misleading RX frame-count warnings and traces RX FIFO pointer updates |
| `tools/userspace-lan/switch.py` | Python Unix-domain datagram Ethernet switch |

The emulator backend accepts raw Ethernet frames, forwards them through the userspace socket transport and receives frames passed by the switch. Current A/B instance profiles use separate virtual MAC addresses. Verbose packet logging is deliberately enabled for development.

## Getting started: Linux userspace LAN

**Requirements:** Linux x86-64, a compiled version of this modified PCSX2 build with compatible runtime libraries, Python 3, a legally dumped PS2 BIOS, and an appropriate PS2 networking application.

1. Start the switch from the repository root:

```bash
python3 tools/userspace-lan/switch.py --socket /tmp/ps2-ether-switch.sock
```

2. Launch instance A using an independent PCSX2 data/configuration directory:

```bash
PCSX2_USERSPACE_LAN_SOCKET=/tmp/ps2-ether-switch.sock \
PCSX2_USERSPACE_LAN_LOCAL=/tmp/pcsx2-ether-a.sock \
PCSX2_USERSPACE_LAN_INSTANCE=A ./pcsx2-qt
```

3. Launch instance B using a *different* data/configuration directory:

```bash
PCSX2_USERSPACE_LAN_SOCKET=/tmp/ps2-ether-switch.sock \
PCSX2_USERSPACE_LAN_LOCAL=/tmp/pcsx2-ether-b.sock \
PCSX2_USERSPACE_LAN_INSTANCE=B ./pcsx2-qt
```

Enable DEV9 Ethernet in both emulator configurations and set suitable in-game/network-application IP addresses. The example assumes the executable is available as `./pcsx2-qt`; if not, change the path to your compiled binary. Use a separate configuration/data path for each instance to avoid conflicting settings.

**Environment variables:**

| Variable | Meaning |
| --- | --- |
| `PCSX2_USERSPACE_LAN_SOCKET` | Shared Unix-domain socket path of the switch |
| `PCSX2_USERSPACE_LAN_LOCAL` | Distinct local socket path for each instance |
| `PCSX2_USERSPACE_LAN_INSTANCE` | Current instance identity: `A` or `B` |

If the userspace switch variable is unset, PCSX2 uses its normally configured DEV9 backend rather than the custom userspace transport.

## Development status and verification

**Experimental; tested on 11 October 2026.** Two emulator instances running the PS2 MultiScreen diagnostic application demonstrated:

| Verification | Status |
| --- | --- |
| Launching two emulators and exchanging Ethernet frames | Passed |
| ARP / IPv4 transport | Observed |
| Bidirectional UDP peer discovery | Passed |
| Bidirectional TCP connection | Passed |
| Parsing `S1` synchronization messages | Passed; 162 messages received at each endpoint in the recorded test |
| Dynamic controller or player movement state | Not yet verified |
| Commercial PS2 game LAN interoperability | Not established |

The synchronized test message values were stationary, so the results do **not** demonstrate interactive movement synchronization. Diagnostic PS2 MultiScreen ELF files are a separate homebrew test application, not part of the PCSX2 emulator distribution.

## Known limitations

- This is a development-quality fork, not a stable general-purpose release.
- Automatic instance-specific MAC configuration currently supports only the A/B test identifiers.
- Verbose DEV9 logs may affect performance or create large output files.
- The switch is designed for local testing; networking security, more peers and hardening need additional work.
- The compiled Linux development ZIP is **not** a self-contained AppImage and may need compatible libraries installed.
- More regression, error-handling, cleanup and PS2-game compatibility testing is required.

## Releases and building

Download any available fork-specific builds from [this fork's Releases page](https://github.com/khant735/PCSX2-Userspace-LAN/releases). Development builds may require additional runtime dependencies; use upstream PCSX2 for routine stable emulation.

For general upstream build and contribution documentation, see [PCSX2's contribution guide](https://pcsx2.net/docs/contributing/). The existing build system, source licences and third-party notice files in this repository remain applicable.

## About PCSX2 (upstream project information)

The following material is retained from the [original PCSX2 README](https://github.com/PCSX2/pcsx2/blob/master/README.md) and describes the **upstream emulator**, not the test status or supported platforms of this experimental fork.

![Windows Build Status](https://img.shields.io/github/actions/workflow/status/PCSX2/pcsx2/windows_build_matrix.yml?label=%F0%9F%96%A5%EF%B8%8F%20Windows%20Builds)
![Linux Build Status](https://img.shields.io/github/actions/workflow/status/PCSX2/pcsx2/linux_build_matrix.yml?label=%F0%9F%90%A7%20Linux%20Builds)
![MacOS Build Status](https://img.shields.io/github/actions/workflow/status/PCSX2/pcsx2/macos_build_matrix.yml?label=%F0%9F%8D%8E%20MacOS%20Builds)
[![Codacy Badge](https://app.codacy.com/project/badge/Grade/1f7c0d75fec74d6daa6adb084e5b4f71)](https://app.codacy.com/gh/PCSX2/pcsx2/dashboard?utm_source=github.com&utm_medium=referral&utm_content=PCSX2/pcsx2&utm_campaign=Badge_Grade)
[![Discord Server](https://img.shields.io/discord/309643527816609793?color=%235CA8FA&label=PCSX2%20Discord&logo=discord&logoColor=white)](https://discord.com/invite/TCz3t9k)

PCSX2 is a free and open-source PlayStation 2 (PS2) emulator. Its purpose is to emulate the PS2's hardware, using a combination of MIPS CPU [Interpreters](<https://en.wikipedia.org/wiki/Interpreter_(computing)>), [Recompilers](https://en.wikipedia.org/wiki/Dynamic_recompilation) and a [Virtual Machine](https://en.wikipedia.org/wiki/Virtual_machine) which manages hardware states and PS2 system memory. This allows you to play PS2 games on your PC, with many additional features and benefits.

## Project Details

PCSX2 has been in development for more than 20 years. Past versions could only run a few public domain game demos, but newer versions can run most games at full speed, including popular titles such as Final Fantasy X and Devil May Cry 3. Visit the [PCSX2 compatibility list](https://pcsx2.net/compat/) to check the latest compatibility status of games (with more than 2500 titles tested).

Installers and binaries for both stable and nightly builds are available from [our website](https://pcsx2.net/downloads/).

## System Requirements

PCSX2 supports Windows, Linux, and Mac platforms. Our [setup documentation page](https://pcsx2.net/docs/setup/requirements) contains additional details on software and hardware requirements.

Please note that a BIOS dump from a legitimately-owned PS2 console is required to use the emulator. For more information, visit [this page](https://pcsx2.net/docs/setup/bios/).

## Contributing / Building

PCSX2 supports translation into other languages using [Crowdin](https://crowdin.com/project/pcsx2-emulator).

See the [Contribution Guide](https://pcsx2.net/docs/contributing/) for more info on how to contribute.

## Credits, licences and relationship to upstream

PCSX2 was created and continues to be maintained by the [PCSX2 Development Team and contributors](https://github.com/PCSX2/pcsx2). This independent repository retains their original copyright notices and relevant third-party acknowledgements. The PCSX2 source is subject to GNU GPL v3 or later and other applicable component licences; see the repository licence and notice files for details.

**Upstream project:** https://github.com/PCSX2/pcsx2  
**Upstream site and downloads:** https://pcsx2.net/  
**Independent fork:** https://github.com/khant735/PCSX2-Userspace-LAN

No PS2 BIOS dump, game ISO or proprietary Sony system software is supplied with this fork.
