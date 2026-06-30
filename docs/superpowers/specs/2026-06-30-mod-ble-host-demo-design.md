# mod_ble_host_demo Design

## 目标

搭建一个完全独立于现有业务工程的 Linux BLE 主机 demo，先把未来 `platform/mod` 需要的边界和工程结构固定下来，再逐步补齐 BlueZ GATT 和协议实现。

## 约束

- 新项目单独落在 `projects/project002_ble_host_demo/`
- 不引用 `project001_bluetooth` 的源码
- 对外暴露 socket 风格接口：`open/send/recv/close`
- 内部保留三层边界：BLE 传输层、协议编解码层、协议会话层
- 在缺少 GATT UUID 的情况下，先交付可编译、可运行的骨架 demo

## 架构

### 对外接口层

`mod_ble.*` 负责对外提供统一 API，未来迁入 `platform/mod` 时只让上层依赖这一层。

### BLE 传输层

`ble_client.*` 负责 BlueZ 侧扫描、连接、服务发现、写入和通知订阅。第一阶段只搭结构和上下文，不接真实 D-Bus 逻辑。

### 协议层

`proto_codec.*` 负责帧结构、控制域、校验和的编码和解码。第一阶段只做最小实现，保证上层接口可联调。

### 会话层

`proto_session.*` 负责 `PSEQ/FSEQ`、超时、重发、分帧和补包机制。第一阶段只保留上下文和占位发送/接收流程。

## 交付物

- 独立项目骨架
- CMake 构建入口
- README 与 GATT 占位文档
- 设计文档和实现计划文档
- 可执行 demo：支持参数解析、API 调用、日志输出和 stub 返回
