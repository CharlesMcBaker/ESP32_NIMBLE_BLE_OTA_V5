#ifndef H_GATT_
#define H_GATT_

#include "nimble/ble.h"
#include "modlog/modlog.h"
#include <assert.h>

#include <string.h>
#include "host/ble_hs.h"
#include "host/ble_uuid.h"
#include "services/gap/ble_svc_gap.h"
#include "services/gatt/ble_svc_gatt.h"
#include "gapp.h"
#include "ota.h"

#include "modes.h"

#define GATT_DEVICE_INFO_UUID                   0x180A
#define GATT_MANUFACTURER_NAME_UUID             0x2A29
#define GATT_MODEL_NUMBER_UUID                  0x2A24

#define GATT_SERIAL_NUMBER_UUID                 0x2A25
#define GATT_HARDWARE_NUMBER_UUID               0x2A27
#define GATT_FIRMWARE_NUMBER_UUID               0x2A26
#define GATT_SYSYEM_ID_UUID                     0x2A23

#define GATT_BATTERY_UUID                       0x180F
#define GATT_BATTERY_LEVEL_UUID                 0X2A19
#define GATT_BATTERY_INFO_UUID                  0x2BEB

extern uint16_t first_handle;
extern uint16_t second_handle;
extern uint16_t battery_handle;

struct ble_hs_cfg;
struct ble_gatt_register_ctxt;

void gatt_svr_register_cb(struct ble_gatt_register_ctxt *ctxt, void *arg);
int gatt_svr_init(void);

#endif