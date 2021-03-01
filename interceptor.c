#include "interceptor.h"
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

static bool shutdown(const char *request, int request_buffer_size)
{
    printf("Received a request to poweroff\n");
    return true;
}

void interceptor(const char *request, int request_buffer_size)
{
    if (strstr(request, "poweroff_")) {
        shutdown(request, request_buffer_size);
    }
}
