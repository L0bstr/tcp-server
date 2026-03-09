# Tcp server

An evolving TCP server in C.

---

## Dependencies
- C compiler
- [CMake](https://cmake.org/)

---

## Planned
- [ ] Core
- [ ] HTTP parsing
- [ ] Multiplexing
    - [ ] select
    - [ ] poll
    - [ ] epoll
- [ ] Non-blocking sockets

---

## Core server routine

Useful resources:
[socket](https://man7.org/linux/man-pages/man2/socket.2.html),
[address](https://man7.org/linux/man-pages/man3/sockaddr.3type.html),
[bind](https://man7.org/linux/man-pages/man2/bind.2.html),
[listen](https://man7.org/linux/man-pages/man2/listen.2.html),
[accept](https://man7.org/linux/man-pages/man2/accept.2.html),
[recv](https://man7.org/linux/man-pages/man2/recv.2.html),
[send](https://man7.org/linux/man-pages/man2/sendmsg.2.html),
[close](https://man7.org/linux/man-pages/man2/close.2.html)

1. Create a `socket` and an `address` object for the server
2. `bind` the server `address` to the server `socket`
3. `listen` for connections on the server `socket`
5. `accept` a connection and recive the `socket` and `address` for the client
6. `recv` client request
7. `send` response
8. `close` the client and finally the server `socket`

## Build and run
```bash
cmake -B build
cmake --build build
./build/tcp-server
```
