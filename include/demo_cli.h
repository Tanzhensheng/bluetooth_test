#ifndef DEMO_CLI_H
#define DEMO_CLI_H

#include "mod_ble_types.h"

typedef struct {
    mod_ble_config_t config;
    uint8_t payload[MOD_BLE_MAX_FRAME_DATA_LEN];
    size_t payload_len;
} demo_cli_options_t;

void demo_cli_options_init(demo_cli_options_t *options);
int demo_cli_parse(int argc, char **argv, demo_cli_options_t *options);
void demo_cli_print_usage(const char *program_name);

#endif
