/*
 * AP_PHM_UAVCAN.h
 *
 *  Created on: Aug 2, 2018
 *      Author: Justin.Gravett
 */

//
// UAVCAN PHM Driver
//

#ifndef AP_PHM_UAVCAN_H_
#define AP_PHM_UAVCAN_H_

#include "AP_PHM_Backend.h"
#include <AP_UAVCAN/AP_UAVCAN.h>

class AP_PHM_UAVCAN : public AP_PHM_Backend {
public:
    AP_PHM_UAVCAN(AP_PHM &_phm, AP_PHM::PHM_Status &_status);
    ~AP_PHM_UAVCAN() override;

    bool read() override;

    // This method is called from UAVCAN thread
    void handle_phm_msg(const AP_PHM::PHM_Status &msg) override;

private:

    AP_PHM::PHM_Status _interm_status;
    AP_HAL::Semaphore *_sem_phm;
};


#endif /* AP_PHM_UAVCAN_H_ */
