# mod_ble_host_demo

独立的 Linux BLE 主机 demo 工程，目标是先在工程外部搭出 `platform/mod` 可复用的蓝牙通道骨架，再逐步补齐 BlueZ GATT 接入和协议细节。

## 当前范围

- 对外接口固定为 socket 风格：`open/send/recv/close`
- 协议层预留 `A5/L/C/PSEQ/FSEQ/PROT/DATA/CS/96` 的编解码和会话状态机
- BLE 传输层先保留 BlueZ D-Bus 接入点，不依赖现有 `project001`
- GATT 的 `Service UUID / Characteristic UUID / 写入方式 / Notify 方式` 先留空

## 目录结构

```text
include/
  mod_ble.h
  mod_ble_types.h
  mod_ble_log.h
  ble_client.h
  proto_codec.h
  proto_session.h
src/
  main.c
  mod_ble_log.c
  ble_client.c
  proto_codec.c
  proto_session.c
  mod_ble.c
docs/
  gatt_placeholders.md
  superpowers/
    specs/
    plans/
```

## 对外 API

```c
int mod_ble_open(const char *target_id);
int mod_ble_send(const uint8_t *data, size_t len, uint8_t prot);
int mod_ble_recv(uint8_t *buf, size_t buf_size, int timeout_ms);
void mod_ble_close(void);
```

## 构建

```bash
cmake -S . -B build
cmake --build build
```

如果目标环境暂时没有 `cmake`，可以直接用 `gcc` 验证当前骨架：

```bash
gcc -std=c11 -Wall -Wextra -Wpedantic -I include \
  src/main.c src/mod_ble.c src/mod_ble_log.c src/ble_client.c \
  src/proto_codec.c src/proto_session.c \
  -o build/mod_ble_demo
```

## 运行

```bash
./build/mod_ble_demo --target demo-target --hex A5 00 01
```

当前 demo 只验证骨架、日志和 API 连线，BlueZ GATT 细节仍是占位实现。
