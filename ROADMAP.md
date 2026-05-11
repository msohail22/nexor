# In-Memory KV Store (C++) — Learning Roadmap

> A project-based curriculum: each phase contains **concrete programs** to write that build skills toward the final KV store. Start with tiny standalone programs, then integrate them into the real store.

---

## Phase 0 — C++ Foundation & Build System

**Goal**: Get comfortable with C++ project setup, CMake, and basic tooling.

**Programs to write:**

### 0.1 — Hello CMake
- `hello/` — single `main.cpp` that prints "Hello, World!"
- `CMakeLists.txt` from scratch
- Build with `cmake -B build && cmake --build build`

### 0.2 — Logger library
- `logger/` — your own `Logger` class with levels (INFO, WARN, ERROR)
- Writes to stdout with timestamps
- Header-only or static library via CMake

### 0.3 — Ring buffer
- `ring_buffer/` — fixed-capacity circular buffer (template)
- Operations: `push`, `pop`, `front`, `back`, `size`, `empty`, `full`
- Write a test harness in `main.cpp` that exercises it

### 0.4 — Project skeleton
- [ ] Create `kv-store/` with CMake
- [ ] Subdirectories: `core/`, `net/`, `storage/`, `persistence/`, `protocol/`
- [ ] Each dir has a `CMakeLists.txt` that builds a library
- [ ] Top-level `CMakeLists.txt` links them into a single binary
- [ ] Main entry point that just starts and logs "KV Store starting..."

**Skills learned:** CMake project structure, library linking, logging, templates, testing mindset.

---

## Phase 1 — Networking 101

**Goal**: Understand TCP sockets, blocking vs non-blocking, and event loops.

**Programs to write:**

### 1.1 — Echo server (blocking, single-client)
- Listens on a port, accepts one connection
- Reads lines, echoes them back
- Run with `nc localhost <port>` to test
- Files: `echo_server.cpp`

### 1.2 — Echo server (blocking, multi-client with threads)
- Spawn a thread per accepted connection
- Each thread echoes until client disconnects
- Files: `echo_server_mt.cpp`

### 1.3 — Echo server (non-blocking, single-threaded)
- Set sockets to `O_NONBLOCK`
- Poll manually in a loop (using `select` or `poll`)
- Handle multiple clients without threads
- Files: `echo_server_nb.cpp`

### 1.4 — Echo server (epoll)
- Use `epoll_create1`, `epoll_ctl`, `epoll_wait`
- Edge-triggered or level-triggered mode
- Handle accept + read/write in the event loop
- Files: `echo_server_epoll.cpp`

### 1.5 — Integrate: net/ module
- [ ] TCP server class (listen, accept)
- [ ] Non-blocking sockets
- [ ] epoll event loop
- [ ] Handle multiple clients
- [ ] Connection lifecycle (connect → read/write → close)

**Skills learned:** Socket API, blocking vs non-blocking, epoll, event-driven architecture, connection state machines.

---

## Phase 2 — Protocol Parser

**Goal**: Design a simple text protocol and write a parser.

**Programs to write:**

### 2.1 — String tokenizer
- Write a function that splits a string by whitespace
- Handle quotes (optional): `SET "my key" "my value"` → 3 tokens
- Files: `tokenizer.cpp`

### 2.2 — Command parser
- Parse strings like `SET key value`, `GET key`, `DEL key`
- Return a struct: `{CommandType type, vector<string> args}`
- Handle errors: unknown command, missing args
- Files: `command_parser.cpp`

### 2.3 — Response serializer
- Serialize responses: `OK\r\n`, `value\r\n`, `ERROR message\r\n`
- Handle nil/bulk strings (for future protocol variants)
- Files: `response_serializer.cpp`

### 2.4 — Fuzz tester for parser
- Read random/brute-force inputs from stdin
- Feed them to your parser — must never crash
- Files: `parser_fuzz.cpp`

### 2.5 — Integrate: protocol/ module
- [ ] Define text protocol:
  - `SET key value`
  - `GET key`
  - `DEL key`
