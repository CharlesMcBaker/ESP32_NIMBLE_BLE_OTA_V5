#include "gatt.h"

static const char *manuf_name   = "Feel Robotics";
static const char *model_num    = "FeelChip-SX";
static const char *serial_num   = "Version 4";
static const char *hardware_num = "VA.3";
static const char *firmware_num = "V1.0";
static const char *system_id    = "CHARLIE";


uint16_t first_handle;
uint16_t second_handle;
uint16_t battery_handle;

static int gatt_svr_chr_access_device_info(uint16_t conn_handle, uint16_t attr_handle,struct ble_gatt_access_ctxt *ctxt, void *arg);
static int gatt_svr_chr_access_battery(uint16_t conn_handle, uint16_t attr_handle, struct ble_gatt_access_ctxt *ctxt, void *arg);

//      https://mynewt.apache.org/latest/tutorials/ble/bleprph/bleprph-sections/bleprph-svc-reg.html

static const struct ble_gatt_svc_def    INFO_SERVICE[] =                       // INFO SERVICE
{
    {
        /* Service: Device Information */
        .type = BLE_GATT_SVC_TYPE_PRIMARY,
        .uuid = BLE_UUID16_DECLARE(GATT_DEVICE_INFO_UUID),
        .characteristics = (struct ble_gatt_chr_def[])
        { 
            {
                /* Characteristic: * Manufacturer name */
                .uuid       = BLE_UUID16_DECLARE(GATT_MANUFACTURER_NAME_UUID),
                .access_cb  =   gatt_svr_chr_access_device_info,
                .flags      =   BLE_GATT_CHR_F_READ,
            }, 
            
            {
                /* Characteristic: Model number string */
                .uuid       =   BLE_UUID16_DECLARE(GATT_MODEL_NUMBER_UUID),
                .access_cb  =   gatt_svr_chr_access_device_info,
                .flags      =   BLE_GATT_CHR_F_READ,
            }, 

            {
                /* Characteristic: Model number string */
                .uuid       =   BLE_UUID16_DECLARE(GATT_SERIAL_NUMBER_UUID),
                .access_cb  =   gatt_svr_chr_access_device_info,
                .flags      =   BLE_GATT_CHR_F_READ,
            }, 

            {
                /* Characteristic: Model number string */
                .uuid       =   BLE_UUID16_DECLARE(GATT_HARDWARE_NUMBER_UUID),
                .access_cb  =   gatt_svr_chr_access_device_info,
                .flags      =   BLE_GATT_CHR_F_READ,
            }, 

            {
                /* Characteristic: Model number string */
                .uuid       =   BLE_UUID16_DECLARE(GATT_FIRMWARE_NUMBER_UUID),
                .access_cb  =   gatt_svr_chr_access_device_info,
                .flags      =   BLE_GATT_CHR_F_READ,
            }, 

            {
                /* Characteristic: Model number string */
                .uuid       =   BLE_UUID16_DECLARE(GATT_SYSYEM_ID_UUID),
                .access_cb  =   gatt_svr_chr_access_device_info,
                .flags      =   BLE_GATT_CHR_F_READ,
            }, 

            {
                0,  //  No more characteristics 
            },
        }
    },

    {
        0,          //  No more services
    },

};

static const struct ble_gatt_svc_def    ACCEL_SERVICE[] =                       // ACCEL SERVICE
{
    {
        //First Primary Service 
        .type = BLE_GATT_SVC_TYPE_PRIMARY,
        .uuid = BLE_UUID16_DECLARE(0x180),
        .characteristics = (struct ble_gatt_chr_def[])
        {   
            {
                //First Characherticic
                .uuid       =   BLE_UUID16_DECLARE(0x1900),
                .access_cb  =   gatt_svr_chr_access_battery,
                .val_handle =   &first_handle,
                .flags      =   BLE_GATT_CHR_F_NOTIFY,
            }, 
            
            {
                //Second Characherticic
                .uuid       =   BLE_UUID16_DECLARE(0x1901),
                .access_cb  =   gatt_svr_chr_access_battery,
                .flags      =   BLE_GATT_CHR_F_READ,
            }, 

            {
                0,  //  No more characteristics 
            },
        }
    },

    {
        0,          //  No more services
    },
};

