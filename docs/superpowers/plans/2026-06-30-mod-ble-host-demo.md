# mod_ble_host_demo Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Create a standalone Linux BLE host demo with a socket-style API boundary that can later move into `platform/mod` without dragging in `project001`.

**Architecture:** The project exposes a small `mod_ble_open/send/recv/close` API while keeping BLE transport, protocol codec, and protocol session state in separate files. The first delivery is a compilable scaffold with placeholders for BlueZ GATT integration and protocol state machines.

**Tech Stack:** C11, CMake, future BlueZ D-Bus integration

---

### Task 1: Create project-local docs and skeleton layout

**Files:**
- Create: `docs/gatt_placeholders.md`
- Create: `docs/superpowers/specs/2026-06-30-mod-ble-host-demo-design.md`
- Create: `docs/superpowers/plans/2026-06-30-mod-ble-host-demo.md`
- Create: `PROJECT_SYNC.md`

- [ ] Step 1: Add project metadata and placeholders
- [ ] Step 2: Record architecture and boundaries
- [ ] Step 3: Keep all new materials inside `project002_ble_host_demo`

### Task 2: Scaffold the build and public API

**Files:**
- Create: `CMakeLists.txt`
- Create: `include/mod_ble_types.h`
- Create: `include/mod_ble.h`

- [ ] Step 1: Define shared enums, result codes, and config structs
- [ ] Step 2: Define socket-style API signatures
- [ ] Step 3: Wire a minimal CMake target for library plus demo

### Task 3: Scaffold transport, codec, and session layers

**Files:**
- Create: `include/mod_ble_log.h`
- Create: `include/ble_client.h`
- Create: `include/proto_codec.h`
- Create: `include/proto_session.h`
- Create: `src/mod_ble_log.c`
- Create: `src/ble_client.c`
- Create: `src/proto_codec.c`
- Create: `src/proto_session.c`
- Create: `src/mod_ble.c`

- [ ] Step 1: Define focused responsibilities per file
- [ ] Step 2: Implement stub logic that compiles cleanly
- [ ] Step 3: Keep BlueZ-specific details behind `ble_client.*`

### Task 4: Add a runnable demo entrypoint

**Files:**
- Create: `src/main.c`
- Modify: `README.md`

- [ ] Step 1: Parse target id and optional hex payload arguments
- [ ] Step 2: Call `mod_ble_open/send/recv/close`
- [ ] Step 3: Print clear logs showing the end-to-end stub flow
