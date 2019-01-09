/*
 * AP_PHM_Backend.cpp
 *
 *  Created on: Aug 2, 2018
 *      Author: Justin.Gravett
 */

#include "AP_PHM_Backend.h"

#define PHM_BACKEND_DEBUGGING 1

#if PHM_BACKEND_DEBUGGING
#define Debug(fmt, args ...) do { hal.console->printf("%s:%d: " fmt "\n", __FUNCTION__, __LINE__, ## args); hal.scheduler->delay(1); } while(0))
#else
#define Debug(fmt, args...)
#endif

// constructor
AP_PHM_Backend::AP_PHM_Backend(AP_PHM &_phm, AP_PHM::PHM_Status &_status) : phm(_phm), status(_status)
{
    status.phm_status = 0; // Initialize to nominal

    _new_status = false;
}

/*
  copy latest data to the frontend from a backend
 */
void AP_PHM_Backend::_copy_to_frontend(AP_PHM::PHM_Status _status)
{
    phm.last_status[phm.primary_instance] = phm.status[phm.primary_instance];
    phm.status[phm.primary_instance] = _status;
    status = _status;

    // Check if the PHM status has changed
    if (phm.last_status[phm.primary_instance].phm_status != _status.phm_status) {
        _new_status = true;
    }
    else {
        _new_status = false;
    }
}


