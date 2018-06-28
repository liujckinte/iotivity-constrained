/****************************************************************************
 *
 * Copyright 2018 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/

#include <gtest/gtest.h>
#include <cstdlib>

extern "C"{
    #include "st_manager.h"
    #include "st_easy_setup.h"
    #include "sc_easysetup.h"
    #include "st_store.h"
    #include "easysetup.h"
    #include "st_port.h"
    #include "sttestcommon.h"
    #include "messaging/coap/engine.h"
    void st_manager_quit(void);
}

#define MAX_SSID_LEN (32)
#define EASYSETUP_TAG "E1"

static const char *device_name = "Samsung";
static const char *manufacturer = "xxxx";
static const char *sid = "000";
static const char *modelNumber = "Model Number";

void easy_setup_handler_test(st_easy_setup_status_t status)
{
    (void)status;
}

class TestSTEasySetup: public testing::Test
{
    protected:
        virtual void SetUp()
        {

        }

        virtual void TearDown()
        {
            st_store_info_initialize();
        }
};

TEST_F(TestSTEasySetup, st_is_easy_setup_finish)
{
    st_store_t *store_info = st_store_get_info();
    store_info->status = true;
    int ret = st_is_easy_setup_finish();
    EXPECT_EQ(0, ret);
}

TEST_F(TestSTEasySetup, st_is_easy_setup_finish_fail)
{
    st_store_t *store_info = st_store_get_info();
    store_info->status = false;
    int ret = st_is_easy_setup_finish();
    EXPECT_EQ(-1, ret);
}

TEST_F(TestSTEasySetup, st_easy_setup_start)
{
    sc_properties st_vendor_props;
    memset(&st_vendor_props, 0, sizeof(sc_properties));
    int ret = st_easy_setup_start(&st_vendor_props, easy_setup_handler_test);
    EXPECT_EQ(0, ret);
    st_easy_setup_stop();
}

TEST_F(TestSTEasySetup, st_easy_setup_stop_reset_sc_properties)
{
    sc_properties g_scprop;
    sc_properties *g_scprop_ptr = NULL;
    memset(&g_scprop, 0, sizeof(sc_properties));
    oc_new_string(&g_scprop.model, modelNumber, strlen(modelNumber));
    set_sc_properties(&g_scprop);
    st_easy_setup_stop();
    g_scprop_ptr = get_sc_properties();

    EXPECT_EQ(NULL, g_scprop_ptr);
}

TEST_F(TestSTEasySetup, st_easy_setup_stop_es_set_state)
{
    es_enrollee_state en_state;
    st_easy_setup_stop();
    en_state = es_get_state();

    EXPECT_EQ(ES_STATE_INIT, en_state);
}

TEST_F(TestSTEasySetup, st_gen_ssid_ret)
{
    char ssid[MAX_SSID_LEN + 1];
    int ret = st_gen_ssid(ssid, device_name, manufacturer, sid);

    EXPECT_EQ(0, ret);
}

TEST_F(TestSTEasySetup, st_gen_ssid_compare)
{
    unsigned char mac[6] = { 0 };
    oc_get_mac_addr(mac);
    char ssid[MAX_SSID_LEN + 1], expected_ssid[MAX_SSID_LEN + 1];
    snprintf(expected_ssid, MAX_SSID_LEN, "%s_%s%s%s%d%02X%02X", device_name,
           EASYSETUP_TAG, manufacturer, sid, 0, mac[4], mac[5]);
    expected_ssid[strlen(expected_ssid)] = '\0';
    st_gen_ssid(ssid, device_name, manufacturer, sid);

    EXPECT_STREQ(expected_ssid, ssid);
}
/*
{
    "cd": "wifi_ssid",
    "ctry": "GB",
    "lang": "en",
    "loc": "",
    "tnn": "wifi_pw",
    "wat": 3,
    "wet": 4,
    "x.com.samsung.chn": 2462,
    "x.com.samsung.country": "GB",
    "x.com.samsung.datetime": "2018-06-27T05:03:15Z",
    "x.com.samsung.gpslocation": "",
    "x.com.samsung.language": "en",
    "x.com.samsung.regionaldatetime": "2018-06-27T14:03:15Z"
}
 */
