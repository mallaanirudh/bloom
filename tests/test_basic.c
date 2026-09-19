#include <stdio.h>
#include <stdint.h>

#include "bloom/xrce_protocol.h"

int main(void)
{
    int ret = bloom_xrce_init("127.0.0.1", "8888");

    if (ret != 0) {
        printf("Init failed: %d\n", ret);
        return 1;
    }

    printf("Bloom XRCE initialization successful\n");

    uint8_t data[] = {1, 2, 3, 4};

    ret = bloom_xrce_publish(
        1,
        data,
        sizeof(data)
    );

    if (ret != 0) {
        printf("Publish failed: %d\n", ret);
        bloom_xrce_close();
        return 1;
    }

    printf("Message published\n");

    bloom_xrce_close();

    return 0;
}