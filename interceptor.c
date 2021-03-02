#include "interceptor.h"
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/reboot.h>
#include <linux/reboot.h>
#include "yclog/log.h"

static bool shutdown(const char *request, int request_buffer_size)
{
    print_log(WARNING, "Received a request to poweroff the host.\n");
    reboot(LINUX_REBOOT_CMD_POWER_OFF);
    return true;
}

void interceptor(const char *request, int request_buffer_size)
{
    if (strstr(request, "poweroff_")) {
        shutdown(request, request_buffer_size);
    }
}
