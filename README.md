# http-server-c

A small HTTP server written in C, built from scratch as a learning project.

I wanted to understand what happens under the hood when a browser talks to a web server, so this project uses only the C standard library and the POSIX socket API (`socket`, `bind`, `listen`, `accept`, `read`, `write`). No frameworks. Developed and tested on macOS.

## Features

- Listens on `127.0.0.1:8080`
- Accepts `GET` and `POST` requests
- `GET` returns a fixed greeting
- `POST` with a body like `name=lukas` replies with `Hello lukas`

## Build and run

```sh
clang -Wall main.c -o server && ./server
```

## Try it

In a second terminal:

```sh
curl -i http://localhost:8080/hello
curl -i -X POST -d "name=lukas" http://localhost:8080/hello
```

## Known limitations

This is a learning project, not a production server.

- Handles one connection at a time (no threads, `fork` or `poll`)
- Reads the request with a single `read()` into a 4 KB buffer, so large requests or bodies that arrive in several packets are not handled
- `Content-Length` is not evaluated yet
- A malformed `POST` currently terminates the whole server instead of returning a `400` response
- Responses have no `Content-Length` header and the connection is closed after each reply
- Only the HTTP method is checked, the path is ignored

## What I learned

- How a request is structured: request line, headers, empty line (`\r\n\r\n`), body
- The socket lifecycle: `socket` → `bind` → `listen` → `accept` → `read`/`write` → `close`
- Byte order (`htons`, `htonl`) and why a `sockaddr_in` needs it
