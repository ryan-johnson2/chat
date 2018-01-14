#!/usr/bin/env python
import socket
import thread
import sys

def main():
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.connect(("192.168.0.115", 8080))
    
    t1 = thread.start_new_thread(recv_msg, (s,))
    t2 = thread.start_new_thread(send_msg, (s,))
    
    while (1):
        pass

def recv_msg(s):
    while(1):
        data = s.recv(512)
        sys.stdout.flush()
        print data

def send_msg(s):
    while(1):
        data = raw_input("send> ")
        s.send(data)


if __name__ == "__main__":
    main()
