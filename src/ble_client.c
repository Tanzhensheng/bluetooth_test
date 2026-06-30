#include "ble_client.h"

#include "mod_ble_log.h"

#include <stdio.h>
#include <string.h>

int ble_client_open(ble_client_context_t *ctx, const char *target_id)
{
    if (ctx == NULL || target_id == NULL || target_id[0] == '\0') {
        return MOD_BLE_STATUS_INVALID_ARG;
    }

    (void)memset(ctx, 0, sizeof(*ctx));
    (void)snprintf(ctx->target_id, sizeof(ctx->target_id), "%s", target_id);
    ctx->is_connected = 1;

    mod_ble_log_info("ble_client_open target=%s", ctx->target_id);
    mod_ble_log_info("BlueZ GATT placeholders are not filled yet; using stub transport");
    return MOD_BLE_STATUS_OK;
}

int ble_client_send(ble_client_context_t *ctx, const uint8_t *data, size_t len)
{
    size_t i;

    if (ctx == NULL || data == NULL || len == 0U) {
        return MOD_BLE_STATUS_INVALID_ARG;
    }
    if (!ctx->is_connected) {
        return MOD_BLE_STATUS_STATE;
    }

    mod_ble_log_info("ble_client_send len=%zu", len);
    (void)fprintf(stdout, "[TX] ");
    for (i = 0; i < len; ++i) {
        (void)fprintf(stdout, "%02X ", data[i]);
    }
    (void)fprintf(stdout, "\n");
    return MOD_BLE_STATUS_OK;
}

int ble_client_receive(ble_client_context_t *ctx, uint8_t *buf, size_t buf_size, int timeout_ms)
{
    static const uint8_t stub_response[] = {0xA5, 0x04, 0x80, 0x00, 0x80, 0x10, 0x10, 0x96};

    if (ctx == NULL || buf == NULL || buf_size < sizeof(stub_response)) {
        return MOD_BLE_STATUS_INVALID_ARG;
    }
    if (!ctx->is_connected) {
        return MOD_BLE_STATUS_STATE;
    }

    (void)memcpy(buf, stub_response, sizeof(stub_response));
    mod_ble_log_info("ble_client_receive timeout_ms=%d stub_len=%zu", timeout_ms, sizeof(stub_response));
    return (int)sizeof(stub_response);
}

void ble_client_close(ble_client_context_t *ctx)
{
    if (ctx == NULL) {
        return;
    }

    mod_ble_log_info("ble_client_close target=%s", ctx->target_id);
    ctx->is_connected = 0;
}
