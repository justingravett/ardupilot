/*
 * AP_PHM.h
 *
 *  Created on: Aug 2, 2018
 *      Author: Justin.Gravett
 */

#ifndef AP_PHM_H_
#define AP_PHM_H_

#include <AP_HAL/AP_HAL.h>

// maximum number of PHM instances
#define PHM_MAX_INSTANCES 1

// maximum number of drivers. Note that a single driver can provide
// multiple PHM instances
#define PHM_MAX_DRIVERS 1

class AP_PHM_Backend;

class AP_PHM {
    friend class AP_PHM_Backend;

public:

    // constructor
    AP_PHM();

    // initialize the PHM object, loading backend drivers
    bool init();

    // update the PHM object, asking backend to push date to the frontend
    void update(void);

    struct PHM_Status {
        uint16_t phm_status;
        // TODO: Add update counter and last timestamp
    };

    /// PHM status codes
    enum PHM_Status_Codes {
        PHM_NOMINAL         = 0,
        PHM_LOG_FAULT       = 1,
        PHM_WARNING         = 2,
        PHM_LOITER          = 3,
        PHM_RETURN_HOME     = 4,
        PHM_LAND_NOW        = 5,
        PHM_KILL_MOTORS     = 6,
    };

    //status
    PHM_Status get_phm_status(void) const { return get_phm_status(primary_instance); }
    PHM_Status get_phm_status(uint8_t instance) const { return status[instance]; }

    PHM_Status get_last_phm_status(void) const { return get_last_phm_status(primary_instance); }
    PHM_Status get_last_phm_status(uint8_t instance) const { return last_status[instance]; }

    bool is_new_status(void) const { return new_status; }

    // return number of registered PHMs
    uint8_t get_num_instances(void) const { return num_instances; }

private:
    // how many drivers do we have?
    PHM_Status status[PHM_MAX_INSTANCES]; // this is the current status message
    PHM_Status last_status[PHM_MAX_INSTANCES]; // this is the last status message
    AP_PHM_Backend *drivers[PHM_MAX_DRIVERS];

    // primary PHM instance
    uint8_t primary_instance;

    // number of PHM instances present
    uint8_t num_instances;

    bool new_status;
};


#endif /* AP_PHM_H_ */
