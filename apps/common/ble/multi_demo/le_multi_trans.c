/*********************************************************************************************
    *   Filename        : le_multi_trans.c.c

    *   Description     :

    *   Author          :

    *   Email           : zh-jieli.com

    *   Last modifiled  : 2017-01-17 11:14

    *   Copyright:(c)JIELI  2011-2016  @ , All Rights Reserved.
*********************************************************************************************/

// *****************************************************************************
/* EXAMPLE_START(le_counter): LE Peripheral - Heartbeat Counter over GATT
 *
 * @text All newer operating systems provide GATT Client functionality.
 * The LE Counter examples demonstrates how to specify a minimal GATT Database
 * with a custom GATT Service and a custom Characteristic that sends periodic
 * notifications.
 */
// *****************************************************************************
#include "system/app_core.h"
#include "system/includes.h"

#include "app_config.h"

#include "btstack/btstack_task.h"
#include "btstack/bluetooth.h"
#include "btcontroller_modules.h"
#include "bt_common.h"

#include "le_common.h"
#include "ble_user.h"
#include "btstack/btstack_event.h"
#include "le_multi_common.h"

#define MUTIL_ROLE_IS_SERVER      1
#include "le_multi_trans.h"

#if (TCFG_BLE_DEMO_SELECT == DEF_BLE_DEMO_MULTI && SUPPORT_MAX_SERVER)
//TRANS ANCS
#define TRANS_ANCS_EN  			  	 0  //for ios ancs function

#define TEST_SEND_DATA_RATE          0  //测试上行发送数据
#define TEST_SEND_HANDLE_VAL         ATT_CHARACTERISTIC_ae02_01_VALUE_HANDLE
#define EXT_ADV_MODE_EN              0
#define TEST_AUDIO_DATA_UPLOAD       0 //测试文件上传

#if LE_DEBUG_PRINT_EN
extern void printf_buf(u8 *buf, u32 len);
//#define log_info            y_printf
#define log_info(x, ...)  y_printf("[LE_MUL_TRANS]" x " ", ## __VA_ARGS__)
#define log_info_hexdump  printf_buf
#else
#define log_info(...)
#define log_info_hexdump(...)
#endif

//----------------------------------------------------------------------------------------

#if TEST_SEND_DATA_RATE
static u32 test_data_count;
static u32 server_timer_handle = 0;
static u8 test_data_start;
#endif

//---------------
// 广播周期 (unit:0.625ms)
#define ADV_INTERVAL_MIN          (160*1)
//---------------
#define HOLD_LATENCY_CNT_MIN      (3)  //(0~0xffff)
#define HOLD_LATENCY_CNT_MAX      (15) //(0~0xffff)
#define HOLD_LATENCY_CNT_ALL      (0xffff)
//---------------
//加密设置
/* static const uint8_t sm_min_key_size = 7; */

//连接参数更新请求设置
//是否使能参数请求更新,0--disable, 1--enable
static const uint8_t connection_update_enable = 1; ///0--disable, 1--enable
//当前请求的参数表index
static uint8_t connection_update_cnt[SUPPORT_MAX_SERVER]; //

//参数表
static const struct conn_update_param_t connection_param_table[] = {
    {16, 24, 10, 600},//11
    {12, 28, 10, 600},//3.7
    {8,  20, 10, 600},
    /* {12, 28, 4, 600},//3.7 */
    /* {12, 24, 30, 600},//3.05 */
};

//共可用的参数组数
#define CONN_PARAM_TABLE_CNT      (sizeof(connection_param_table)/sizeof(struct conn_update_param_t))

#define EIR_TAG_STRING   0xd6, 0x05, 0x08, 0x00, 'J', 'L', 'A', 'I', 'S', 'D','K'
static const char user_tag_string[] = {EIR_TAG_STRING};

static u8 adv_data_len;
static u8 adv_data[ADV_RSP_PACKET_MAX];//max is 31
static u8 scan_rsp_data_len;
static u8 scan_rsp_data[ADV_RSP_PACKET_MAX];//max is 31

static char gap_device_name[BT_NAME_LEN_MAX] = "jl_ble_test";
static u8 gap_device_name_len = 0; //名字长度，不包含结束符
static u8 ble_work_state[SUPPORT_MAX_SERVER];      //ble 状态变化
static u8 adv_ctrl_en;             //广播控制
static u8 test_read_write_buf[4];

static void (*app_recieve_callback)(void *priv, void *buf, u16 len) = NULL;
static void (*app_ble_state_callback)(void *priv, ble_state_e state) = NULL;
static void (*ble_resume_send_wakeup)(void) = NULL;
static u32 channel_priv;

static s8 cur_dev_cid; // 当前操作的多机id
//------------------------------------------------------
static int app_send_user_data_check(u16 len);
static int app_send_user_data(u16 conn_handle, u16 handle, u8 *data, u16 len, u8 handle_type);
//广播参数设置
static int get_buffer_vaild_len(void *priv);
extern const char *bt_get_local_name();
//------------------------------------------------------

//for ANCS
#if TRANS_ANCS_EN
#define ANCS_SUBEVENT_CLIENT_NOTIFICATION        (0xF1)
void ancs_client_init(void);
void ancs_client_register_callback(btstack_packet_handler_t callback);
const char *ancs_client_attribute_name_for_id(int id);
void ancs_set_notification_buffer(u8 *buffer, u16 buffer_size);

//ancs info buffer
#define ANCS_INFO_BUFFER_SIZE                    (1024)
static u8 ancs_info_buffer[ANCS_INFO_BUFFER_SIZE];
#endif

//------------------------------------------------------
#if TEST_AUDIO_DATA_UPLOAD
static const u8 test_audio_data_file[1024] = {
    1, 2, 3, 4, 5, 6, 7, 8, 9
};