static uint8_t wifi_prov_data[] = {
    0x48, 0x02, 0x6D, 0xD5, 0x78, 0x99, 0x3B, 0xEB, 0x75, 0xEC, 0x21, 0xFE,
    0xBD, 0x02, 0x45, 0x61, 0x73, 0x79, 0x53, 0x65, 0x74, 0x75, 0x70, 0x52,
    0x65, 0x73, 0x55, 0x52, 0x49, 0x11, 0x3C, 0x3B, 0x69, 0x66, 0x3D, 0x6F,
    0x69, 0x63, 0x2E, 0x69, 0x66, 0x2E, 0x62, 0x0D, 0x05, 0x72, 0x74, 0x3D,
    0x6F, 0x69, 0x63, 0x2E, 0x72, 0x2E, 0x65, 0x61, 0x73, 0x79, 0x73, 0x65,
    0x74, 0x75, 0x70, 0x21, 0x3C, 0xE1, 0xFE, 0xD6, 0xC0, 0xFF, 0xAD, 0x62,
    0x63, 0x64, 0x69, 0x77, 0x69, 0x66, 0x69, 0x5F, 0x73, 0x73, 0x69, 0x64,
    0x64, 0x63, 0x74, 0x72, 0x79, 0x62, 0x47, 0x42, 0x64, 0x6C, 0x61, 0x6E,
    0x67, 0x62, 0x65, 0x6E, 0x63, 0x6C, 0x6F, 0x63, 0x60, 0x63, 0x74, 0x6E,
    0x6E, 0x67, 0x77, 0x69, 0x66, 0x69, 0x5F, 0x70, 0x77, 0x63, 0x77, 0x61,
    0x74, 0x03, 0x63, 0x77, 0x65, 0x74, 0x04, 0x71, 0x78, 0x2E, 0x63, 0x6F,
    0x6D, 0x2E, 0x73, 0x61, 0x6D, 0x73, 0x75, 0x6E, 0x67, 0x2E, 0x63, 0x68,
    0x6E, 0x19, 0x09, 0x9E, 0x75, 0x78, 0x2E, 0x63, 0x6F, 0x6D, 0x2E, 0x73,
    0x61, 0x6D, 0x73, 0x75, 0x6E, 0x67, 0x2E, 0x63, 0x6F, 0x75, 0x6E, 0x74,
    0x72, 0x79, 0x62, 0x47, 0x42, 0x76, 0x78, 0x2E, 0x63, 0x6F, 0x6D, 0x2E,
    0x73, 0x61, 0x6D, 0x73, 0x75, 0x6E, 0x67, 0x2E, 0x64, 0x61, 0x74, 0x65,
    0x74, 0x69, 0x6D, 0x65, 0x74, 0x32, 0x30, 0x31, 0x38, 0x2D, 0x30, 0x36,
    0x2D, 0x32, 0x37, 0x54, 0x30, 0x35, 0x3A, 0x30, 0x33, 0x3A, 0x31, 0x35,
    0x5A, 0x78, 0x19, 0x78, 0x2E, 0x63, 0x6F, 0x6D, 0x2E, 0x73, 0x61, 0x6D,
    0x73, 0x75, 0x6E, 0x67, 0x2E, 0x67, 0x70, 0x73, 0x6C, 0x6F, 0x63, 0x61,
    0x74, 0x69, 0x6F, 0x6E, 0x60, 0x76, 0x78, 0x2E, 0x63, 0x6F, 0x6D, 0x2E,
    0x73, 0x61, 0x6D, 0x73, 0x75, 0x6E, 0x67, 0x2E, 0x6C, 0x61, 0x6E, 0x67,
    0x75, 0x61, 0x67, 0x65, 0x62, 0x65, 0x6E, 0x78, 0x1E, 0x78, 0x2E, 0x63,
    0x6F, 0x6D, 0x2E, 0x73, 0x61, 0x6D, 0x73, 0x75, 0x6E, 0x67, 0x2E, 0x72,
    0x65, 0x67, 0x69, 0x6F, 0x6E, 0x61, 0x6C, 0x64, 0x61, 0x74, 0x65, 0x74,
    0x69, 0x6D, 0x65, 0x74, 0x32, 0x30, 0x31, 0x38, 0x2D, 0x30, 0x36, 0x2D,
    0x32, 0x37, 0x54, 0x31, 0x34, 0x3A, 0x30, 0x33, 0x3A, 0x31, 0x35, 0x5A
};
static int wifi_prov_data_len = 324;