- [ ] Parse incoming commands
- [ ] Serialize responses (`OK`, value, errors)

**Skills learned:** String parsing, state machines, error handling, fuzz testing, protocol design.

---

## Phase 3 — Core Storage Engine

**Goal**: Build the in-memory hash map that stores key-value pairs.

**Programs to write:**

### 3.1 — Homemade hash map (open addressing)
- Implement `HashMap<K, V>` with linear probing
- Operations: `insert`, `get`, `erase`, `contains`, `size`
- Resize when load factor exceeds threshold
- Benchmark against `std::unordered_map`
- Files: `hash_map.cpp`

### 3.2 — String store
- `StringStore class`: wrapper around `std::unordered_map<string, string>`
- Methods: `Set(key, value)`, `Get(key) → optional<string>`, `Delete(key) → bool`
- Files: `string_store.cpp`

### 3.3 — Storage benchmark
- Insert 1M keys, measure time
- Measure memory usage (`/proc/self/status` or `mallinfo`)
- Files: `storage_bench.cpp`

### 3.4 — Integrate: core/ + storage/ modules
- [ ] In-memory hash map (key → value)
- [ ] Basic operations: GET, SET, DEL
- [ ] Thread safety documentation (single-threaded for now)

**Skills learned:** Hash table internals, open addressing vs chaining, load factor, `std::optional`, memory measurement.

---

## Phase 4 — TTL / Expiration

**Goal**: Add time-to-live and automatic key expiration.

**Programs to write:**

### 4.1 — Timed key-value store
- Extend `StringStore`: each key has an optional expiry timestamp
- `Set(key, value, ttl_seconds=0)` — 0 means no expiry
- `Get(key)` — check expiry before returning; delete if expired
- Files: `timed_store.cpp`

### 4.2 — Expiry sweeper
- Periodic background task (or inline check on access)
- Scan and delete expired keys
- Compare: lazy (on access) vs active (periodic sweep) vs hybrid
- Files: `expiry_sweeper.cpp`

### 4.3 — Expiration benchmark
- Set 100K keys with varying TTLs
- Measure memory reduction over time
- Files: `expiry_bench.cpp`

### 4.4 — Integrate: TTL into storage
- [ ] Add expiry metadata per key
- [ ] Lazy expiration on access
- [ ] Periodic cleanup (optional)

**Skills learned:** Time handling (`std::chrono`), lazy vs active expiration, memory management over time.

---

## Phase 5 — Persistence

**Goal**: Make data survive restarts.

**Programs to write:**

### 5.1 — Simple file writer/reader
- Write a string to a file, read it back
- Handle binary data safely (write size-prefixed)
- Files: `file_io.cpp`

### 5.2 — Snapshot dumper
- Dump entire `StringStore` to a file
- Format: `key_len:key:value_len:value\n` per line
- Load on startup and rebuild the store
- Files: `snapshot.cpp`

### 5.3 — Append-only log (AOF)
- Every write command (`SET`, `DEL`) appends to a log file
- Format: `SET key_len key value_len value\n` or `DEL key_len key\n`
- On startup: replay all commands from log
- Handle log rotation (start new file after N bytes)
- Files: `aof.cpp`

### 5.4 — Persistence benchmark
- Snapshot 10M keys, measure time and file size
- AOF replay 10M ops, measure time
- Compare: load time, file size, write throughput
- Files: `persist_bench.cpp`

### 5.5 — Integrate: persistence/ module
- [ ] Option A: Snapshot (dump entire DB to file) + load on startup
- [ ] Option B (recommended): Append-only log (AOF) + replay on startup

**Skills learned:** File I/O, serialization formats, append-only logs, crash recovery, read/write tradeoffs.

---

## Phase 6 — Memory Management

**Goal**: Control memory usage with eviction policies.

**Programs to write:**

### 6.1 — Memory tracker
- Track total bytes used by stored keys + values
- Use `std::malloc` hooks or override `new`/`delete` (or just sum sizes)
- Expose `MemoryUsage()` method
- Files: `mem_tracker.cpp`