#define AUDIO_ONE_PACKET_LEN                     (128)
static void test_send_audio_data(int init_flag)
{
    static u32 send_pt = 0;
    static u32 start_flag = 0;

    if (!server_con_handle[cur_dev_cid]) {
        return;
    }

    if (init_flag) {
        log_info("audio send init\n");
        send_pt = 0;
        start_flag = 1;
    }

    if (!start_flag) {
        return;
    }

    u32 file_size = sizeof(test_audio_data_file);
    u8 *file_ptr = test_audio_data_file;

    if (send_pt >= file_size) {
        log_info("audio send Complete\n");
        start_flag = 0;
        return;
    }

    u32 send_len = file_size - send_pt;
    if (send_len > AUDIO_ONE_PACKET_LEN) {
        send_len = AUDIO_ONE_PACKET_LEN;
    }

    while (1) {
        if (app_send_user_data_check(send_len)) {
            log_info("audio send %08x\n", send_pt);
            if (app_send_user_data(server_con_handle[cur_dev_cid], ATT_CHARACTERISTIC_ae3c_01_VALUE_HANDLE, &file_ptr[send_pt], send_len, ATT_OP_AUTO_READ_CCC)) {
                log_info("audio send fail!\n");
                break;
            } else {
                send_pt += send_len;
            }
        } else {
            break;
        }
    }
}
#endif


static void send_request_connect_parameter(u8 table_index, u8 cid)
{
    struct conn_update_param_t *param = (void *)&connection_param_table[table_index];//static ram

    if (server_con_handle[cid]) {
        log_info("update_request:%04x----%d-%d-%d-%d-\n", server_con_handle[cid], param->interval_min, param->interval_max, param->latency, param->timeout);
        ble_op_conn_param_request(server_con_handle[cid], param);
    }
}

static void check_connetion_updata_deal(u8 cid)
{
    if (connection_update_enable) {
        if (connection_update_cnt[cid] < CONN_PARAM_TABLE_CNT) {
            send_request_connect_parameter(connection_update_cnt[cid], cid);
        }
    }
}

static void connection_update_complete_success(u16 conn_handle, u8 *packet)
{
    int conn_interval, conn_latency, conn_timeout;

    conn_interval = hci_subevent_le_connection_update_complete_get_conn_interval(packet);
    conn_latency = hci_subevent_le_connection_update_complete_get_conn_latency(packet);
    conn_timeout = hci_subevent_le_connection_update_complete_get_supervision_timeout(packet);

    log_info("conn_handle = %04x\n", conn_handle);
    log_info("conn_interval = %d\n", conn_interval);
    log_info("conn_latency = %d\n", conn_latency);
    log_info("conn_timeout = %d\n", conn_timeout);
}


static void set_ble_work_state(u8 cid, ble_state_e state)
{
    if (state != ble_work_state[cid]) {
        log_info("ble_work_st[%d]:%x->%x\n", cid, ble_work_state[cid], state);
        ble_work_state[cid] = state;
        if (app_ble_state_callback) {
            app_ble_state_callback((void *)channel_priv, state);
        }
    }
}

static ble_state_e get_ble_work_state(u8 cid)
{
    return ble_work_state[cid];
}

#if TEST_SEND_DATA_RATE
static void server_timer_handler(void *priv)
{
    if (!server_con_handle[cur_dev_cid]) {
        test_data_count = 0;
        test_data_start = 0;
        return;
    }

    log_info("peer_rssi = %d\n", ble_vendor_get_peer_rssi(server_con_handle[cur_dev_cid]));

    if (test_data_count) {
        log_info("\n%d bytes send: %d.%02d KB/s \n", test_data_count, test_data_count / 1000, test_data_count % 1000);
        test_data_count = 0;
    }
}

static void server_timer_start(void)
{
    if (server_timer_handle) {
        return;
    }

    server_timer_handle = sys_timer_add(NULL, server_timer_handler, 1000);
}

static void server_timer_stop(void)
{
    if (server_timer_handle) {
        sys_timeout_del(server_timer_handle);
        server_timer_handle = 0;
    }
}

static void test_data_send_packet(void)
{
    u32 vaild_len = get_buffer_vaild_len(0);//获取发送buffer可写入的数据
    if (!test_data_start) {
        return;
    }

    if (vaild_len) {
        if (!app_send_user_data(server_con_handle[cur_dev_cid], TEST_SEND_HANDLE_VAL, (void *)&test_data_count, vaild_len, ATT_OP_AUTO_READ_CCC)) {
            test_data_count += vaild_len;
        }
    }
}
#endif

static void can_send_now_wakeup(void)
{
    /* putchar('E'); */
    if (!mul_dev_have_connected(MULTI_ROLE_SERVER)) {
        return;
    }

    if (ble_resume_send_wakeup) {
        ble_resume_send_wakeup();
    }

#if TEST_SEND_DATA_RATE
    test_data_send_packet();
#endif

#if TEST_AUDIO_DATA_UPLOAD
    test_send_audio_data(0);
#endif
}

static void ble_auto_shut_down_enable(u8 enable)
{
}

static const char *const phy_result[] = {
    "None",
    "1M",
    "2M",
    "Coded",
};

static void set_connection_data_length(u16 tx_octets, u16 tx_time)
{
    if (server_con_handle[cur_dev_cid]) {
        ble_op_set_data_length(server_con_handle[cur_dev_cid], tx_octets, tx_time);
    }
}

static void set_connection_data_phy(u8 tx_phy, u8 rx_phy)
{
    if (0 == server_con_handle[cur_dev_cid]) {
        return;
    }

    u8 all_phys = 0;
    u16 phy_options = CONN_SET_PHY_OPTIONS_S8;
    ble_op_set_ext_phy(server_con_handle[cur_dev_cid], all_phys, tx_phy, rx_phy, phy_options);
}

static void server_profile_start(u16 conn_handle)
{
#if BT_FOR_APP_EN
    set_app_connect_type(TYPE_BLE);
#endif

    set_ble_work_state(cur_dev_cid, BLE_ST_CONNECT);
    ble_auto_shut_down_enable(0);

    /* set_connection_data_phy(CONN_SET_CODED_PHY, CONN_SET_CODED_PHY); */
}

_WEAK_
u8 ble_update_get_ready_jump_flag(void)
{
    return 0;
}

