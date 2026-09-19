#include "bloom/xrce_protocol.h"

#include <uxr/client/client.h>

#include <stdint.h>
#include <stdio.h>

#define XRCE_CLIENT_KEY  0xB1000001u
#define STREAM_HISTORY   8
#define BUFFER_SIZE      (UXR_CONFIG_UDP_TRANSPORT_MTU * STREAM_HISTORY)

static uxrUDPTransport transport;
static uxrSession session;

static uint8_t output_buffer[BUFFER_SIZE];
static uint8_t input_buffer[BUFFER_SIZE];

static uxrStreamId reliable_out;
static uxrStreamId reliable_in;

static uxrObjectId participant_id;
static uxrObjectId topic_id;
static uxrObjectId publisher_id;
static uxrObjectId writer_id;

static int initialized = 0;

int bloom_xrce_init(
    const char *agent_ip,
    const char *agent_port)
{
    /* ---------- Transport ---------- */

    if (!uxr_init_udp_transport(
            &transport,
            UXR_IPv4,
            agent_ip,
            agent_port))
    {
        return -1;
    }

    /* ---------- Session ---------- */

    uxr_init_session(
        &session,
        &transport.comm,
        XRCE_CLIENT_KEY);

    if (!uxr_create_session(&session))
    {
        uxr_close_udp_transport(&transport);
        return -2;
    }

    /* ---------- Streams ---------- */

    reliable_out =
        uxr_create_output_reliable_stream(
            &session,
            output_buffer,
            sizeof(output_buffer),
            STREAM_HISTORY);

    reliable_in =
        uxr_create_input_reliable_stream(
            &session,
            input_buffer,
            sizeof(input_buffer),
            STREAM_HISTORY);

    /* ---------- Participant ---------- */

    participant_id =
        uxr_object_id(
            0x01,
            UXR_PARTICIPANT_ID);

    const char *participant_xml =
        "<dds>"
            "<participant>"
                "<rtps>"
                    "<name>bloom_participant</name>"
                "</rtps>"
            "</participant>"
        "</dds>";

    uint16_t participant_req =
        uxr_buffer_create_participant_xml(
            &session,
            reliable_out,
            participant_id,
            0,
            participant_xml,
            UXR_REPLACE);

    /* ---------- Topic ---------- */

    topic_id =
        uxr_object_id(
            0x01,
            UXR_TOPIC_ID);

    const char *topic_xml =
        "<dds>"
            "<topic>"
                "<name>BloomTest</name>"
                "<dataType>BloomTest</dataType>"
            "</topic>"
        "</dds>";

    uint16_t topic_req =
        uxr_buffer_create_topic_xml(
            &session,
            reliable_out,
            topic_id,
            participant_id,
            topic_xml,
            UXR_REPLACE);

    /* ---------- Publisher ---------- */

    publisher_id =
        uxr_object_id(
            0x01,
            UXR_PUBLISHER_ID);

    const char *publisher_xml = "";

    uint16_t publisher_req =
        uxr_buffer_create_publisher_xml(
            &session,
            reliable_out,
            publisher_id,
            participant_id,
            publisher_xml,
            UXR_REPLACE);

    /* ---------- DataWriter ---------- */

    writer_id =
        uxr_object_id(
            0x01,
            UXR_DATAWRITER_ID);

    const char *datawriter_xml =
        "<dds>"
            "<data_writer>"
                "<topic>"
                    "<kind>NO_KEY</kind>"
                    "<name>BloomTest</name>"
                    "<dataType>BloomTest</dataType>"
                "</topic>"
            "</data_writer>"
        "</dds>";

    uint16_t writer_req =
        uxr_buffer_create_datawriter_xml(
            &session,
            reliable_out,
            writer_id,
            publisher_id,
            datawriter_xml,
            UXR_REPLACE);

    /* ---------- Wait for Agent ---------- */

    uint16_t requests[4] = {
        participant_req,
        topic_req,
        publisher_req,
        writer_req
    };

    uint8_t status[4];

    if (!uxr_run_session_until_all_status(
            &session,
            1000,
            requests,
            status,
            4))
    {
        printf(
            "Entity creation failed:\n"
            "  participant: %u\n"
            "  topic:       %u\n"
            "  publisher:   %u\n"
            "  datawriter:  %u\n",
            status[0],
            status[1],
            status[2],
            status[3]);

        uxr_delete_session(&session);
        uxr_close_udp_transport(&transport);

        return -3;
    }

    initialized = 1;

    printf("Bloom XRCE entities created\n");

    return 0;
}

int bloom_xrce_publish(
    bloom_topic_id_t id,
    const bloom_byte_t *data,
    bloom_size_t len)
{
    (void)id;

    if (!initialized || data == NULL || len == 0)
        return -1;

    ucdrBuffer ub;

    uint16_t request =
        uxr_prepare_output_stream(
            &session,
            reliable_out,
            writer_id,
            &ub,
            (uint32_t)len);

    if (request == UXR_INVALID_REQUEST_ID)
        return -2;

    if (!ucdr_serialize_array_uint8_t(
            &ub,
            data,
            (uint32_t)len))
    {
        return -3;
    }

    if (!uxr_run_session_until_confirm_delivery(
            &session,
            1000))
    {
        return -4;
    }

    return 0;
}

void bloom_xrce_close(void)
{
    if (!initialized)
        return;

    uxr_delete_session(&session);
    uxr_close_udp_transport(&transport);

    initialized = 0;
}