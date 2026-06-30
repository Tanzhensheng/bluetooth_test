# GATT 占位项

当前阶段只搭工程框架，不绑定真实终端参数。后续接入前需要补齐以下内容：

- 目标设备过滤方式：MAC、设备名或广播数据特征
- Service UUID
- 写入 Characteristic UUID
- Notify/Indicate Characteristic UUID
- 写入类型：`Write` 或 `Write Without Response`
- 是否需要配对、加密或 MTU 协商

建议后续接入时先做一个只负责扫描、连接、发现 GATT 的侦察 demo，再把结果回填到 `ble_client.c` 的配置结构中。