static const struct ble_gatt_svc_def    BAT_SERVICE[] =                         // BAT SERVICE
{
    {    //Second Primary Service                                               
        .type = BLE_GATT_SVC_TYPE_PRIMARY,
        .uuid = BLE_UUID16_DECLARE(GATT_BATTERY_UUID),
        .characteristics = (struct ble_gatt_chr_def[])
        {   
            {   //First Characherticic
                .uuid = BLE_UUID16_DECLARE(GATT_BATTERY_LEVEL_UUID),            
                .access_cb  =   gatt_svr_chr_access_battery,
                .val_handle =   &battery_handle,
                .flags      =   BLE_GATT_CHR_F_NOTIFY | BLE_GATT_CHR_F_READ,
            }, 
            
            {   //Second Characherticic
                .uuid       =   BLE_UUID16_DECLARE(GATT_BATTERY_INFO_UUID),     
                .access_cb  =   gatt_svr_chr_access_battery,
                .flags      =   BLE_GATT_CHR_F_READ,
            }, 

            {
                0,  //  No more characteristics 
            },
        }
    },

    {
        0,          //  No more services
    },
};

static const struct ble_gatt_svc_def    OTA_SERVICE[] =                         // OTA SERVICE
{
    { 
        .type = BLE_GATT_SVC_TYPE_PRIMARY,
        .uuid = &OTA_UUID.u,
        .characteristics = (struct ble_gatt_chr_def[])
        {   
            { 
                .uuid       =   &OTA_CONTROL_UUID.u,              
                .access_cb  =   gatt_svr_chr_ota_control_cb,
                .flags      =   BLE_GATT_CHR_F_READ | BLE_GATT_CHR_F_WRITE | BLE_GATT_CHR_F_NOTIFY,
                .val_handle =   &ota_control_val_handle,
            }, 
            
            {   //Second Characherticic
                .uuid       =   &OTA_DATA_UUID.u,                 
                .access_cb  =   gatt_svr_chr_ota_data_cb,
                .flags      =   BLE_GATT_CHR_F_WRITE,
                .val_handle =   &ota_data_val_handle,
            }, 

            {
                0,  //  No more characteristics 
            },
        }
    },

    {
        0,          //  No more services
    },
};

static int gatt_svr_chr_access_battery(uint16_t conn_handle, uint16_t attr_handle, struct ble_gatt_access_ctxt *ctxt, void *arg)
{
    /* Sensor location, set to "Chest" */
  
    uint16_t uuid;
    int rc;

    static uint8_t databack = 50;

    uuid = ble_uuid_u16(ctxt->chr->uuid);

    if (uuid == GATT_BATTERY_UUID) 
    {
        rc = os_mbuf_append(ctxt->om, &databack, sizeof(databack));
        return rc == 0 ? 0 : BLE_ATT_ERR_INSUFFICIENT_RES;
    }

    
    if (uuid == GATT_BATTERY_LEVEL_UUID) 
    {
        rc = os_mbuf_append(ctxt->om, &databack, sizeof(databack));
        return rc == 0 ? 0 : BLE_ATT_ERR_INSUFFICIENT_RES;
    }

    
    if (uuid == GATT_BATTERY_INFO_UUID) 
    {
        rc = os_mbuf_append(ctxt->om, &databack, sizeof(databack));
        return rc == 0 ? 0 : BLE_ATT_ERR_INSUFFICIENT_RES;
    }


    assert(0);
    return BLE_ATT_ERR_UNLIKELY;
    printf("TEST: gatt_svr_chr_access_battery\n");

    return 1;
}







