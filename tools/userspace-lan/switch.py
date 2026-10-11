#!/usr/bin/env python3
"""Unprivileged Unix-datagram Ethernet switch for PCSX2 backend development.
Protocol: each endpoint sends raw Ethernet frames to switch socket; switch sends
frames to the endpoint's registered Unix socket path. First packet registers port.
"""
import argparse, os, selectors, socket, struct, signal, time

def main():
 p=argparse.ArgumentParser();p.add_argument('--socket',default='/tmp/ps2-ether-switch.sock');p.add_argument('--idle',type=float,default=0);a=p.parse_args()
 if os.path.exists(a.socket): raise SystemExit('Socket already exists: '+a.socket)
 s=socket.socket(socket.AF_UNIX,socket.SOCK_DGRAM);s.bind(a.socket);s.settimeout(.5)
 peers={}; macs={}; start=time.monotonic(); count=0
 print('Switch listening',a.socket,flush=True)
 try:
  while True:
   if a.idle and time.monotonic()-start>a.idle:break
   try:packet,addr=s.recvfrom(65535)
   except socket.timeout:continue
   if not addr:continue
   if packet == b"\x00":
    peers[addr]=time.monotonic()
    print("registered",addr,flush=True)
    continue
   if len(packet)<14:continue
   peers[addr]=time.monotonic();dst=packet[:6];src=packet[6:12];macs[src]=addr
   targets=[macs[dst]] if dst in macs and dst != b'\xff'*6 else [x for x in peers if x!=addr]
   if dst not in macs:targets=[x for x in peers if x!=addr]
   for target in targets:
    if target==addr:continue
    try:s.sendto(packet,target);count+=1
    except OSError as exc:print('delivery failed',target,exc,flush=True);peers.pop(target,None)
   print('frame',src.hex(':'),'->',dst.hex(':'),'bytes',len(packet),'forwarded',len(targets),flush=True)
 finally:
  s.close();os.unlink(a.socket);print('forwarded total',count,flush=True)
if __name__=='__main__':main()
