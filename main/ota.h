#ifndef H_OTA_
#define H_OTA_

#include "esp_log.h"
#include "esp_ota_ops.h"
#include "nvs_flash.h"

#include "host/ble_hs.h"
#include "host/ble_uuid.h"
#include "services/gap/ble_svc_gap.h"
#include "services/gatt/ble_svc_gatt.h"

#include "gatt.h"

void check_partition();
void check_ota();
bool run_diagnostics();

extern uint16_t ota_control_val_handle;
extern uint16_t ota_data_val_handle;

const esp_partition_t *partition;
esp_ota_img_states_t ota_state;

typedef enum 
{
    SVR_CHR_OTA_CONTROL_NOP,
    SVR_CHR_OTA_CONTROL_REQUEST,
    SVR_CHR_OTA_CONTROL_REQUEST_ACK,
    SVR_CHR_OTA_CONTROL_REQUEST_NAK,
    SVR_CHR_OTA_CONTROL_DONE,
    SVR_CHR_OTA_CONTROL_DONE_ACK,
    SVR_CHR_OTA_CONTROL_DONE_NAK,
} svr_chr_ota_control_val_t;


static const ble_uuid128_t OTA_UUID             = BLE_UUID128_INIT(0xd8, 0xe6, 0xfd, 0x1d, 0x4a, 024, 0xc6, 0xb1, 0x53, 0x4c,0x4c, 0x59, 0x6d, 0xd9, 0xf1, 0xd6);
static const ble_uuid128_t OTA_CONTROL_UUID     = BLE_UUID128_INIT(0x30, 0xd8, 0xe3, 0x3a, 0x0e, 0x27, 0x22, 0xb7, 0xa4, 0x46,0xc0, 0x21, 0xaa, 0x71, 0xd6, 0x7a);
static const ble_uuid128_t OTA_DATA_UUID        = BLE_UUID128_INIT(0xb0, 0xa5, 0xf8, 0x45, 0x8d, 0xca, 0x89, 0x9b, 0xd8, 0x4c,0x40, 0x1f, 0x88, 0x88, 0x40, 0x23);


int gatt_svr_chr_write(struct os_mbuf *om, uint16_t min_len,uint16_t max_len, void *dst, uint16_t *len);
int gatt_svr_chr_ota_control_cb(uint16_t conn_handle,uint16_t attr_handle,struct ble_gatt_access_ctxt *ctxt,void *arg);
int gatt_svr_chr_ota_data_cb(uint16_t conn_handle, uint16_t attr_handle,struct ble_gatt_access_ctxt *ctxt,void *arg);

#endif