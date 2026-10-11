# PCSX2 Userspace LAN

An experimental fork of [PCSX2](https://github.com/PCSX2/pcsx2), focused on extending PlayStation 2 DEV9 networking with a userspace virtual Ethernet backend.

## Project Goals

- Enable local network communication between multiple PCSX2 emulator instances.
- Provide a virtual Ethernet transport without requiring TAP/TUN devices, kernel network interfaces or elevated privileges.
- Use Unix-domain sockets and a userspace Ethernet switch to exchange virtual network frames.
- Support development, debugging and testing of PlayStation 2 LAN-enabled applications.
- Maintain compatibility with upstream PCSX2 wherever practical.

## Development Status

**Experimental — work in progress.**

The custom DEV9 networking modifications are under development. The modified emulator has not yet completed compilation and end-to-end LAN communication testing.

The initial target is Linux x86-64, using PCSX2 v2.9.31 as the development baseline.

## Acknowledgements

Based on the open-source PCSX2 project. This is an independent, unofficial fork and is not affiliated with or endorsed by the PCSX2 development team.

Original project: https://github.com/PCSX2/pcsx2

The original PCSX2 copyright notices, licensing requirements and applicable third-party acknowledgements are retained.
