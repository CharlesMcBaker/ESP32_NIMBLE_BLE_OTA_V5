#include "ota.h"

#define LOG_OTA "OTA"

uint8_t gatt_svr_chr_ota_control_val;
uint8_t gatt_svr_chr_ota_data_val[512];

uint16_t ota_control_val_handle;
uint16_t ota_data_val_handle;

uint16_t num_pkgs_received = 0;
uint16_t packet_size = 0;

const esp_partition_t *update_partition;
esp_ota_handle_t update_handle;
bool updating = false;


#define REBOOT_DEEP_SLEEP_TIMEOUT 500


int gatt_svr_chr_write(struct os_mbuf *om, uint16_t min_len,uint16_t max_len, void *dst, uint16_t *len)
{
    uint16_t om_len;
    int rc;

    om_len = OS_MBUF_PKTLEN(om);

    if (om_len < min_len || om_len > max_len)
    {
        return BLE_ATT_ERR_INVALID_ATTR_VALUE_LEN;
    }

    rc = ble_hs_mbuf_to_flat(om, dst, max_len, len);

    if (rc != 0)
    {
        return BLE_ATT_ERR_UNLIKELY;
    }

    return 0;
}

void update_ota_control(uint16_t conn_handle)
{
  struct os_mbuf *om;
  esp_err_t err;

  // check which value has been received
    switch (gatt_svr_chr_ota_control_val)
    {
        case SVR_CHR_OTA_CONTROL_REQUEST:
        {
            ESP_LOGI(LOG_OTA, "OTA has been requested via BLE.");
            // get the next free OTA partition
            update_partition = esp_ota_get_next_update_partition(NULL);
            // start the ota update
            err = esp_ota_begin(update_partition, OTA_WITH_SEQUENTIAL_WRITES,&update_handle);
            if (err != ESP_OK)
            {
                ESP_LOGE(LOG_OTA, "esp_ota_begin failed (%s)",esp_err_to_name(err));
                esp_ota_abort(update_handle);
                gatt_svr_chr_ota_control_val = SVR_CHR_OTA_CONTROL_REQUEST_NAK;
            }
            else
            {
                gatt_svr_chr_ota_control_val = SVR_CHR_OTA_CONTROL_REQUEST_ACK;
                updating = true;

                // retrieve the packet size from OTA data
                packet_size = (gatt_svr_chr_ota_data_val[1] << 8) + gatt_svr_chr_ota_data_val[0];
                ESP_LOGI(LOG_OTA, "Packet size is: %d", packet_size);

                num_pkgs_received = 0;
            }

            // notify the client via BLE that the OTA has been acknowledged (or not)
            om = ble_hs_mbuf_from_flat(&gatt_svr_chr_ota_control_val, sizeof(gatt_svr_chr_ota_control_val));
            ble_gattc_notify_custom(conn_handle, ota_control_val_handle, om);
            ESP_LOGI(LOG_OTA, "OTA request acknowledgement has been sent.");

            break;
        }

        case SVR_CHR_OTA_CONTROL_DONE:
        {
            updating = false;

            // end the OTA and start validation
            err = esp_ota_end(update_handle);

            if (err != ESP_OK)
            {
                if (err == ESP_ERR_OTA_VALIDATE_FAILED)
                {
                    ESP_LOGE(LOG_OTA,"Image validation failed, image is corrupted!");
                }
                else
                {
                    ESP_LOGE(LOG_OTA, "esp_ota_end failed (%s)!", esp_err_to_name(err));
                }
            }
            else
            {
                // select the new partition for the next boot
                err = esp_ota_set_boot_partition(update_partition);
                if (err != ESP_OK)
                {
                    ESP_LOGE(LOG_OTA, "esp_ota_set_boot_partition failed (%s)!",esp_err_to_name(err));
                }
            }

            // set the control value
            if (err != ESP_OK)
            {
                gatt_svr_chr_ota_control_val = SVR_CHR_OTA_CONTROL_DONE_NAK;
            }
            else
            {
                gatt_svr_chr_ota_control_val = SVR_CHR_OTA_CONTROL_DONE_ACK;
            }

            // notify the client via BLE that DONE has been acknowledged
            om = ble_hs_mbuf_from_flat(&gatt_svr_chr_ota_control_val,sizeof(gatt_svr_chr_ota_control_val));
            ble_gattc_notify_custom(conn_handle, ota_control_val_handle, om);
            ESP_LOGI(LOG_OTA, "OTA DONE acknowledgement has been sent.");

            // restart the ESP to finish the OTA
            if (err == ESP_OK)
            {
                ESP_LOGI(LOG_OTA, "Preparing to restart!");
                vTaskDelay(pdMS_TO_TICKS(REBOOT_DEEP_SLEEP_TIMEOUT));
                esp_restart();
            }
            
            break;
        }

        default:
        break;
    }
}

