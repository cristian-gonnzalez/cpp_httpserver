# C++ HTTP Server (C++20 Learning Project)

This repository contains a **simple HTTP server written in C++**, started as a
**learning and experimentation project while studying modern C++20 features**.

The goal of the project was to practice:
- Using modern C++ language features
- Designing a small but structured codebase
- Implementing a minimal HTTP server and REST-style protocol from scratch

The implementation prioritizes clarity and learning over completeness,
performance, or full HTTP compliance.

---

## Features

- Minimal HTTP server implementation
- Simple REST-style protocol
- Command-based request handling
- Modular architecture (server, protocol, HTTP, logging)
- Basic logging infrastructure
- Example command (`add`)
- Use of modern C++ (C++20) features where appropriate

---

## REST Protocol

The server implements a **very small REST-style protocol**, designed purely for
learning and experimentation.

Current functionality includes:
- Basic request parsing
- Protocol dispatch
- A simple `add` command

The protocol is intentionally minimal and easy to extend.

---

## Building

From the `make/` directory, run:

```bash
make
```
This compiles the server using C++20 and produces the binary at:

```bash
./target/app

```

You should see debug output indicating that the server and thread pool
have started.

## Running

Start the server:
```bash
./target/app
```
You should see debug output indicating that the server and thread pool
have started.


## Testing

Open a new terminal and run the provided test script:

```bash
cd test
python server_test.py
```

Stop the server using CTRL+C.

The server performs a clean shutdown and logs the shutdown sequence.