### 6.2 — LRU cache
- `LRUCache<K, V>` with fixed capacity
- Operations: `Get`, `Put`, `Evict`
- Use doubly-linked list + hash map for O(1) operations
- Files: `lru_cache.cpp`

### 6.3 — Eviction policy adapter
- `EvictionPolicy` interface: `SelectVictim()`, `OnAccess(key)`, `OnInsert(key)`
- Implement LRU, FIFO, and optionally LFU or RR
- Files: `eviction_policy.cpp`

### 6.4 — Configurable store with eviction
- Wrapper around `StringStore` + `EvictionPolicy` + max memory limit
- When memory exceeds limit, evict keys until under limit
- Files: `bounded_store.cpp`

### 6.5 — Integrate: memory management into storage
- [ ] Track memory usage
- [ ] Implement LRU eviction
- [ ] Configurable max memory

**Skills learned:** Memory tracking, cache eviction policies, linked list + hash map combo, resource limits.

---

## Phase 7 — Concurrency

**Goal**: Handle multiple clients safely and efficiently.

**Programs to write:**

### 7.1 — Thread-safe counter
- `AtomicCounter` using `std::atomic<int>`
- `MutexCounter` using `std::mutex`
- Compare performance under contention (10 threads, 1M increments)
- Files: `counter_bench.cpp`

### 7.2 — Thread-safe string store
- Wrap `StringStore` with `std::shared_mutex`
  - Readers (GET) take shared lock
  - Writers (SET, DEL) take exclusive lock
- Files: `concurrent_store.cpp`

### 7.3 — Producer-consumer queue
- Lock-based queue (`std::mutex` + `std::condition_variable`)
- Lock-free queue (single-producer single-consumer using ring buffer)
- Files: `work_queue.cpp`

### 7.4 — Worker thread pool
- `ThreadPool` class: N workers, a shared task queue
- Submit tasks via `enqueue(callback)`
- Graceful shutdown
- Files: `thread_pool.cpp`

### 7.5 — Integrate: concurrent KV store
- [ ] Single-threaded epoll loop (recommended default)
- [ ] OR add worker threads + thread-safe store
- [ ] Optional: lock-free queue for requests

**Skills learned:** Mutexes, atomics, reader-writer locks, condition variables, thread pools, lock-free programming basics.

---

## Phase 8 — Observability

**Goal**: See what the server is doing.

**Programs to write:**

### 8.1 — Metrics collector
- `Metrics` singleton
- Counters: `commands_received`, `commands_success`, `commands_failed`
- Gauges: `active_connections`, `memory_usage_bytes`, `key_count`
- Histograms: `command_latency_us`
- Files: `metrics.cpp`

### 8.2 — Latency probe
- Measure time from request received to response sent
- Track p50, p95, p99, p99.9 latencies
- Files: `latency_tracker.cpp`

### 8.3 — Stats endpoint
- `STATS` command that returns all current metrics as key-value pairs
- Example response: `commands_received: 1000\nops_per_sec: 500.5\n...`
- Files: `stats_handler.cpp`

### 8.4 — Prometheus exporter (optional stretch)
- Expose metrics in Prometheus text format on an HTTP endpoint
- Files: `prometheus_exporter.cpp`

### 8.5 — Integrate: observability into main server
- [ ] Logging (commands, errors)
- [ ] Metrics: ops/sec, latency, memory usage

**Skills learned:** Metrics collection, latency percentiles, instrumentation, Prometheus exposition format.

---

## Phase 9 — Performance Engineering

**Goal**: Measure and optimize.

**Programs to write:**

### 9.1 — Macro benchmark
- Launch N clients (using threads + sockets)
- Each sends M commands (mix of GET/SET)
- Measure total time, calculate ops/sec
- Files: `bench_client.cpp`

### 9.2 — Micro benchmarks per component
- HashMap lookup vs `std::unordered_map`
- Parser throughput (MB/sec)
- Epoll vs poll vs select latency
- Thread pool overhead
- Files: `micro_bench.cpp`

