#ifndef H_GAPP_
#define H_GAPP_

#include "modlog/modlog.h"
#include <assert.h>
#include <string.h>

#include "nvs_flash.h"

#include "host/ble_hs.h"
#include "host/ble_uuid.h"
#include "host/util/util.h"

#include "services/gap/ble_svc_gap.h"
#include "services/gatt/ble_svc_gatt.h"

//BLE
#include "nimble/nimble_port.h"
#include "nimble/nimble_port_freertos.h"
#include "nimble/ble.h"

#include "esp_nimble_hci.h"

#include "console/console.h"
#include "gatt.h"
#include "utill.h"
#include "esp_ota_ops.h"
#include "ota.h"

#include "modes.h"

int GAP_EVENT(struct ble_gap_event *event, void *arg);
void advertise(void);
void on_sync(void);
void BLE_Config(void);
void on_reset(int reason);
void host_task(void *param);

uint16_t conn_handle;

extern bool CONNECTION_STATE;

#endif