//尝试开新设备广播
int ble_enable_new_dev_adv(void)
{
    if (!server_con_handle[cur_dev_cid]) {
        log_info("dev_doing\n");
        return -1;
    }

    s8 tmp_cid = mul_get_idle_dev_index(MULTI_ROLE_SERVER);
    if (tmp_cid == MULTI_INVAIL_INDEX) {
        log_info("no idle dev to do!!!\n");
        return -2;
    }

    log_info("new_dev_adv\n");
    cur_dev_cid = tmp_cid;
    set_ble_work_state(cur_dev_cid, BLE_ST_IDLE);
    bt_ble_adv_enable(1);
    return 0;
}

void trans_deal_passkey_input(u32 *key, u16 conn_handle)
{
    //0~999999
    *key = 123456;
    log_info("con_handle= %04x,new_key: %06u.\n", conn_handle, *key);
}

void trans_cbk_sm_packet_handler(uint8_t packet_type, uint16_t channel, uint8_t *packet, uint16_t size)
{
    sm_just_event_t *event = (void *)packet;
    u32 tmp32;
    //log_info("%s\n", __FUNCTION__);

    switch (packet_type) {
    case HCI_EVENT_PACKET:
        switch (hci_event_packet_get_type(packet)) {
        case SM_EVENT_JUST_WORKS_REQUEST:
            sm_just_works_confirm(sm_event_just_works_request_get_handle(packet));
            log_info("Just Works Confirmed.\n");
            break;

        case SM_EVENT_PASSKEY_DISPLAY_NUMBER:
            log_info_hexdump(packet, size);
            memcpy(&tmp32, event->data, 4);
            log_info("Passkey display: %06u.\n", tmp32);
            break;

        case SM_EVENT_PASSKEY_INPUT_NUMBER:
            /*IO_CAPABILITY_KEYBOARD_ONLY 方式*/
            trans_deal_passkey_input(&tmp32, event->con_handle);
            log_info("%04x->Passkey input: %06u.\n", event->con_handle, tmp32);
            sm_passkey_input(event->con_handle, tmp32); /*update passkey*/
            break;

        case SM_EVENT_PAIR_PROCESS:
            log_info("======PAIR_PROCESS: %02x\n", event->data[0]);
            put_buf(event->data, 4);
            break;
        }
        break;
    }
}

/*
 * @section Packet Handler
 *
 * @text The packet handler is used to:
 *        - stop the counter after a disconnect
 *        - send a notification when the requested ATT_EVENT_CAN_SEND_NOW is received
 */
