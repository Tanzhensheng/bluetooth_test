#ifndef PROTO_SESSION_H
#define PROTO_SESSION_H

#include "mod_ble_types.h"

void proto_session_init(proto_session_t *session);
int proto_session_build_request(proto_session_t *session, mod_ble_proto_t prot, const uint8_t *payload,
    size_t payload_len, proto_frame_t *frame);
int proto_session_parse_response(proto_session_t *session, const uint8_t *raw, size_t raw_len, proto_frame_t *frame);

#endif
