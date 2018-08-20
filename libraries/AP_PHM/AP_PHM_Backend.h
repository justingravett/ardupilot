/*
 * AP_PHM_Backend.h
 *
 *  Created on: Aug 2, 2018
 *      Author: Justin.Gravett
 */

#ifndef AP_PHM_BACKEND_H_
#define AP_PHM_BACKEND_H_

#include "AP_PHM.h"

class AP_PHM_Backend {
public:
    AP_PHM_Backend(AP_PHM &_phm, AP_PHM::PHM_Status &_status);
    virtual ~AP_PHM_Backend(void) {};

    // The read() method is the only one needed in each driver. It
    // should return true when the backend has successfully received a
    // valid packet.
    virtual bool read() = 0;

    // callback for UAVCAN messages
    virtual void handle_phm_msg(const AP_PHM::PHM_Status &msg) { return; }

    bool is_new_status() {return _new_status;}

protected:
    AP_PHM &phm;                    ///< access to frontend (for parameters)
    AP_PHM::PHM_Status &status;     ///< public state for this instance

    bool _new_status;

    void _copy_to_frontend(AP_PHM::PHM_Status _status);
};


#endif /* AP_PHM_BACKEND_H_ */
