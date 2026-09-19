#include "bloom/transport.h"
#include <stdio.h>
#include <string.h>

static int mock_init(void)
{
    printf("Mock transport initialized\n");
    return 0;
}

static int mock_write(const uint8_t *data, size_t len)
{
    printf("TX: ");

    for (size_t i = 0; i < len; i++)
        printf("%02X ", data[i]);

    printf("\n");

    return (int)len;
}

static int mock_read(uint8_t *data, size_t len)
{
    (void)data;
    (void)len;

    return 0;
}

static void mock_close(void)
{
    printf("Mock transport closed\n");
}

bloom_transport_t bloom_mock_transport = {
    .init = mock_init,
    .write = mock_write,
    .read = mock_read,
    .close = mock_close
};