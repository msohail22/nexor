# In-Memory KV Store (C++) — Roadmap

## Phase 0 — Setup
- [ ] Create `kv-store/` with CMake
- [ ] Basic build + run
- [ ] Add logging (stdout)
- [ ] Structure:
  - core/
  - net/
  - storage/
  - persistence/
  - protocol/

---

## Phase 1 — Networking
- [ ] TCP server (listen, accept)
- [ ] Non-blocking sockets
- [ ] epoll event loop
- [ ] Handle multiple clients
- [ ] Connection lifecycle

---

## Phase 2 — Protocol (simple)
- [ ] Define text protocol:
  - `SET key value`
  - `GET key`
  - `DEL key`
- [ ] Parse incoming commands
- [ ] Serialize responses (`OK`, value, errors)

---

## Phase 3 — Core Storage
- [ ] In-memory hash map (key → value)
- [ ] Basic operations:
  - GET
  - SET
  - DEL
- [ ] Thread safety (single-threaded first)

---

## Phase 4 — TTL / Expiration
- [ ] Add expiry metadata per key
- [ ] Lazy expiration on access
- [ ] Periodic cleanup (optional)

---

## Phase 5 — Persistence (choose one first)

### Option A (simpler)
- [ ] Snapshot (dump entire DB to file)
- [ ] Load snapshot on startup

### Option B (better signal)
- [ ] Append-only log (AOF)
- [ ] Replay log on startup

---

## Phase 6 — Memory Management
- [ ] Track memory usage
- [ ] Implement LRU eviction
- [ ] Configurable max memory

---

## Phase 7 — Concurrency (optional)
- [ ] Keep single-threaded OR
- [ ] Add worker threads
- [ ] Optional: lock-free queue for requests

---

## Phase 8 — Observability
- [ ] Logging (commands, errors)
- [ ] Metrics:
  - ops/sec
  - latency
  - memory usage

---

## Phase 9 — Performance
- [ ] Benchmark (ops/sec)
- [ ] Profile CPU/memory
- [ ] Optimize:
  - hash map
  - allocations
  - parsing

---

## Phase 10 — Robustness
- [ ] Handle malformed input
- [ ] Handle large values
- [ ] Prevent crashes on bad input
- [ ] Graceful shutdown

---

## Phase 11 — Polish
- [ ] Clean structure
- [ ] README:
  - architecture
  - protocol
  - benchmarks
- [ ] Example client usage

---

## Final Goal
- Fast in-memory KV store
- Supports persistence
- Measured performance
- Clean design

---

## Rule
Start simple:
- single-threaded
- correct behavior
Then optimize later
