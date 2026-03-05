# Tcp server

The core understaning of how a tcp server works in c.

## Server workflow

Useful resources:
[socket](https://man7.org/linux/man-pages/man2/socket.2.html)
[address](https://man7.org/linux/man-pages/man3/sockaddr.3type.html)
[bind](https://man7.org/linux/man-pages/man2/bind.2.html)
[listen](https://man7.org/linux/man-pages/man2/listen.2.html)
[select](https://man7.org/linux/man-pages/man2/select.2.html)
[poll](https://man7.org/linux/man-pages/man2/poll.2.html)
[epoll](https://man7.org/linux/man-pages/man7/epoll.7.html)
[accept](https://man7.org/linux/man-pages/man2/accept.2.html)
[read](https://man7.org/linux/man-pages/man2/read.2.html)
[send](https://man7.org/linux/man-pages/man2/sendmsg.2.html)
[close](https://man7.org/linux/man-pages/man2/close.2.html)

1. Create a `socket` and an `address` object for the server.
2. `bind` the `address` to the `socket`
3. Set the `socket` as `listen`ing
4. Multiplexing (optional)
    - Handle the ready sockets which are `select`ed into `fd_set`. (limited to `FD_SETSIZE`)
    - `poll` for a new event, then scan for the ready sockets.
    - Handle the ready sockets from an array maintained by the kernel via `epoll`. (linux only)
5. `accept` a connection and recive the `socket` and `address` for the client
6. `read` client message.
7. `send` response.
8. `close` the client and finally the server socket.

> [!NOTE]
> Start the server and test it on [http://127.0.0.1:3000](http://127.0.0.1:3000)

## Build and Run
```bash
make
./build/server --port 3000
```

## Dockerize
### Build
```bash
docker build -t "tcp-server" .
```

### Run
```bash
docker run --rm -d --name tcp-server -p 3000:3000 tcp-server:latest
```
