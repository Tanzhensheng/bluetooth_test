#ifndef BLE_CLIENT_H
#define BLE_CLIENT_H

#include "mod_ble_types.h"

int ble_client_open(ble_client_context_t *ctx);
int ble_client_send(ble_client_context_t *ctx, const uint8_t *data, size_t len);
int ble_client_receive(ble_client_context_t *ctx, uint8_t *buf, size_t buf_size, int timeout_ms);
void ble_client_close(ble_client_context_t *ctx);

#endif