/*
{
    "ac": "",
    "apn": "https://auth.iotivity.org",
    "at": "xxxxxxxxxx",
    "att": 0,
    "cis": "coaps+tcp://iotivity-ciserver.org:443",
    "x.com.samsung.clientid": "1234567890",
    "x.com.samsung.refreshtoken": "xxxxxxxxxx",
    "x.com.samsung.uid": "xxxxxxxxxx"
}
*/
static uint8_t cloud_prov_data[] = {
    0x48, 0x02, 0x48, 0x67, 0xD8, 0x91, 0x06, 0xA0, 0xEA, 0x78, 0x47, 0x91,
    0xBD, 0x02, 0x45, 0x61, 0x73, 0x79, 0x53, 0x65, 0x74, 0x75, 0x70, 0x52,
    0x65, 0x73, 0x55, 0x52, 0x49, 0x11, 0x3C, 0x3B, 0x69, 0x66, 0x3D, 0x6F,
    0x69, 0x63, 0x2E, 0x69, 0x66, 0x2E, 0x62, 0x0D, 0x05, 0x72, 0x74, 0x3D,
    0x6F, 0x69, 0x63, 0x2E, 0x72, 0x2E, 0x65, 0x61, 0x73, 0x79, 0x73, 0x65,
    0x74, 0x75, 0x70, 0x21, 0x3C, 0xE1, 0xFE, 0xD6, 0xC0, 0xFF, 0xA8, 0x62,
    0x61, 0x63, 0x60, 0x63, 0x61, 0x70, 0x6E, 0x78, 0x19, 0x68, 0x74, 0x74,
    0x70, 0x73, 0x3A, 0x2F, 0x2F, 0x61, 0x75, 0x74, 0x68, 0x2E, 0x69, 0x6F,
    0x74, 0x69, 0x76, 0x69, 0x74, 0x79, 0x2E, 0x6F, 0x72, 0x67, 0x62, 0x61,
    0x74, 0x6A, 0x78, 0x78, 0x78, 0x78, 0x78, 0x78, 0x78, 0x78, 0x78, 0x78,
    0x63, 0x61, 0x74, 0x74, 0x00, 0x63, 0x63, 0x69, 0x73, 0x78, 0x25, 0x63,
    0x6F, 0x61, 0x70, 0x73, 0x2B, 0x74, 0x63, 0x70, 0x3A, 0x2F, 0x2F, 0x69,
    0x6F, 0x74, 0x69, 0x76, 0x69, 0x74, 0x79, 0x2D, 0x63, 0x69, 0x73, 0x65,
    0x72, 0x76, 0x65, 0x72, 0x2E, 0x6F, 0x72, 0x67, 0x3A, 0x34, 0x34, 0x33,
    0x76, 0x78, 0x2E, 0x63, 0x6F, 0x6D, 0x2E, 0x73, 0x61, 0x6D, 0x73, 0x75,
    0x6E, 0x67, 0x2E, 0x63, 0x6C, 0x69, 0x65, 0x6E, 0x74, 0x69, 0x64, 0x6A,
    0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30, 0x78, 0x1A,
    0x78, 0x2E, 0x63, 0x6F, 0x6D, 0x2E, 0x73, 0x61, 0x6D, 0x73, 0x75, 0x6E,
    0x67, 0x2E, 0x72, 0x65, 0x66, 0x72, 0x65, 0x73, 0x68, 0x74, 0x6F, 0x6B,
    0x65, 0x6E, 0x6A, 0x78, 0x78, 0x78, 0x78, 0x78, 0x78, 0x78, 0x78, 0x78,
    0x78, 0x71, 0x78, 0x2E, 0x63, 0x6F, 0x6D, 0x2E, 0x73, 0x61, 0x6D, 0x73,
    0x75, 0x6E, 0x67, 0x2E, 0x75, 0x69, 0x64, 0x6A, 0x78, 0x78, 0x78, 0x78,
    0x78, 0x78, 0x78, 0x78, 0x78, 0x78
};
static int cloud_prov_data_len = 282;

