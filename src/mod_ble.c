#include "mod_ble.h"

#include "ble_client.h"
#include "mod_ble_log.h"
#include "proto_codec.h"
#include "proto_session.h"

#include <string.h>

static ble_client_context_t g_client;
static proto_session_t g_session;
static int g_is_open = 0;

int mod_ble_open(const char *target_id)
{
    int status;

    if (g_is_open) {
        return MOD_BLE_STATUS_STATE;
    }

    status = ble_client_open(&g_client, target_id);
    if (status != MOD_BLE_STATUS_OK) {
        return status;
    }

    proto_session_init(&g_session);
    g_is_open = 1;
    return MOD_BLE_STATUS_OK;
}

int mod_ble_send(const uint8_t *data, size_t len, uint8_t prot)
{
    proto_frame_t frame;
    uint8_t raw[MOD_BLE_MAX_HEX_DUMP_LEN];
    size_t raw_len = 0U;
    int status;

    if (!g_is_open) {
        return MOD_BLE_STATUS_STATE;
    }

    status = proto_session_build_request(&g_session, (mod_ble_proto_t)prot, data, len, &frame);
    if (status != MOD_BLE_STATUS_OK) {
        return status;
    }

    status = proto_codec_encode(&frame, raw, sizeof(raw), &raw_len);
    if (status != MOD_BLE_STATUS_OK) {
        return status;
    }

    mod_ble_log_info("mod_ble_send pseq=%u fseq=0x%02X prot=0x%02X", frame.pseq, frame.fseq, prot);
    return ble_client_send(&g_client, raw, raw_len);
}

int mod_ble_recv(uint8_t *buf, size_t buf_size, int timeout_ms)
{
    uint8_t raw[MOD_BLE_MAX_HEX_DUMP_LEN];
    proto_frame_t frame;
    int raw_len;
    int status;

    if (!g_is_open || buf == NULL) {
        return MOD_BLE_STATUS_INVALID_ARG;
    }

    raw_len = ble_client_receive(&g_client, raw, sizeof(raw), timeout_ms);
    if (raw_len < 0) {
        return raw_len;
    }

    status = proto_session_parse_response(&g_session, raw, (size_t)raw_len, &frame);
    if (status != MOD_BLE_STATUS_OK) {
        return status;
    }
    if (buf_size < frame.data_len) {
        return MOD_BLE_STATUS_INVALID_ARG;
    }

    (void)memcpy(buf, frame.data, frame.data_len);
    mod_ble_log_info("mod_ble_recv pseq=%u fseq=0x%02X data_len=%zu", frame.pseq, frame.fseq, frame.data_len);
    return (int)frame.data_len;
}

void mod_ble_close(void)
{
    if (!g_is_open) {
        return;
    }

    ble_client_close(&g_client);
    (void)memset(&g_client, 0, sizeof(g_client));
    (void)memset(&g_session, 0, sizeof(g_session));
    g_is_open = 0;
}