int gatt_svr_chr_ota_control_cb(uint16_t conn_handle,uint16_t attr_handle,struct ble_gatt_access_ctxt *ctxt,void *arg)
{
    int rc;
    uint8_t length = sizeof(gatt_svr_chr_ota_control_val);

    switch (ctxt->op)
    {
      case BLE_GATT_ACCESS_OP_READ_CHR:
        // a client is reading the current value of ota control
        rc = os_mbuf_append(ctxt->om, &gatt_svr_chr_ota_control_val, length);
        return rc == 0 ? 0 : BLE_ATT_ERR_INSUFFICIENT_RES;
        break;

      case BLE_GATT_ACCESS_OP_WRITE_CHR:
        // a client is writing a value to ota control
        rc = gatt_svr_chr_write(ctxt->om, 1, length,
                                &gatt_svr_chr_ota_control_val, NULL);
        // update the OTA state with the new value
        update_ota_control(conn_handle);
        return rc;
        break;

      default:
        break;
    }

    // this shouldn't happen
    assert(0);
    return BLE_ATT_ERR_UNLIKELY;
}

int gatt_svr_chr_ota_data_cb(uint16_t conn_handle, uint16_t attr_handle,struct ble_gatt_access_ctxt *ctxt,void *arg)
{
  int rc;
  esp_err_t err;

    // store the received data into gatt_svr_chr_ota_data_val
   rc = gatt_svr_chr_write(ctxt->om, 1, sizeof(gatt_svr_chr_ota_data_val),gatt_svr_chr_ota_data_val, NULL);

   // write the received packet to the partition
  if (updating)
  {
    err = esp_ota_write(update_handle, (const void *)gatt_svr_chr_ota_data_val, packet_size);
    if (err != ESP_OK)
    {
    ESP_LOGE(LOG_OTA, "esp_ota_write failed (%s)!",esp_err_to_name(err));
    }

    num_pkgs_received++;
    ESP_LOGI(LOG_OTA, "Received packet %d", num_pkgs_received);
  }

  return rc;
}



void check_partition()
{
    const esp_partition_t *partition = esp_ota_get_running_partition();

    switch (partition->address)
        {
            case 0x00010000:
            ESP_LOGI(LOG_OTA, "Running partition: factory.................0x00010000");
            break;
            case 0x00110000:
            ESP_LOGI(LOG_OTA, "Running partition: ota_0...................0x00110000");
            break;
            case 0x00210000:
            ESP_LOGI(LOG_OTA, "Running partition: ota_1...................0x00110000");
            break;

            default:
            ESP_LOGI(LOG_OTA, "Running partition: unknown.................??????????");
            break;
        }
}

void check_ota()
{
    esp_ota_img_states_t ota_state;

    if (esp_ota_get_state_partition(partition, &ota_state) == ESP_OK)
    {
        if (ota_state == ESP_OTA_IMG_PENDING_VERIFY)
        {
            ESP_LOGI(LOG_OTA, "An OTA update has been detected..................");

            if (run_diagnostics())
            {
                ESP_LOGI(LOG_OTA,"Diagnostics completed successfully! Continuing execution.");
                esp_ota_mark_app_valid_cancel_rollback();
            }
            else
            {
                ESP_LOGE(LOG_OTA,"Diagnostics failed! Start rollback to the previous version.");
                esp_ota_mark_app_invalid_rollback_and_reboot();
            }

        }
    }
    else
    {
        ESP_LOGI(LOG_OTA, "NO OTA update is detected..................");
    }
}

bool run_diagnostics()
{
  // do some diagnostics
  return true;
}