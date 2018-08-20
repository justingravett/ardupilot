/*
 * AP_PHM_UAVCAN.cpp
 *
 *  Created on: Aug 2, 2018
 *      Author: Justin.Gravett
 */
//
//  UAVCAN PHM driver
//

#include <AP_HAL/AP_HAL.h>

#if HAL_WITH_UAVCAN

#include <AP_BoardConfig/AP_BoardConfig.h>
#include "AP_PHM_UAVCAN.h"

extern const AP_HAL::HAL& hal;

#define debug_phm_uavcan(level, fmt, args...) do { if ((level) <= AP_BoardConfig::get_can_debug()) { hal.console->printf(fmt, ##args); }} while (0)

// There is limitation to use only one UAVCAN phm now.

/*
  constructor - registers instance at top GenSet driver
 */
AP_PHM_UAVCAN::AP_PHM_UAVCAN(AP_PHM &_phm, AP_PHM::PHM_Status &_status) :
    AP_PHM_Backend(_phm, _status)
{
    _new_status = false;
    _sem_phm = hal.util->new_semaphore();
}

// For each instance we need to deregister from AP_UAVCAN class
AP_PHM_UAVCAN::~AP_PHM_UAVCAN()
{
    if (hal.can_mgr != nullptr)
    {
        AP_UAVCAN *ap_uavcan = hal.can_mgr->get_UAVCAN();
        if (ap_uavcan != nullptr)
        {
            ap_uavcan->remove_phm_listener(this);

            debug_phm_uavcan(2, "AP_PHM_UAVCAN destructed\n\r");
        }
    }
}

// Consume new data and mark it received
bool AP_PHM_UAVCAN::read(void)
{
    if (_sem_phm->take(0)) {
        _copy_to_frontend(_interm_status);
        _sem_phm->give();
        return true;
    }
    return false;
}

void AP_PHM_UAVCAN::handle_phm_msg(const AP_PHM::PHM_Status &msg)
{
    if (_sem_phm->take(0)) {
        _interm_status = msg;
        _sem_phm->give();
    }
}

#endif // HAL_WITH_UAVCAN



