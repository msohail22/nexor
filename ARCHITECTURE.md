# Architecture

Nexor is organized as a small set of layers around the main server entrypoint.

```mermaid
flowchart LR
    Main[src/main.cpp] --> Net[src/net]
    Main --> Cmd[src/cmd]
    Main --> Persist[src/persist]
    Main --> Repl[src/repl]
    Main --> Cluster[src/cluster]
    Main --> Module[src/module]
    Main --> Memory[src/memory]

    Net --> Event[src/event]
    Cmd --> Core[src/core]
    Persist --> Core
    Repl --> Net
    Cluster --> Repl
    Module --> Core
    Memory --> Core
    Core --> DS[src/ds]
```

## Layers

- `core` and `ds`: shared utilities and data structures
- `event` and `net`: event loop and networking
- `cmd`: command parsing and replies
- `persist` and `repl`: storage and replication
- `cluster`, `module`, `memory`: distributed, extensibility, and runtime support