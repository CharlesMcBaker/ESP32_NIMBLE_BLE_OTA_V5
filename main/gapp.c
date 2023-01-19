#include "gapp.h"

bool notify_state;

uint8_t addr_type;
uint8_t manufacturer[11] = {0xA5, 0x05, 'K', 'I', 'I', 'R', 'O', 'O', ' ', 'B', 'V'};

const char *tag = "Kiiroo Control";
const char *device_name = "Xontrol C2";

bool CONNECTION_STATE = false;

void BLE_Config(void)
{
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) 
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
    ESP_ERROR_CHECK(esp_nimble_hci_and_controller_init());

    nimble_port_init();
    /* Initialize the NimBLE host configuration */
    ble_hs_cfg.sync_cb  = on_sync;
    ble_hs_cfg.reset_cb = on_reset;

    gatt_svr_init();                                    //from gatt.c 
    ble_svc_gap_device_name_set(device_name);
    nimble_port_freertos_init(host_task);
}



void advertise(void)
{
    struct ble_gap_adv_params adv_params;
    struct ble_hs_adv_fields fields;
    int rc;

    memset(&fields, 0, sizeof(fields));

    fields.flags = BLE_HS_ADV_F_DISC_GEN | BLE_HS_ADV_F_BREDR_UNSUP;
    fields.tx_pwr_lvl_is_present = 1;
    fields.tx_pwr_lvl = BLE_HS_ADV_TX_PWR_LVL_AUTO;

    fields.name = (uint8_t *)device_name;
    fields.name_len = strlen(device_name);
    fields.name_is_complete = 1;
    fields.mfg_data = manufacturer;
    fields.mfg_data_len = sizeof(manufacturer);  

    rc = ble_gap_adv_set_fields(&fields);
    if (rc != 0) 
    {
        MODLOG_DFLT(ERROR, "error setting advertisement data; rc=%d\n", rc);
        return;
    }

    memset(&adv_params, 0, sizeof(adv_params));

    adv_params.conn_mode = BLE_GAP_CONN_MODE_UND;
    adv_params.disc_mode = BLE_GAP_DISC_MODE_GEN;
    rc = ble_gap_adv_start(addr_type, NULL, BLE_HS_FOREVER,&adv_params, GAP_EVENT, NULL);

    if (rc != 0) 
    {
        MODLOG_DFLT(ERROR, "error enabling advertisement; rc=%d\n", rc);
        return;
    }
}

int GAP_EVENT(struct ble_gap_event *event, void *arg)
{
    switch (event->type) 
    {
        // {} denotes a new block of scope.
        case BLE_GAP_EVENT_CONNECT:
        {
             /* A new connection was established or a connection attempt failed */
            MODLOG_DFLT(INFO, "connection %s; status=%d\n",event->connect.status == 0 ? "established" : "failed",event->connect.status);
            CONNECTION_STATE = true;
            if (event->connect.status != 0) 
            {
                advertise();
            }
            conn_handle = event->connect.conn_handle;
            break;
        }
        
        case BLE_GAP_EVENT_DISCONNECT:
        {
            CONNECTION_STATE = false;
            MODLOG_DFLT(INFO, "disconnect; reason=%d\n", event->disconnect.reason);
            advertise();
            break;
        }

        case BLE_GAP_EVENT_ADV_COMPLETE:
        {
            MODLOG_DFLT(INFO, "adv complete\n");
            advertise(); 
            break;
        }

        case BLE_GAP_EVENT_SUBSCRIBE:
        {
            MODLOG_DFLT(INFO, "subscribe event; cur_notify=%d\n value handle; "  "val_handle=%d\n",event->subscribe.cur_notify, first_handle);
           
            ESP_LOGI("BLE_GAP_SUBSCRIBE_EVENT", "conn_handle from subscribe=%d", conn_handle);

            /////////// First Handle 
            if (event->subscribe.attr_handle == first_handle) 
            {
                notify_state = event->subscribe.cur_notify;
            } 
            else if (event->subscribe.attr_handle != first_handle) 
            {
                notify_state = event->subscribe.cur_notify;
            }

            /////////// Second Handle 
            if (event->subscribe.attr_handle == second_handle) 
            {
                notify_state = event->subscribe.cur_notify;
            } 
            else if (event->subscribe.attr_handle != second_handle) 
            {
                notify_state = event->subscribe.cur_notify;
            }

            /////////// Battery Handle  
            if (event->subscribe.attr_handle == battery_handle) 
            {
                notify_state = event->subscribe.cur_notify;
                ESP_LOGI("BLE_GAP_SUBSCRIBE_EVENT", "battery_handle from subscribe=%d", battery_handle);
            } 
            else if (event->subscribe.attr_handle != battery_handle) 
            {
                notify_state = event->subscribe.cur_notify;
                 ESP_LOGI("BLE_GAP_UNSUBSCRIBE_EVENT", "battery_handle from subscribe=%d", battery_handle);
            }

            break;
        }

        case BLE_GAP_EVENT_MTU:
        {
            //MODLOG_DFLT(INFO, "mtu update event; conn_handle=%d mtu=%d\n",event->mtu.connection_handle,event->mtu.value);
            break;
        }
    }

    return 0;
}

void on_sync(void)
{
    int rc;

    rc = ble_hs_id_infer_auto(0, &addr_type);
    assert(rc == 0);

    uint8_t addr_val[6] = {0};
    rc = ble_hs_id_copy_addr(addr_type, addr_val, NULL);

    MODLOG_DFLT(INFO, "Device Address: ");
    //print_addr(addr_val);
    MODLOG_DFLT(INFO, "\n");
    advertise();  // ADVERTISE!
}

void on_reset(int reason)
{
    MODLOG_DFLT(ERROR, "Resetting state; reason=%d\n", reason);
}

void host_task(void *param)
{
    /* This function will return only when nimble_port_stop() is executed */
    ESP_LOGI(tag, "BLE Host Task Started");
    nimble_port_run();
    nimble_port_freertos_deinit();
}