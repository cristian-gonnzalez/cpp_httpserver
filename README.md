# C++ HTTP Server (Modern C++20 Example)

This repository contains a **minimal HTTP server written in C++**, designed as a
**focused example to explore modern C++20 features and clean code structure**.

The project demonstrates how a small, self-contained codebase can be organized
using modern C++ practices while implementing a simple HTTP / REST-style protocol
from scratch.

The implementation is intentionally scoped and minimal, prioritizing
**readability, structure, and design clarity** over full HTTP compliance or
production-level performance.

---

## Goals

- Apply modern C++ (C++20) language features in a concrete example
- Design a small but well-structured native server
- Implement a minimal HTTP and REST-style protocol
- Keep the code easy to reason about and extend

---

## REST Protocol

The server implements a **small REST-style protocol** intended as a
demonstrative example.

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

