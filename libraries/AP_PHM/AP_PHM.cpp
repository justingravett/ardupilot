/*
 * AP_PHM.cpp
 *
 *  Created on: Aug 2, 2018
 *      Author: Justin.Gravett
 */

//
// AP_PHM.cpp - PHM driver
//

#include "AP_PHM.h"

#include <utility>

#include <AP_Common/AP_Common.h>
#include <AP_HAL/AP_HAL.h>
#include <AP_BoardConfig/AP_BoardConfig.h>
#include <AP_Vehicle/AP_Vehicle_type.h>

#if HAL_WITH_UAVCAN
#include "AP_PHM_UAVCAN.h"
#endif

extern const AP_HAL::HAL& hal;

// AP_PHM constructor
AP_PHM::AP_PHM()
{
    primary_instance = 0;
    num_instances = 0;
    new_status = false;
}

// Initialize the PHM driver, loading the backend drivers
bool AP_PHM::init()
{
    AP_PHM_Backend *new_phm = nullptr;

#if HAL_WITH_UAVCAN
    // If there is place left - allocate one UAVCAN based PHM
    if ((AP_BoardConfig::get_can_enable() != 0) && (hal.can_mgr != nullptr))
    {
        printf("Creating AP_PHM_UAVCAN\n\r");
        new_phm = new AP_PHM_UAVCAN(*this, status[0]);

        // register new listener at first empty node
        if (hal.can_mgr != nullptr) {
            AP_UAVCAN *ap_uavcan = hal.can_mgr->get_UAVCAN();
            if (ap_uavcan != nullptr) {
                ap_uavcan->register_phm_listener(new_phm, 0);

                if (AP_BoardConfig::get_can_debug() >= 2) {
                    hal.console->printf("AP_PHM_UAVCAN registered\n\r");
                    printf("AP_PHM_UAVCAN registered!\n\r");
                }
            }
        }
    }
#endif

    // check to make sure phm was created
    if (new_phm != nullptr)
    {
        num_instances = 1;
        drivers[primary_instance] = new_phm;

        return true;
    }
    else
    {
        AP_BoardConfig::sensor_config_error("AP_PHM::init - unable to initialize driver");
        printf("AP_PHM::init - unable to initialize driver\n");

        return false;
    }
}

// Call Update on all drivers
void AP_PHM::update(void)
{
    uint8_t instance = primary_instance;

    if (drivers[instance] == nullptr)
    {
        printf("No PHM backends available\n\r");
        return;
    }

    // we have an active driver for this instance
    if (!drivers[instance]->read()) {
        printf("Failed to read from PHM backend driver\n\r");
        return;
    }

    new_status = drivers[instance]->is_new_status();
}