### 9.3 — Profiling session
- Use `perf` (Linux) to find hottest functions
- Use `valgrind --tool=callgrind` or `heaptrack` for memory
- Document findings in `PROFILING_NOTES.md`
- Files: `profiling_session/`

### 9.4 — Optimizations (choose and implement)
- Custom hash map (open addressing, Robin Hood hashing)
- Arena allocator for strings (reduce malloc calls)
- Zero-copy parsing (avoid `std::string` allocations)
- Batch processing in epoll
- Files: `arena_allocator.cpp`, `robin_hood_map.cpp`

### 9.5 — Integrate: benchmark + profile the full server
- [ ] Benchmark (ops/sec)
- [ ] Profile CPU/memory
- [ ] Optimize: hash map, allocations, parsing

**Skills learned:** Benchmarking methodology, profiling with perf/valgrind, CPU optimization, allocator design, data-oriented design.

---

## Phase 10 — Robustness

**Goal**: Handle anything gracefully.

**Programs to write:**

### 10.1 — Malformed input handler
- Feed the server malformed commands
  - Too-long keys (1MB key name)
  - Too-long values
  - Binary garbage
  - Partial lines
- Must not crash; must return error responses
- Files: `malformed_input_test.cpp`

### 10.2 — Connection flood test
- Open 10k connections simultaneously
- Send/receive on some, idle on others
- Server must handle all without OOM
- Files: `flood_test.cpp`

### 10.3 — Large value handler
- SET a key with 100MB value
- GET it back, verify integrity
- Stream writes/reads in chunks (don't buffer entire thing)
- Files: `large_value_test.cpp`

### 10.4 — Graceful shutdown
- Handle `SIGINT`, `SIGTERM`
- Flush pending writes
- Close all connections cleanly
- Save snapshot on shutdown (if AOF is enabled)
- Files: `signal_handler.cpp`

### 10.5 — Integration test suite
- Start server, run client, kill server, restart, verify data
- Use `fork`/`exec` or subprocess management
- Test persistence round-trip
- Files: `integration_test.cpp`

### 10.6 — Integrate: robustness into production server
- [ ] Handle malformed input
- [ ] Handle large values
- [ ] Prevent crashes on bad input
- [ ] Graceful shutdown

**Skills learned:** Robust programming, resource limits, signal handling, integration testing, failure modes.

---

## Phase 11 — Polish & Ship

**Goal**: Make the project presentable and documented.

**Programs to write:**

### 11.1 — CLI argument parser
- `--port`, `--max-memory`, `--persistence`, `--help`
- Files: `cli_args.cpp`

### 11.2 — Configuration file support
- Read config from a TOML or simple key-value file
- Merge CLI args + config file
- Files: `config_parser.cpp`

### 11.3 — Dockerfile
- Multi-stage build
- Small final image (Alpine + static binary)
- `docker-compose.yml` for easy testing
- Files: `Dockerfile`, `docker-compose.yml`

### 11.4 — Example clients
- Python example: `python3 client.py` (plain sockets)
- Node.js example: `node client.js`
- Bash example: `exec 3<>/dev/tcp/localhost/6379; echo "SET a b" >&3`
- Files: `examples/`

### 11.5 — CI setup
- GitHub Actions workflow:
  - Build on Ubuntu + macOS
  - Run unit tests
  - Run integration tests
  - Run benchmarks (warn on regression)
- Files: `.github/workflows/ci.yml`

### 11.6 — Final documentation
- `README.md`: architecture overview, protocol spec, quick start
- Code comments on all public APIs (Doxygen)
- `CHANGELOG.md` for future releases
- Files: `README.md`, `CHANGELOG.md`, `Doxyfile`

### 11.7 — Integrate: final polish touches
- [ ] Clean structure
- [ ] README: architecture, protocol, benchmarks
- [ ] Example client usage

**Skills learned:** CLI parsing, Docker, CI/CD, documentation, packaging, API design.

---

## Final Goal
- Fast in-memory KV store
- Supports persistence
- Measured performance
- Clean design

## Rule
Start simple:
- single-threaded
- correct behavior
Then optimize later
