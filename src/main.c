#include "mod_ble.h"
#include "mod_ble_log.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static size_t fill_default_payload(uint8_t *buf, size_t buf_size)
{
    static const uint8_t sample[] = {'H', 'e', 'l', 'l', 'o', ',', ' ', 'B', 'L', 'E'};

    if (buf_size < sizeof(sample)) {
        return 0U;
    }

    (void)memcpy(buf, sample, sizeof(sample));
    return sizeof(sample);
}

static int parse_hex_bytes(int argc, char **argv, int start_index, uint8_t *buf, size_t buf_size, size_t *out_len)
{
    int i;
    size_t len = 0U;

    for (i = start_index; i < argc; ++i) {
        char *end = NULL;
        unsigned long value;

        if (len >= buf_size) {
            return MOD_BLE_STATUS_INVALID_ARG;
        }

        value = strtoul(argv[i], &end, 16);
        if (end == argv[i] || *end != '\0' || value > 0xFFUL) {
            return MOD_BLE_STATUS_INVALID_ARG;
        }

        buf[len++] = (uint8_t)value;
    }

    *out_len = len;
    return MOD_BLE_STATUS_OK;
}

int main(int argc, char **argv)
{
    const char *target_id = "demo-target";
    uint8_t tx_buf[MOD_BLE_MAX_FRAME_DATA_LEN];
    uint8_t rx_buf[MOD_BLE_MAX_FRAME_DATA_LEN];
    size_t tx_len = 0U;
    int status;
    int rx_len;

    if (argc >= 3 && strcmp(argv[1], "--target") == 0) {
        target_id = argv[2];
        if (argc > 3) {
            status = parse_hex_bytes(argc, argv, 3, tx_buf, sizeof(tx_buf), &tx_len);
            if (status != MOD_BLE_STATUS_OK) {
                mod_ble_log_error("invalid hex payload");
                return EXIT_FAILURE;
            }
        }
    }

    if (tx_len == 0U) {
        tx_len = fill_default_payload(tx_buf, sizeof(tx_buf));
    }

    status = mod_ble_open(target_id);
    if (status != MOD_BLE_STATUS_OK) {
        mod_ble_log_error("mod_ble_open failed: %d", status);
        return EXIT_FAILURE;
    }

    status = mod_ble_send(tx_buf, tx_len, MOD_BLE_PROTO_NEAR_FIELD);
    if (status != MOD_BLE_STATUS_OK) {
        mod_ble_log_error("mod_ble_send failed: %d", status);
        mod_ble_close();
        return EXIT_FAILURE;
    }

    rx_len = mod_ble_recv(rx_buf, sizeof(rx_buf), 3000);
    if (rx_len < 0) {
        mod_ble_log_error("mod_ble_recv failed: %d", rx_len);
        mod_ble_close();
        return EXIT_FAILURE;
    }

    mod_ble_log_info("demo completed with %d bytes of payload", rx_len);
    mod_ble_close();
    return EXIT_SUCCESS;
}
