#ifndef PROTO_CODEC_H
#define PROTO_CODEC_H

#include "mod_ble_types.h"

int proto_codec_encode(const proto_frame_t *frame, uint8_t *out, size_t out_size, size_t *out_len);
int proto_codec_decode(const uint8_t *raw, size_t raw_len, proto_frame_t *frame);

#endif