/* LISTING_START(packetHandler): Packet Handler */
void trans_cbk_packet_handler(uint8_t packet_type, uint16_t channel, uint8_t *packet, uint16_t size)
{
    int mtu;
    u32 tmp;
    u8 status;
    const char *attribute_name;

    switch (packet_type) {
    case HCI_EVENT_PACKET:
        switch (hci_event_packet_get_type(packet)) {

        /* case DAEMON_EVENT_HCI_PACKET_SENT: */
        /* break; */
        case ATT_EVENT_HANDLE_VALUE_INDICATION_COMPLETE:
            log_info("ATT_EVENT_HANDLE_VALUE_INDICATION_COMPLETE\n");
        case ATT_EVENT_CAN_SEND_NOW:
            can_send_now_wakeup();
            break;

        case HCI_EVENT_LE_META:
            switch (hci_event_le_meta_get_subevent_code(packet)) {
            case HCI_SUBEVENT_LE_ENHANCED_CONNECTION_COMPLETE: {
                status = hci_subevent_le_enhanced_connection_complete_get_status(packet);
                if (status) {
                    log_info("LE_SLAVE CONNECTION FAIL!!! %0x\n", status);
                    set_ble_work_state(cur_dev_cid, BLE_ST_DISCONN);
                    break;
                }

                u16 tmp_handle1 = hci_subevent_le_enhanced_connection_complete_get_connection_handle(packet);
                log_info("HCI_SUBEVENT_LE_ENHANCED_CONNECTION_COMPLETE : %0x\n", tmp_handle1);

                s8 tmp_dev_cid1 = mul_get_idle_dev_index(MULTI_ROLE_SERVER);
                if (tmp_dev_cid1 == MULTI_INVAIL_INDEX) {
                    log_info("dev_cid full!!!\n");
                    ble_op_disconnect(tmp_handle1);
                    break;
                }

                cur_dev_cid = tmp_dev_cid1;
                server_con_handle[cur_dev_cid] = tmp_handle1;
                connection_update_cnt[cur_dev_cid] = 0;
                ble_op_multi_att_send_conn_handle(server_con_handle[cur_dev_cid], cur_dev_cid, MULTI_ROLE_SERVER);

                log_info("conn_interval = %d\n", hci_subevent_le_enhanced_connection_complete_get_conn_interval(packet));
                log_info("conn_latency = %d\n", hci_subevent_le_enhanced_connection_complete_get_conn_latency(packet));
                log_info("conn_timeout = %d\n", hci_subevent_le_enhanced_connection_complete_get_supervision_timeout(packet));
                server_profile_start(server_con_handle[cur_dev_cid]);
            }
            break;

            case HCI_SUBEVENT_LE_CONNECTION_COMPLETE: {
                if (0 == hci_subevent_le_connection_complete_get_role(packet)) {
                    //connect is master
                    break;
                }

                if (packet[3]) {
                    log_info("LE_SLAVE CONNECTION FAIL!!! %0x\n", packet[3]);
                    set_ble_work_state(cur_dev_cid, BLE_ST_DISCONN);
                    bt_ble_adv_enable(1);
                    break;
                }

                u16 tmp_handle = hci_subevent_le_connection_complete_get_connection_handle(packet);
                log_info("HCI_SUBEVENT_LE_CONNECTION_COMPLETE: %0x\n", tmp_handle);

                s8 tmp_dev_cid = mul_get_idle_dev_index(MULTI_ROLE_SERVER);

                if (tmp_dev_cid == MULTI_INVAIL_INDEX) {
                    log_info("dev_cid full!!!\n");
                    ble_op_disconnect(tmp_handle);
                    break;
                }

                cur_dev_cid = tmp_dev_cid;
                server_con_handle[cur_dev_cid] = tmp_handle;
                connection_update_cnt[cur_dev_cid] = 0;
                ble_op_multi_att_send_conn_handle(server_con_handle[cur_dev_cid], cur_dev_cid, MULTI_ROLE_SERVER);

                connection_update_complete_success(tmp_handle, packet + 8);
                server_profile_start(server_con_handle[cur_dev_cid]);

#if RCSP_BTMATE_EN
#if (defined(BT_CONNECTION_VERIFY) && (0 == BT_CONNECTION_VERIFY))
                JL_rcsp_auth_reset();
#endif
                //rcsp_dev_select(RCSP_BLE);
                rcsp_init();
#endif
                log_info("ble remote rssi= %d\n", ble_vendor_get_peer_rssi(server_con_handle[cur_dev_cid]));
            }
            break;

            case HCI_SUBEVENT_LE_CONNECTION_UPDATE_COMPLETE: {
                u16 tmp_handle = little_endian_read_16(packet, 4);
                if (mul_get_dev_index(tmp_handle, MULTI_ROLE_SERVER) >= 0) {
                    connection_update_complete_success(tmp_handle, packet);
                }
            }
            break;

            case HCI_SUBEVENT_LE_DATA_LENGTH_CHANGE:
                log_info("APP HCI_SUBEVENT_LE_DATA_LENGTH_CHANGE\n");
                /* set_connection_data_phy(CONN_SET_CODED_PHY, CONN_SET_CODED_PHY); */
                break;

            case HCI_SUBEVENT_LE_PHY_UPDATE_COMPLETE:
                log_info("APP HCI_SUBEVENT_LE_PHY_UPDATE %s\n", hci_event_le_meta_get_phy_update_complete_status(packet) ? "Fail" : "Succ");
                log_info("Tx PHY: %s\n", phy_result[hci_event_le_meta_get_phy_update_complete_tx_phy(packet)]);
                log_info("Rx PHY: %s\n", phy_result[hci_event_le_meta_get_phy_update_complete_rx_phy(packet)]);
                break;
            }
            break;

        case HCI_EVENT_DISCONNECTION_COMPLETE: {
            u16 tmp_handle = little_endian_read_16(packet, 3);
            s8 tmp_index = mul_get_dev_index(tmp_handle, MULTI_ROLE_SERVER);
            if (tmp_index == MULTI_INVAIL_INDEX) {
                log_info("unknown_handle:%04x\n", tmp_handle);
                break;
            }

            log_info("HCI_EVENT_DISCONNECTION_COMPLETE: %0x\n", packet[5]);
            multi_att_clear_ccc_config(tmp_handle);
            server_con_handle[tmp_index] = 0;
            set_ble_work_state(tmp_index, BLE_ST_DISCONN);
            ble_op_multi_att_send_conn_handle(0, tmp_index, MULTI_ROLE_SERVER);

            if (0 == server_con_handle[cur_dev_cid]) {
                //判断当前设备是否正在ADV
                if (BLE_ST_ADV == get_ble_work_state(cur_dev_cid)) {
                    //直接退出，不做别的操作
                    break;
                }
            }

#if RCSP_BTMATE_EN
            rcsp_exit();
#endif

            //配设备开scan
            cur_dev_cid = tmp_index;

            if (!ble_update_get_ready_jump_flag()) {
                bt_ble_adv_enable(1);
            }

#if BT_FOR_APP_EN
            set_app_connect_type(TYPE_NULL);
#endif
            ble_auto_shut_down_enable(1);
        }
        break;

        case ATT_EVENT_MTU_EXCHANGE_COMPLETE: {
            u16 tmp_handle = little_endian_read_16(packet, 2);
            s8 tmp_cid = mul_get_dev_index(tmp_handle, MULTI_ROLE_SERVER);
            if (tmp_cid != MULTI_INVAIL_INDEX) {
                mtu = att_event_mtu_exchange_complete_get_MTU(packet) - 3;
                log_info("ATT MTU = %u\n", mtu);
                ble_op_multi_att_set_send_mtu(tmp_handle, mtu);
                /* set_connection_data_length(251, 2120); */
            }
        }
        break;

        case HCI_EVENT_VENDOR_REMOTE_TEST:
            log_info("--- HCI_EVENT_VENDOR_REMOTE_TEST\n");
            break;

        case L2CAP_EVENT_CONNECTION_PARAMETER_UPDATE_RESPONSE: {
            u16 tmp_handle = little_endian_read_16(packet, 2);
            s8 tmp_cid = mul_get_dev_index(tmp_handle, MULTI_ROLE_SERVER);
            if (tmp_cid != MULTI_INVAIL_INDEX) {
                tmp = little_endian_read_16(packet, 4);
                log_info("-update_rsp:%04x, %02x\n", tmp_handle, tmp);
                if (tmp) {
                    connection_update_cnt[tmp_cid]++;
                    log_info("remoter reject!!!\n");
                    check_connetion_updata_deal(tmp_cid);
                } else {
                    connection_update_cnt[tmp_cid] = CONN_PARAM_TABLE_CNT;
                }

            }
        }
        break;

        case HCI_EVENT_ENCRYPTION_CHANGE: {
            u16 tmp_handle = little_endian_read_16(packet, 3);
            s8 tmp_cid = mul_get_dev_index(tmp_handle, MULTI_ROLE_SERVER);
            if (tmp_cid != MULTI_INVAIL_INDEX) {
                log_info("HCI_EVENT_ENCRYPTION_CHANGE= %d\n", packet[2]);
            }
        }
        break;

#if TRANS_ANCS_EN
        case HCI_EVENT_ANCS_META:
            switch (hci_event_ancs_meta_get_subevent_code(packet)) {
            case ANCS_SUBEVENT_CLIENT_NOTIFICATION:
                printf("ANCS_SUBEVENT_CLIENT_NOTIFICATION \n");
                attribute_name = ancs_client_attribute_name_for_id(ancs_subevent_client_notification_get_attribute_id(packet));
                if (!attribute_name) {
                    printf("ancs unknow attribute_id :%d \n", ancs_subevent_client_notification_get_attribute_id(packet));
                    break;
                } else {
                    u16 attribute_strlen = little_endian_read_16(packet, 7);
                    u8 *attribute_str = (void *)little_endian_read_32(packet, 9);
                    printf("Notification: %s - %s \n", attribute_name, attribute_str);
                }
                break;
            default:
                break;
            }

            break;
#endif

        }
        break;
    }
}


