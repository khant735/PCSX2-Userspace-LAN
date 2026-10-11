// Experimental userspace DEV9 Ethernet backend for Linux/POSIX.
// SPDX-License-Identifier: GPL-3.0-or-later
#pragma once
#if defined(__POSIX__)
#include "net.h"
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <fcntl.h>
#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <cstddef>
#include <string>
#include <cstdio>

class UserspaceLanAdapter final : public NetAdapter
{
  int fd = -1;
  std::string local;
  static bool address(const char* path, sockaddr_un& out)
  {
    if (!path || !*path || strlen(path) >= sizeof(out.sun_path)) return false;
    memset(&out, 0, sizeof(out));
    out.sun_family = AF_UNIX;
    strcpy(out.sun_path, path);
    return true;
  }
public:
  UserspaceLanAdapter()
  {
    const char* remote = getenv("PCSX2_USERSPACE_LAN_SOCKET");
    const char* localPath = getenv("PCSX2_USERSPACE_LAN_LOCAL");
    sockaddr_un r{}, l{};
    if (!address(remote,r) || !address(localPath,l)) return;
    local = localPath;
    fd = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (fd < 0) return;
    // macOS lacks Linux's SOCK_NONBLOCK and SOCK_CLOEXEC socket() flags.
    // Configure the same properties portably after creating the socket.
    const int currentFlags = fcntl(fd, F_GETFL, 0);
    const int descriptorFlags = fcntl(fd, F_GETFD, 0);
    if (currentFlags < 0 || descriptorFlags < 0 ||
        fcntl(fd, F_SETFL, currentFlags | O_NONBLOCK) < 0 ||
        fcntl(fd, F_SETFD, descriptorFlags | FD_CLOEXEC) < 0)
    {
      ::close(fd);
      fd = -1;
      return;
    }
    unlink(local.c_str());
    if (bind(fd, reinterpret_cast<sockaddr*>(&l), sizeof(l)) != 0 ||
        connect(fd, reinterpret_cast<sockaddr*>(&r), sizeof(r)) != 0)
    {
      ::close(fd); fd = -1; unlink(local.c_str()); return;
    }
    // Register this endpoint before the first Ethernet packet.
    const unsigned char hello[1] = {0};
    ::send(fd, hello, sizeof(hello), MSG_DONTWAIT | MSG_NOSIGNAL);
    // Give each instance a distinct locally administered MAC address.
    const char* instance = getenv("PCSX2_USERSPACE_LAN_INSTANCE");
    if (instance && (*instance == 'A' || *instance == 'B'))
    {
      PacketReader::MAC_Address mac = {{{0x02, 0x04, 0x1f, 0x82, 0x30, static_cast<unsigned char>(*instance == 'A' ? 0x31 : 0x32)}}};
      SetMACAddress(&mac);
    }
    InitInternalServer(nullptr);
  }
  bool blocks() override { return false; }
  bool isInitialised() override { return fd >= 0; }
  bool recv(NetPacket* pkt) override
  {
    if (NetAdapter::recv(pkt)) return true;
    if (fd < 0) return false;
    ssize_t n = ::recv(fd, pkt->buffer, sizeof(pkt->buffer), MSG_DONTWAIT | MSG_TRUNC);
    if (n >= 0) {
      std::fprintf(stderr, "USERSPACE_DEV9_RX raw=%zd hdr=", n);
      for (int i=0;i<static_cast<int>(n) && i<20;i++) std::fprintf(stderr,"%02x",static_cast<unsigned char>(pkt->buffer[i]));
      std::fprintf(stderr,"\n");
    }
    if (n < 14 || n > 1514 || n > static_cast<ssize_t>(sizeof(pkt->buffer))) return false;
    pkt->size = static_cast<int>(n);
    if (!VerifyPkt(pkt, pkt->size)) { std::fprintf(stderr, "USERSPACE_DEV9_RX verify_failed size=%d\n", pkt->size); return false; }
    std::fprintf(stderr, "USERSPACE_DEV9_RX accepted size=%d\n", pkt->size);
    InspectRecv(pkt);
    return true;
  }
  bool send(NetPacket* pkt) override
  {
    // SMAP driver adapter-detection diagnostic, not an Ethernet frame.
    if (pkt && pkt->size == 0x5EA) {
      bool diagnostic = true;
      for (int i = 0; i < 0x5E8; i += 4) {
        const unsigned int word = static_cast<unsigned char>(pkt->buffer[i])
          | (static_cast<unsigned int>(static_cast<unsigned char>(pkt->buffer[i+1])) << 8)
          | (static_cast<unsigned int>(static_cast<unsigned char>(pkt->buffer[i+2])) << 16)
          | (static_cast<unsigned int>(static_cast<unsigned char>(pkt->buffer[i+3])) << 24);
        if (word != static_cast<unsigned int>(i)) { diagnostic = false; break; }
      }
      if (diagnostic) {
        std::fprintf(stderr, "USERSPACE_DEV9_TX skipped_smap_adapter_diagnostic size=%d\n", pkt->size);
        return true;
      }
    }
    InspectSend(pkt);
    if (NetAdapter::send(pkt)) return true;
    if (fd < 0 || pkt->size < 14 || pkt->size > 1514) return false;
    const ssize_t sent = ::send(fd, pkt->buffer, pkt->size, MSG_DONTWAIT | MSG_NOSIGNAL);
    std::fprintf(stderr, "USERSPACE_DEV9_TX size=%d sent=%zd hdr=", pkt->size, sent);
    for (int i=0;i<pkt->size && i<20;i++) std::fprintf(stderr,"%02x",static_cast<unsigned char>(pkt->buffer[i]));
    std::fprintf(stderr,"\n");
    return sent == pkt->size;
  }
  void reloadSettings() override { ReloadInternalServer(nullptr); }
  ~UserspaceLanAdapter() override { if (fd >= 0) ::close(fd); if (!local.empty()) unlink(local.c_str()); }
};
#endif
