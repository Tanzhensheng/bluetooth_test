#include "proto_codec.h"

#include <string.h>

enum {
    PROTO_FRAME_HEAD = 0xA5,
    PROTO_FRAME_TAIL = 0x96,
    PROTO_MIN_FRAME_SIZE = 8
};

static uint8_t proto_codec_checksum(const proto_frame_t *frame)
{
    size_t i;
    uint32_t sum = 0U;

    sum += frame->control;
    sum += frame->pseq;
    sum += frame->fseq;
    sum += frame->prot;
    for (i = 0; i < frame->data_len; ++i) {
        sum += frame->data[i];
    }

    return (uint8_t)(sum & 0xFFU);
}

int proto_codec_encode(const proto_frame_t *frame, uint8_t *out, size_t out_size, size_t *out_len)
{
    size_t payload_len;

    if (frame == NULL || out == NULL || out_len == NULL) {
        return MOD_BLE_STATUS_INVALID_ARG;
    }

    payload_len = frame->data_len + 4U;
    if (out_size < payload_len + 2U) {
        return MOD_BLE_STATUS_INVALID_ARG;
    }

    out[0] = PROTO_FRAME_HEAD;
    out[1] = (uint8_t)payload_len;
    out[2] = frame->control;
    out[3] = frame->pseq;
    out[4] = frame->fseq;
    out[5] = frame->prot;
    (void)memcpy(&out[6], frame->data, frame->data_len);
    out[6 + frame->data_len] = proto_codec_checksum(frame);
    out[7 + frame->data_len] = PROTO_FRAME_TAIL;
    *out_len = 8U + frame->data_len;
    return MOD_BLE_STATUS_OK;
}

int proto_codec_decode(const uint8_t *raw, size_t raw_len, proto_frame_t *frame)
{
    size_t payload_len;
    uint8_t checksum;

    if (raw == NULL || frame == NULL || raw_len < PROTO_MIN_FRAME_SIZE) {
        return MOD_BLE_STATUS_INVALID_ARG;
    }
    if (raw[0] != PROTO_FRAME_HEAD || raw[raw_len - 1U] != PROTO_FRAME_TAIL) {
        return MOD_BLE_STATUS_IO;
    }
    if ((size_t)raw[1] + 4U != raw_len) {
        return MOD_BLE_STATUS_IO;
    }

    payload_len = raw_len - 8U;
    if (payload_len > MOD_BLE_MAX_FRAME_DATA_LEN) {
        return MOD_BLE_STATUS_UNSUPPORTED;
    }

    (void)memset(frame, 0, sizeof(*frame));
    frame->control = raw[2];
    frame->pseq = raw[3];
    frame->fseq = raw[4];
    frame->prot = raw[5];
    frame->data_len = payload_len;
    (void)memcpy(frame->data, &raw[6], payload_len);
    checksum = proto_codec_checksum(frame);
    if (checksum != raw[6 + payload_len]) {
        return MOD_BLE_STATUS_IO;
    }
    return MOD_BLE_STATUS_OK;
}