/* LISTING_END */

/*
 * @section ATT Read
 *
 * @text The ATT Server handles all reads to constant data. For dynamic data like the custom characteristic, the registered
 * att_read_callback is called. To handle long characteristics and long reads, the att_read_callback is first called
 * with buffer == NULL, to request the total value length. Then it will be called again requesting a chunk of the value.
 * See Listing attRead.
 */

/* LISTING_START(attRead): ATT Read */

// ATT Client Read Callback for Dynamic Data
// - if buffer == NULL, don't copy data, just return size of value
// - if buffer != NULL, copy data and return number bytes copied
// @param offset defines start of attribute value
static uint16_t att_read_callback(hci_con_handle_t connection_handle, uint16_t att_handle, uint16_t offset, uint8_t *buffer, uint16_t buffer_size)
{

    uint16_t  att_value_len = 0;
    uint16_t handle = att_handle;

    log_info("read_callback,conn_handle =%04x, handle=%04x,buffer=%08x\n", connection_handle, handle, (u32)buffer);

    switch (handle) {
    case ATT_CHARACTERISTIC_2a00_01_VALUE_HANDLE:
        att_value_len = gap_device_name_len;

        if ((offset >= att_value_len) || (offset + buffer_size) > att_value_len) {
            break;
        }

        if (buffer) {
            memcpy(buffer, &gap_device_name[offset], buffer_size);
            att_value_len = buffer_size;
            log_info("\n------read gap_name: %s \n", gap_device_name);
        }
        break;

    case ATT_CHARACTERISTIC_ae10_01_VALUE_HANDLE:
        att_value_len = sizeof(test_read_write_buf);
        if ((offset >= att_value_len) || (offset + buffer_size) > att_value_len) {
            break;
        }

        if (buffer) {
            memcpy(buffer, &test_read_write_buf[offset], buffer_size);
            att_value_len = buffer_size;
        }
        break;

    case ATT_CHARACTERISTIC_ae04_01_CLIENT_CONFIGURATION_HANDLE:
    case ATT_CHARACTERISTIC_ae02_01_CLIENT_CONFIGURATION_HANDLE:
    case ATT_CHARACTERISTIC_ae05_01_CLIENT_CONFIGURATION_HANDLE:
    case ATT_CHARACTERISTIC_ae3c_01_CLIENT_CONFIGURATION_HANDLE:
        if (buffer) {
            buffer[0] = multi_att_get_ccc_config(connection_handle, handle);
            buffer[1] = 0;
        }
        att_value_len = 2;
        break;

    default:
        break;
    }

    log_info("att_value_len= %d\n", att_value_len);

    return att_value_len;
}


/* LISTING_END */
/*
 * @section ATT Write
 *
 * @text The only valid ATT write in this example is to the Client Characteristic Configuration, which configures notification
 * and indication. If the ATT handle matches the client configuration handle, the new configuration value is stored and used
 * in the heartbeat handler to decide if a new value should be sent. See Listing attWrite.
 */