static int gatt_svr_chr_access_device_info(uint16_t conn_handle, uint16_t attr_handle,struct ble_gatt_access_ctxt *ctxt, void *arg)
{
    uint16_t uuid;
    int rc;

    uuid = ble_uuid_u16(ctxt->chr->uuid);

    if (uuid == GATT_MODEL_NUMBER_UUID) 
    {
        rc = os_mbuf_append(ctxt->om, model_num, strlen(model_num));
        return rc == 0 ? 0 : BLE_ATT_ERR_INSUFFICIENT_RES;
    }

    if (uuid == GATT_MANUFACTURER_NAME_UUID) 
    {
        rc = os_mbuf_append(ctxt->om, manuf_name, strlen(manuf_name));
        return rc == 0 ? 0 : BLE_ATT_ERR_INSUFFICIENT_RES;
    }

    if (uuid == GATT_SERIAL_NUMBER_UUID) 
    {
        rc = os_mbuf_append(ctxt->om, serial_num, strlen(serial_num));
        return rc == 0 ? 0 : BLE_ATT_ERR_INSUFFICIENT_RES;
    }

    if (uuid == GATT_HARDWARE_NUMBER_UUID) 
    {
        rc = os_mbuf_append(ctxt->om, hardware_num, strlen(hardware_num));
        return rc == 0 ? 0 : BLE_ATT_ERR_INSUFFICIENT_RES;
    }

    if (uuid == GATT_FIRMWARE_NUMBER_UUID) 
    {
        rc = os_mbuf_append(ctxt->om, firmware_num, strlen(firmware_num));
        return rc == 0 ? 0 : BLE_ATT_ERR_INSUFFICIENT_RES;
    }

    if (uuid == GATT_SYSYEM_ID_UUID) 
    {
        rc = os_mbuf_append(ctxt->om, system_id, strlen(system_id));
        return rc == 0 ? 0 : BLE_ATT_ERR_INSUFFICIENT_RES;
    }

    assert(0);
    return BLE_ATT_ERR_UNLIKELY;
    //return 1;
}

void gatt_svr_register_cb(struct ble_gatt_register_ctxt *ctxt, void *arg)
{
    char buf[BLE_UUID_STR_LEN];

    switch (ctxt->op)   //ble_gap_sync_state 
    {
        case BLE_GATT_REGISTER_OP_SVC:
        {
            MODLOG_DFLT(DEBUG, "registered service %s with handle=%d\n",ble_uuid_to_str(ctxt->svc.svc_def->uuid, buf),ctxt->svc.handle);
            break;
        }

        case BLE_GATT_REGISTER_OP_CHR:
        {
            MODLOG_DFLT(DEBUG, "registered characteristic %s with ""def_handle=%d val_handle=%d\n",ble_uuid_to_str(ctxt->chr.chr_def->uuid, buf),ctxt->chr.def_handle,ctxt->chr.val_handle);
            break;
        }

        case BLE_GATT_REGISTER_OP_DSC:
        {
            MODLOG_DFLT(DEBUG, "registered descriptor %s with handle=%d\n",ble_uuid_to_str(ctxt->dsc.dsc_def->uuid, buf),ctxt->dsc.handle);
            break;
        }

        default:
        assert(0);
        break;
    }
}

int gatt_svr_init(void)
{
    int rc;

    ble_svc_gap_init();
    ble_svc_gatt_init();

    ///////////////////////////////////////////////////////////////

    rc = ble_gatts_count_cfg(INFO_SERVICE);
    if (rc != 0) 
    {
        return rc;
    }

    rc = ble_gatts_add_svcs(INFO_SERVICE);
    if (rc != 0) 
    {
        return rc;
    }

    ///////////////////////////////////////////////////////////////

    rc = ble_gatts_count_cfg(BAT_SERVICE);
    if (rc != 0) 
    {
        return rc;
    }

    rc = ble_gatts_add_svcs(BAT_SERVICE);
    if (rc != 0) 
    {
        return rc;
    }

    /////////////////////////////////////////////////////////////// 

    rc = ble_gatts_count_cfg(ACCEL_SERVICE);
    if (rc != 0) 
    {
        return rc;
    }

    rc = ble_gatts_add_svcs(ACCEL_SERVICE);
    if (rc != 0) 
    {
        return rc;
    }
    
    /////////////////////////////////////////////////////////////// 

    rc = ble_gatts_count_cfg(OTA_SERVICE);
    if (rc != 0) 
    {
        return rc;
    }

    rc = ble_gatts_add_svcs(OTA_SERVICE);
    if (rc != 0) 
    {
        return rc;
    }
    
    ///////////////////////////////////////////////////////////////

    return 0;
}