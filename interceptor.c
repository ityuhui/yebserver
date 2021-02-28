#include "interceptor.h"

void interceptor(const char *request, int request_buffer_size)
{
    if (strstr(request, "poweroff_")) {
        shutdown(request, request_buffer_size);
    }
}

static bool shutdown()