/* LISTING_START(attWrite): ATT Write */
static int att_write_callback(hci_con_handle_t connection_handle, uint16_t att_handle, uint16_t transaction_mode, uint16_t offset, uint8_t *buffer, uint16_t buffer_size)
{
    int result = 0;
    u16 tmp16;

    u16 handle = att_handle;

    log_info("write_callback,conn_handle =%04x, handle =%04x,size =%d\n", connection_handle, handle, buffer_size);

    switch (handle) {

    case ATT_CHARACTERISTIC_2a00_01_VALUE_HANDLE:
        break;

    case ATT_CHARACTERISTIC_ae02_01_CLIENT_CONFIGURATION_HANDLE:
#if TEST_SEND_DATA_RATE
        if (buffer[0]) {
            server_timer_start();
        } else {
            server_timer_stop();
            test_data_start = 0;
        }
#endif
    case ATT_CHARACTERISTIC_ae04_01_CLIENT_CONFIGURATION_HANDLE:
    case ATT_CHARACTERISTIC_ae05_01_CLIENT_CONFIGURATION_HANDLE:
    case ATT_CHARACTERISTIC_ae3c_01_CLIENT_CONFIGURATION_HANDLE:
        set_ble_work_state(cur_dev_cid, BLE_ST_NOTIFY_IDICATE);
        check_connetion_updata_deal(cur_dev_cid);
        log_info("\n------write ccc:%04x,%02x\n", handle, buffer[0]);
        multi_att_set_ccc_config(connection_handle, handle, buffer[0]);

#if TEST_SEND_DATA_RATE
        test_data_start = 1;//start
        if (buffer[0]) {
            test_data_send_packet();
        }
#endif
        //被使能通知后,尝试开新设备广播
        ble_enable_new_dev_adv();
        break;

    case ATT_CHARACTERISTIC_ae10_01_VALUE_HANDLE:
        tmp16 = sizeof(test_read_write_buf);
        if ((offset >= tmp16) || (offset + buffer_size) > tmp16) {
            break;
        }
        memcpy(&test_read_write_buf[offset], buffer, buffer_size);
        break;

    case ATT_CHARACTERISTIC_ae01_01_VALUE_HANDLE:
        printf("\n-ae01_rx(%d):", buffer_size);
        log_info_hexdump(buffer, buffer_size);

        //收发测试，自动发送收到的数据;for test
        if (app_send_user_data_check(buffer_size)) {
            app_send_user_data(connection_handle, ATT_CHARACTERISTIC_ae02_01_VALUE_HANDLE, buffer, buffer_size, ATT_OP_AUTO_READ_CCC);
        }

#if TEST_SEND_DATA_RATE
        if ((buffer[0] == 'A') && (buffer[1] == 'F')) {
            test_data_start = 1;//start
        } else if ((buffer[0] == 'A') && (buffer[1] == 'A')) {
            test_data_start = 0;//stop
        }
#endif
        break;

    case ATT_CHARACTERISTIC_ae03_01_VALUE_HANDLE:
        printf("\n-ae_rx(%d):", buffer_size);
        log_info_hexdump(buffer, buffer_size);

        //收发测试，自动发送收到的数据;for test
        if (app_send_user_data_check(buffer_size)) {
            app_send_user_data(connection_handle, ATT_CHARACTERISTIC_ae05_01_VALUE_HANDLE, buffer, buffer_size, ATT_OP_AUTO_READ_CCC);
        }
        break;

#if RCSP_BTMATE_EN
    case ATT_CHARACTERISTIC_ae02_02_CLIENT_CONFIGURATION_HANDLE:
        ble_op_latency_skip(server_con_handle[cur_dev_cid], HOLD_LATENCY_CNT_ALL); //
        set_ble_work_state(cur_dev_cid, BLE_ST_NOTIFY_IDICATE);
#endif
        /* if ((cur_conn_latency == 0) */
        /*     && (connection_update_cnt == CONN_PARAM_TABLE_CNT) */
        /*     && (Peripheral_Preferred_Connection_Parameters[0].latency != 0)) { */
        /*     connection_update_cnt = 0; */
        /* } */
        check_connetion_updata_deal(cur_dev_cid);
        log_info("\n------write ccc:%04x,%02x\n", handle, buffer[0]);
        multi_att_set_ccc_config(connection_handle, handle, buffer[0]);
        break;

#if RCSP_BTMATE_EN
    case ATT_CHARACTERISTIC_ae01_02_VALUE_HANDLE:
        printf("rcsp_read:%x\n", buffer_size);
        if (app_recieve_callback) {
            app_recieve_callback(0, buffer, buffer_size);
        }
        break;
#endif

    case ATT_CHARACTERISTIC_ae3b_01_VALUE_HANDLE:
        printf("\n-ae3b_rx(%d):", buffer_size);
        log_info_hexdump(buffer, buffer_size);

#if TEST_AUDIO_DATA_UPLOAD
        if (0 == memcmp(buffer, "start", 5)) {
            test_send_audio_data(1);
        }
#endif
        break;

    default:
        break;
    }

    return 0;
}

static int app_send_user_data(u16 conn_handle, u16 handle, u8 *data, u16 len, u8 handle_type)
{
    u32 ret = APP_BLE_NO_ERROR;

    if (!conn_handle) {
        return APP_BLE_OPERATION_ERROR;
    }

    if (!multi_att_get_ccc_config(conn_handle, handle + 1)) {
        log_info("fail,no write ccc!!!,%04x\n", handle + 1);
        return APP_BLE_NO_WRITE_CCC;
    }

    ret = ble_op_multi_att_send_data(conn_handle, handle, data, len, handle_type);

    if (ret == BLE_BUFFER_FULL) {
        ret = APP_BLE_BUFF_FULL;
    }

    if (ret) {
        log_info("app_send_fail:%d !!!\n", ret);
    }
    return ret;
}

//------------------------------------------------------
static int make_set_adv_data(void)
{
    u8 offset = 0;
    u8 *buf = adv_data;

    /* offset += make_eir_packet_val(&buf[offset], offset, HCI_EIR_DATATYPE_FLAGS, 0x18, 1); */
    /* offset += make_eir_packet_val(&buf[offset], offset, HCI_EIR_DATATYPE_FLAGS, 0x1A, 1); */
    offset += make_eir_packet_val(&buf[offset], offset, HCI_EIR_DATATYPE_FLAGS, 0x06, 1);
    offset += make_eir_packet_val(&buf[offset], offset, HCI_EIR_DATATYPE_COMPLETE_16BIT_SERVICE_UUIDS, 0xAF30, 2);

    if (offset > ADV_RSP_PACKET_MAX) {
        puts("***adv_data overflow!!!!!!\n");
        return -1;
    }
    log_info("adv_data(%d):", offset);
    log_info_hexdump(buf, offset);
    adv_data_len = offset;
    ble_op_set_adv_data(offset, buf);
    return 0;
}

static int make_set_rsp_data(void)
{
    u8 offset = 0;
    u8 *buf = scan_rsp_data;

#if RCSP_BTMATE_EN
    u8  tag_len = sizeof(user_tag_string);
    offset += make_eir_packet_data(&buf[offset], offset, HCI_EIR_DATATYPE_MANUFACTURER_SPECIFIC_DATA, (void *)user_tag_string, tag_len);
#endif

    u8 name_len = gap_device_name_len;
    u8 vaild_len = ADV_RSP_PACKET_MAX - (offset + 2);
    if (name_len > vaild_len) {
        name_len = vaild_len;
    }
    offset += make_eir_packet_data(&buf[offset], offset, HCI_EIR_DATATYPE_COMPLETE_LOCAL_NAME, (void *)gap_device_name, name_len);

    if (offset > ADV_RSP_PACKET_MAX) {
        puts("***rsp_data overflow!!!!!!\n");
        return -1;
    }

    log_info("rsp_data(%d):", offset);
    log_info_hexdump(buf, offset);
    scan_rsp_data_len = offset;
    ble_op_set_rsp_data(offset, buf);
    return 0;
}

//广播参数设置
static void advertisements_setup_init()
{
    uint8_t adv_type = ADV_IND;
    uint8_t adv_channel = ADV_CHANNEL_ALL;
    int   ret = 0;

    ble_op_set_adv_param(ADV_INTERVAL_MIN, adv_type, adv_channel);

    ret |= make_set_adv_data();
    ret |= make_set_rsp_data();

    if (ret) {
        puts("advertisements_setup_init fail !!!!!!\n");
    }
}


