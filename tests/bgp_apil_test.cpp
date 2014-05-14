/*
 * File:   bgp_mysql_test.cpp
 * Author: aaron
 *
 * Created on Oct 24, 2013, 9:53:32 AM
 */

#include "bgp_api_test.h"
#include "bgp.h"
#include "test_packets.h"

CPPUNIT_TEST_SUITE_REGISTRATION(bgp_api_test);

bgp_api_test::bgp_api_test() {
    test_bgp = new BGP;
    peer = new bgp_peer;
}

bgp_api_test::~bgp_api_test() {
}

void bgp_api_test::setUp() {

    uint16_t AS = 65301;
    // init BGP instance with test ASN
    test_bgp->init_BGP(AS);

    queue_data *q1 = new queue_data();
    queue_data *q2 = new queue_data();

    // prep test packets
    q1->length = 0x7a;
    memcpy(&q1->packet, &packets::pkt1, (q1->length));
    q2->length = 0x5b;
    memcpy(&q2->packet, &packets::pkt2, (q2->length));

    // push test update to dummy peer queue

    peer->in_queue.push(*q2);
    peer->in_queue.push(*q1);
    peer->in_q = 2;

    delete q2, q1;

    /* init processing of dummy peer queue.
     * push to BGP RIB queue after processing */
    test_bgp->proc_in_q(test_bgp, peer);
    test_bgp->proc_adj_rib_in_queue();

}

void bgp_mysql_test::tearDown() {
    // nothing to see here
}

void bgp_mysql_test::test_api_insert() {

    // process update on the RIB queue

    CPPUNIT_ASSERT(
            test_bgp->api_test()
            );

}

