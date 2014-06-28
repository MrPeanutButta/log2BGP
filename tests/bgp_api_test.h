/*
 * File:   bgp_api_test.h
 * Author: aaron
 *
 * Created on Oct 24, 2013, 9:53:31 AM
 */

#ifndef BGP_API_TEST_H
#define	BGP_API_TEST_H

#include <cppunit/extensions/HelperMacros.h>
#include "../include/bgp_msg.h"

class bgp_api_test : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(bgp_api_test);

    CPPUNIT_TEST(test_api_insert);

    CPPUNIT_TEST_SUITE_END();

public:
    bgp_api_test();
    virtual ~bgp_api_test();
    void setUp();
    void tearDown();

private:
    void test_api_insert();

    BGP *test_bgp;
    bgp_peer *peer;
    bgp_update update;

};

#endif	/* BGP_API_TEST_H */