#if EXT_ADV_MODE_EN

#define EXT_ADV_NAME                    'J', 'L', '_', 'E', 'X', 'T', '_', 'A', 'D', 'V'
/* #define EXT_ADV_NAME                    "JL_EXT_ADV" */
#define BYTE_LEN(x...)                  sizeof((u8 []) {x})
#define EXT_ADV_DATA                    \
    0x02, 0x01, 0x06, \
    0x03, 0x02, 0xF0, 0xFF, \
    BYTE_LEN(EXT_ADV_NAME) + 1, BLUETOOTH_DATA_TYPE_COMPLETE_LOCAL_NAME, EXT_ADV_NAME

struct ext_advertising_param {
    u8 Advertising_Handle;
    u16 Advertising_Event_Properties;
    u8 Primary_Advertising_Interval_Min[3];
    u8 Primary_Advertising_Interval_Max[3];
    u8 Primary_Advertising_Channel_Map;
    u8 Own_Address_Type;
    u8 Peer_Address_Type;
    u8 Peer_Address[6];
    u8 Advertising_Filter_Policy;
    u8 Advertising_Tx_Power;
    u8 Primary_Advertising_PHY;
    u8 Secondary_Advertising_Max_Skip;
    u8 Secondary_Advertising_PHY;
    u8 Advertising_SID;
    u8 Scan_Request_Notification_Enable;
} _GNU_PACKED_;

struct ext_advertising_data  {
    u8 Advertising_Handle;
    u8 Operation;
    u8 Fragment_Preference;
    u8 Advertising_Data_Length;
    u8 Advertising_Data[BYTE_LEN(EXT_ADV_DATA)];
} _GNU_PACKED_;

struct ext_advertising_enable {
    u8  Enable;
    u8  Number_of_Sets;
    u8  Advertising_Handle;
    u16 Duration;
    u8  Max_Extended_Advertising_Events;
} _GNU_PACKED_;

const struct ext_advertising_param ext_adv_param = {
    .Advertising_Handle = 0,
    .Advertising_Event_Properties = 1,
    .Primary_Advertising_Interval_Min = {30, 0, 0},
    .Primary_Advertising_Interval_Max = {30, 0, 0},
    .Primary_Advertising_Channel_Map = 7,
    .Primary_Advertising_PHY = ADV_SET_1M_PHY,
    .Secondary_Advertising_PHY = ADV_SET_1M_PHY,
};

const struct ext_advertising_data ext_adv_data = {
    .Advertising_Handle = 0,
    .Operation = 3,
    .Fragment_Preference = 0,
    .Advertising_Data_Length = BYTE_LEN(EXT_ADV_DATA),
    .Advertising_Data = EXT_ADV_DATA,
};

const struct ext_advertising_enable ext_adv_enable = {
    .Enable = 1,
    .Number_of_Sets = 1,
    .Advertising_Handle = 0,
    .Duration = 0,
    .Max_Extended_Advertising_Events = 0,
};

const struct ext_advertising_enable ext_adv_disable = {
    .Enable = 0,
    .Number_of_Sets = 1,
    .Advertising_Handle = 0,
    .Duration = 0,
    .Max_Extended_Advertising_Events = 0,
};

#endif /* EXT_ADV_MODE_EN */

static int multi_set_adv_enable(void *priv, u32 en)
{
    ble_state_e next_state, cur_state;

    if (!adv_ctrl_en && en) {
        return APP_BLE_OPERATION_ERROR;
    }

    if (server_con_handle[cur_dev_cid]) {
        return APP_BLE_OPERATION_ERROR;
    }

    if (en) {
        next_state = BLE_ST_ADV;
    } else {
        next_state = BLE_ST_IDLE;
    }

    cur_state =  get_ble_work_state(cur_dev_cid);
    switch (cur_state) {
    case BLE_ST_ADV:
    case BLE_ST_IDLE:
    case BLE_ST_INIT_OK:
    case BLE_ST_NULL:
    case BLE_ST_DISCONN:
        break;
    default:
        return APP_BLE_OPERATION_ERROR;
        break;
    }

    if (cur_state == next_state) {
        return APP_BLE_NO_ERROR;
    }
    log_info("adv_en:%d\n", en);
    set_ble_work_state(cur_dev_cid, next_state);

#if EXT_ADV_MODE_EN
    if (en) {
        ble_op_set_ext_adv_param(&ext_adv_param, sizeof(ext_adv_param));

        log_info_hexdump(&ext_adv_data, sizeof(ext_adv_data));
        ble_op_set_ext_adv_data(&ext_adv_data, sizeof(ext_adv_data));

        ble_op_set_ext_adv_enable(&ext_adv_enable, sizeof(ext_adv_enable));
    } else {
        ble_op_set_ext_adv_enable(&ext_adv_disable, sizeof(ext_adv_disable));
    }
#else
    if (en) {
        advertisements_setup_init();
    }
    ble_op_adv_enable(en);
#endif /* EXT_ADV_MODE_EN */

    return APP_BLE_NO_ERROR;
}

static int trans_disconnect_conn(void *priv)
{
    u8 cid = (u8) priv;

    if (cid >= SUPPORT_MAX_SERVER) {
        return APP_BLE_OPERATION_ERROR;
    }

    if (server_con_handle[cid]) {
        if (BLE_ST_SEND_DISCONN != get_ble_work_state(cid)) {
            log_info(">>>ble(%d) send disconnect\n", cid);
            set_ble_work_state(cid, BLE_ST_SEND_DISCONN);
            ble_op_disconnect(server_con_handle[cid]);
        } else {
            log_info(">>>ble(%d) wait disconnect...\n", cid);
        }
        return APP_BLE_NO_ERROR;
    } else {
        return APP_BLE_OPERATION_ERROR;
    }
}

static int get_buffer_vaild_len(void *priv)
{
    u32 vaild_len = 0;
    ble_op_multi_att_get_remain(server_con_handle[cur_dev_cid], &vaild_len);
    return vaild_len;
}