static st_mutex_t mutex = NULL;
static st_cond_t cv = NULL;
static st_thread_t t = NULL;

static void
st_status_handler(st_status_t status)
{
    if (status == ST_STATUS_EASY_SETUP_PROGRESSING ||
        status == ST_STATUS_EASY_SETUP_DONE) {
        st_mutex_lock(mutex);
        st_cond_signal(cv);
        st_mutex_unlock(mutex);
    }
}

static
void *st_manager_func(void *data)
{
    (void)data;
    int ret = st_manager_start();
    EXPECT_EQ(0, ret);

    return NULL;
}

class TestSTEasySetup_cb: public testing::Test
{
    protected:
        virtual void SetUp()
        {
            mutex = st_mutex_init();
            cv = st_cond_init();
            reset_storage();
            st_manager_initialize();
            st_register_status_handler(st_status_handler);
            t = st_thread_create(st_manager_func, "TEST", 0, NULL);
            test_wait_until(mutex, cv, 5);
            get_wildcard_acl_policy();
        }

        virtual void TearDown()
        {
            st_manager_quit();
            st_thread_destroy(t);
            st_manager_stop();
            st_manager_deinitialize();
            reset_storage();
            st_cond_destroy(cv);
            st_mutex_destroy(mutex);
            cv = NULL;
            mutex = NULL;
        }
};

static oc_endpoint_t *
get_endpoint(void)
{
    oc_endpoint_t *eps = oc_connectivity_get_endpoints(0);

    while (eps && ((eps->flags & oc_endpoint_t::transport_flags::TCP) ||
                   (eps->flags & oc_endpoint_t::transport_flags::IPV6))) {
        eps = eps->next;
    }

    EXPECT_NE(NULL, eps);

    return eps;
}

static oc_event_callback_retval_t
coap_receive_handler(void *data)
{
    oc_message_t *message = (oc_message_t *)data;
    coap_receive(message);
    oc_message_unref(message);
    return OC_EVENT_DONE;
}

static int
handle_prov_data(uint8_t *data, int len)
{
    if (!data || len <= 0) {
        return -1;
    }

    oc_message_t *message = oc_allocate_message();
    oc_endpoint_t *ep = get_endpoint();
    if (!message || !ep) {
        return -1;
    }

    memcpy(&message->endpoint, ep, sizeof(oc_endpoint_t));
    memcpy(message->data, data, len);
    message->length = len;

    oc_set_delayed_callback(message, coap_receive_handler, 0);

    return 0;
}

TEST_F(TestSTEasySetup_cb, easy_setup_prov_response_test)
{
    int ret = handle_prov_data(wifi_prov_data, wifi_prov_data_len);
    EXPECT_EQ(0, ret);
    ret = handle_prov_data(cloud_prov_data, cloud_prov_data_len);
    EXPECT_EQ(0, ret);
    _oc_signal_event_loop();

    ret = test_wait_until(mutex, cv, 5);
    EXPECT_EQ(0, ret);
}