#ifndef BLOOM_PROTOCOL_H
#define BLOOM_PROTOCOL_H

#include "bloom/types.h"
#include "bloom/transport.h"

typedef struct {
    bloom_transport_t *transport;

    int (*init)(void);

    int (*publish)(
        bloom_topic_id_t topic_id,
        const bloom_byte_t *data,
        bloom_size_t len
    );

    int (*subscribe)(
        bloom_topic_id_t topic_id
    );

    int (*spin)(void);

} bloom_protocol_t;

bloom_protocol_t *bloom_protocol_create(
    bloom_transport_t *transport
);

#endif