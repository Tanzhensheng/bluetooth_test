#include "proto_session.h"

#include "proto_codec.h"

#include <string.h>

void proto_session_init(proto_session_t *session)
{
    if (session == NULL) {
        return;
    }

    (void)memset(session, 0, sizeof(*session));
    session->is_open = 1;
}

int proto_session_build_request(proto_session_t *session, mod_ble_proto_t prot, const uint8_t *payload,
    size_t payload_len, proto_frame_t *frame)
{
    if (session == NULL || payload == NULL || frame == NULL || payload_len > MOD_BLE_MAX_FRAME_DATA_LEN) {
        return MOD_BLE_STATUS_INVALID_ARG;
    }
    if (!session->is_open) {
        return MOD_BLE_STATUS_STATE;
    }

    (void)memset(frame, 0, sizeof(*frame));
    frame->control = 0x43U;
    frame->pseq = session->next_pseq++;
    frame->fseq = 0x80U;
    frame->prot = (uint8_t)prot;
    frame->data_len = payload_len;
    (void)memcpy(frame->data, payload, payload_len);
    return MOD_BLE_STATUS_OK;
}

int proto_session_parse_response(proto_session_t *session, const uint8_t *raw, size_t raw_len, proto_frame_t *frame)
{
    int status;

    if (session == NULL) {
        return MOD_BLE_STATUS_INVALID_ARG;
    }

    status = proto_codec_decode(raw, raw_len, frame);
    if (status == MOD_BLE_STATUS_OK && frame != NULL) {
        session->last_fseq = frame->fseq;
    }
    return status;
}
