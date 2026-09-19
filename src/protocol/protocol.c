#include "bloom/protocol.h"

static bloom_protocol_t protocol;

static int protocol_init(void)
{
    if (protocol.transport == NULL)
        return -1;

    return protocol.transport->init();
}

static int protocol_spin(void)
{
    return 0;
}

bloom_protocol_t *bloom_protocol_create(
    bloom_transport_t *transport)
{
    if (transport == NULL)
        return NULL;

    protocol.transport = transport;
    protocol.init = protocol_init;
    protocol.spin = protocol_spin;

    return &protocol;
}