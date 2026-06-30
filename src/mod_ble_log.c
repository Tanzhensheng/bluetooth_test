#include "mod_ble_log.h"

#include <stdarg.h>
#include <stdio.h>

static void mod_ble_vlog(const char *level, const char *fmt, va_list args)
{
    (void)fprintf(stdout, "[%s] ", level);
    (void)vfprintf(stdout, fmt, args);
    (void)fprintf(stdout, "\n");
}

void mod_ble_log_info(const char *fmt, ...)
{
    va_list args;

    va_start(args, fmt);
    mod_ble_vlog("INFO", fmt, args);
    va_end(args);
}

void mod_ble_log_error(const char *fmt, ...)
{
    va_list args;

    va_start(args, fmt);
    mod_ble_vlog("ERROR", fmt, args);
    va_end(args);
}
