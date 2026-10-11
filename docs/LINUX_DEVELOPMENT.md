# Linux development build: setup and verification

This is an **experimental** PCSX2 Linux x86-64 build. The release ZIP contains a development executable, not a self-contained AppImage. Exact runtime library versions depend on the build host and should be inspected rather than guessed.

## Prepare

1. Download the Linux ZIP from the [fork's Releases](https://github.com/khant735/PCSX2-Userspace-LAN/releases).
2. Extract it in a fresh directory. Avoid running an untrusted executable as root.
3. Ensure Python 3 is installed for `tools/userspace-lan/switch.py`.
4. Supply a legally dumped BIOS separately; no BIOS is included.
5. Use separate PCSX2 configuration/data directories for the two instances.

Example (Ubuntu/Debian; package availability varies by distribution):

```bash
sudo apt update
sudo apt install python3 unzip libgl1 libegl1 libx11-6 libxcb1 libxkbcommon-x11-0 libpulse0
```

This is a **starting set**, not a complete dependency manifest. Qt, KDDockWidgets, SDL and other libraries may also be needed. Use the following checks to find the precise missing packages for your distribution.

## Inspect the extracted executable

```bash
file ./pcsx2-qt
sha256sum ./pcsx2-qt
ldd ./pcsx2-qt
```

If `ldd` reports `not found`, install compatible libraries from your distribution or use a matching runtime environment. Do not download arbitrary shared libraries from unofficial websites. Check that the executable is the expected Linux ELF before executing it.

## Start a two-instance LAN

See the [main README](../README.md#getting-started-linux-userspace-lan) for the switch and environment-variable commands. Each emulator must have DEV9 Ethernet enabled and an appropriate IP address in its PS2 application.

For diagnostics, save separate emulator logs and the switch output. Do not confuse a successful Ethernet exchange with successful UDP discovery or TCP application-state synchronization.

## Building from source

This fork retains the PCSX2 CMake build system and upstream submodules. Start with [upstream contribution/build documentation](https://pcsx2.net/docs/contributing/) and the `CMakeLists.txt` and `CMakePresets.json` in this repository. A clean CI build must be completed before this fork's binary/source correspondence can be independently verified.

## Release provenance and limitations

The existing development release ZIP was produced outside GitHub Actions. It has not been proven to be reproducible byte-for-byte from the current `master` branch. The published release asset should therefore be treated as an experimental development binary, not an independently verified CI build. Do not replace an existing asset without documenting the change and recording a new checksum.

The emulator and this fork remain subject to upstream PCSX2 copyright and third-party licence notices.
