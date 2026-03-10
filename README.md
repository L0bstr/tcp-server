# Tcp server

An evolving TCP server in C.

---

## Dependencies
- C compiler
- Build system (recommended: make)
- [CMake](https://cmake.org/)

> [!WARNING]
> The source code uses [POSIX](https://hu.wikipedia.org/wiki/POSIX) headers: Not Windows compatible

---

## Vendor
- [picohttpparser](https://github.com/h2o/picohttpparser)

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

---

> [!TIP]
> Start the server with
> - `--ip 127.0.0.1` to accept connections from localhost only
> - `--ip 0.0.0.0` to accept connections from any interface (LAN + internet)
> - `--ip 192.168.1.100` to bind to a specific network interface (e.g. LAN only, replace with your machine's actual IP)
> <br>
> Run the server with ip that includes localhost and open http://127.0.0.1:3000/

## Build and Run
```bash
cmake -B build
cmake --build build
./build/tcp-server --ip 127.0.0.1 --port 3000
```

---

## Dockerize
### Build
```bash
docker build -t "tcp-server" .
```

### Run
```bash
docker run --rm -d --name tcp-server -p 3000:3000 tcp-server:latest
```

### Stop
```bash
docker stop tcp-server
```
