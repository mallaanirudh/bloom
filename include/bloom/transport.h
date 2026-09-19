#ifndef BLOOM_TRANSPORT_H
#define BLOOM_TRANSPORT_H

#include <stddef.h>
#include <stdint.h>

typedef struct {
    int (*init)(void);
    int (*write)(const uint8_t *data, size_t len);
    int (*read)(uint8_t *data, size_t len);
    void (*close)(void);
} bloom_transport_t;

#endif
extern bloom_transport_t bloom_mock_transport;