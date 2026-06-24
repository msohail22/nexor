# Nexor

Nexor is a C++ project organized around a small set of clear subsystems: core data types, in-memory data structures, the event loop, networking, commands, persistence, replication, clustering, modules, and memory management.

## Layout

- `src/core`: shared types, encoding, strings, and object helpers
- `src/ds`: data structures such as lists, dicts, intsets, listpacks, and quicklists
- `src/event`: event loop and platform backends
- `src/net`: sockets, connections, listeners, and server code
- `src/cmd`: command registry, command implementations, and replies
- `src/persist`: persistence helpers for AOF, RDB, and I/O
- `src/repl`: replication state and backlog logic
- `src/cluster`: cluster, gossip, node, and slot mapping code
- `src/module`: module loading and API surface
- `src/memory`: allocation, pooling, and memory stats
- `test`: focused tests for each subsystem

## Build

The project uses CMake.

```bash
cmake -S . -B build
cmake --build build
ctest --test-dir build
```

## Architecture

See [architecture.md](architecture.md) for a simple visual map of the codebase.