static int app_send_user_data_do(void *priv, u8 *data, u16 len)
{
#if PRINT_DMA_DATA_EN
    if (len < 128) {
        log_info("-le_tx(%d):");
        log_info_hexdump(data, len);
    } else {
        putchar('L');
    }
#endif
    return app_send_user_data(server_con_handle[cur_dev_cid], ATT_CHARACTERISTIC_ae02_01_VALUE_HANDLE, data, len, ATT_OP_AUTO_READ_CCC);
}

static int app_send_user_data_check(u16 len)
{
    u32 buf_space = get_buffer_vaild_len(0);
    if (len <= buf_space) {
        return 1;
    }
    return 0;
}

static int regiest_wakeup_send(void *priv, void *cbk)
{
    ble_resume_send_wakeup = cbk;
    return APP_BLE_NO_ERROR;
}

static int regiest_recieve_cbk(void *priv, void *cbk)
{
    channel_priv = (u32)priv;
    app_recieve_callback = cbk;
    return APP_BLE_NO_ERROR;
}

static int regiest_state_cbk(void *priv, void *cbk)
{
    channel_priv = (u32)priv;
    app_ble_state_callback = cbk;
    return APP_BLE_NO_ERROR;
}

//------------------------------------------------------------------------------

u8 *ble_get_scan_rsp_ptr(u16 *len)
{
    if (len) {
        *len = scan_rsp_data_len;
    }
    return scan_rsp_data;
}

u8 *ble_get_adv_data_ptr(u16 *len)
{
    if (len) {
        *len = adv_data_len;
    }
    return adv_data;
}

u8 *ble_get_gatt_profile_data(u16 *len)
{
    *len = sizeof(profile_data);
    return (u8 *)profile_data;
}

void bt_ble_adv_enable(u8 enable)
{
    multi_set_adv_enable(0, enable);
}

u16 bt_ble_is_connected(void)
{
    return server_con_handle[cur_dev_cid];
}

void ble_trans_module_enable(u8 en)
{
    log_info("mode_en:%d\n", en);
    if (en) {
        adv_ctrl_en = 1;
        bt_ble_adv_enable(1);
    } else {
        adv_ctrl_en = 0;
        bt_ble_adv_enable(0);
        ble_multi_trans_disconnect();
    }
}

void server_profile_init(void)
{
    /* setup ATT server */
    att_server_init(profile_data, att_read_callback, att_write_callback);
    att_server_register_packet_handler(trans_cbk_packet_handler);

#if TRANS_ANCS_EN
    //setup ANCS clent
    ancs_client_init();
    ancs_set_notification_buffer(ancs_info_buffer, sizeof(ancs_info_buffer));
    ancs_client_register_callback(&trans_cbk_packet_handler);
#endif
}

static const char ble_ext_name[] = "(BLE)";

void bt_multi_trans_init(void)
{
    const char *name_p;
    u8 ext_name_len = sizeof(ble_ext_name) - 1;

    name_p = bt_get_local_name();
    gap_device_name_len = strlen(name_p);
    if (gap_device_name_len > BT_NAME_LEN_MAX - ext_name_len) {
        gap_device_name_len = BT_NAME_LEN_MAX - ext_name_len;
    }

    //增加后缀，区分名字
    memcpy(gap_device_name, name_p, gap_device_name_len);
    memcpy(&gap_device_name[gap_device_name_len], "(BLE)", ext_name_len);
    gap_device_name_len += ext_name_len;

    log_info("ble name(%d): %s \n", gap_device_name_len, gap_device_name);

    cur_dev_cid = 0;
    memset(&ble_work_state, BLE_ST_INIT_OK, SUPPORT_MAX_SERVER);
    memset(&server_con_handle, 0, SUPPORT_MAX_SERVER << 1);

    set_ble_work_state(cur_dev_cid, BLE_ST_INIT_OK);
    ble_trans_module_enable(1);

#if TEST_SEND_DATA_RATE
    server_timer_start();
#endif
}

void bt_multi_trans_exit(void)
{
    ble_trans_module_enable(0);

#if TEST_SEND_DATA_RATE
    server_timer_stop();
#endif
}

void ble_multi_trans_disconnect(void)
{
    int count;

    for (int i = 0; i < SUPPORT_MAX_SERVER; i++) {
        count = 150;
        if (server_con_handle[i]) {
            trans_disconnect_conn((void *)i);
            while (count-- && server_con_handle[i]) {
                os_time_dly(1);
                putchar('w');
            }
        }
    }
}

#if RCSP_BTMATE_EN
static int rcsp_send_user_data_do(void *priv, u8 *data, u16 len)
{
    log_info("rcsp_tx:%x\n", len);
#if PRINT_DMA_DATA_EN
    if (len < 128) {
        log_info("-dma_tx(%d):");
        log_info_hexdump(data, len);
    } else {
        putchar('L');
    }
#endif
    return app_send_user_data(server_con_handle[cur_dev_cid], ATT_CHARACTERISTIC_ae02_02_VALUE_HANDLE, data, len, ATT_OP_AUTO_READ_CCC);
}
#endif

static const struct ble_server_operation_t mi_ble_operation = {
    .adv_enable = multi_set_adv_enable,
    .disconnect = trans_disconnect_conn,
    .get_buffer_vaild = get_buffer_vaild_len,
#if RCSP_BTMATE_EN
    .send_data = (void *)rcsp_send_user_data_do,
#else
    .send_data = (void *)app_send_user_data_do,
#endif
    .regist_wakeup_send = regiest_wakeup_send,
    .regist_recieve_cbk = regiest_recieve_cbk,
    .regist_state_cbk = regiest_state_cbk,
};

void ble_get_server_operation_table(struct ble_server_operation_t **interface_pt)
{
    *interface_pt = (void *)&mi_ble_operation;
}

//切换已连接多机
int le_multi_server_switch_connect_cid(u8 cid)
{
    if (cid < SUPPORT_MAX_SERVER && server_con_handle[cid]) {
        cur_dev_cid = cid;
        return 0;
    } else {
        log_info("cid not exist!!!\n");
        return -1;
    }
}

#endif

