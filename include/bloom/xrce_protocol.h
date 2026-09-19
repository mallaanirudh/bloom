#ifndef BLOOM_XRCE_PROTOCOL_H
#define BLOOM_XRCE_PROTOCOL_H

#include "bloom/types.h"

int bloom_xrce_init(
    const char *agent_ip,
    const char *agent_port
);

int bloom_xrce_publish(
    bloom_topic_id_t topic_id,
    const bloom_byte_t *data,
    bloom_size_t len
);

void bloom_xrce_close(void);

#endif