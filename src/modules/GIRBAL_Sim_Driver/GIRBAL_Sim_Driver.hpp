#pragma once

#include <px4_platform_common/defines.h>
#include <px4_platform_common/module.h>
#include <px4_platform_common/module_params.h>
#include <px4_platform_common/posix.h>
#include <px4_platform_common/px4_work_queue/ScheduledWorkItem.hpp>
#include <px4_platform_common/log.h>


#include <drivers/drv_hrt.h>
#include <lib/perf/perf_counter.h>

#include <uORB/uORB.h>
#include <uORB/Publication.hpp>
#include <uORB/Subscription.hpp>
#include <uORB/SubscriptionCallback.hpp>
#include <uORB/topics/GIRBAL_anchor_distances.h>
//#include <uORB/topics/orb_test.h>
//#include <uORB/topics/parameter_update.h>
#include <uORB/topics/vehicle_gps_position.h>
//#include <uORB/topics/vehicle_status.h>

using namespace time_literals;

class GIRBAL_Sim_Driver : public ModuleBase<GIRBAL_Sim_Driver>, public ModuleParams, public px4::ScheduledWorkItem
{
public:
    // Instance variables
    struct coordinates_gps // struct for passing 3D coords around the module
    {
        int lat;
        int lon;
        int alt;
    };
    typedef struct coordinates_gps gpsCOORDS;

    struct coordinates_xyz // struct for passing 3D coords around the module
    {
        int x;
        int y;
        int z;
    };
    typedef struct coordinates_xyz xyzCOORDS;

    gpsCOORDS anchor_nodes_gps[4]; // coords arrays to store our anchors (in both GPS and XYZ systems)
    xyzCOORDS anchor_nodes_xyz[4];

    int *distances[4];

    GIRBAL_Sim_Driver(); // constructor

    ~GIRBAL_Sim_Driver() override; // destructor

    void publishDistances(int *distances[]);

    void calculateDistances(gpsCOORDS current_location, xyzCOORDS nodes[], int *distances[]);

    xyzCOORDS gps2ecef(gpsCOORDS gps);

    bool init(); // unsure how this differs from the constructor

private:
    void Run() override; // callback func

    // Publications
	uORB::Publication<GIRBAL_anchor_distances_s> _anchor_distance_pub{ORB_ID(GIRBAL_anchor_distances)}; //

    // Subscriptions
    uORB::SubscriptionCallbackWorkItem sensor_gps_s{this, ORB_ID(vehicle_gps_position)};        // subscription that schedules WorkItemExample when updated
    //uORB::SubscriptionInterval         _parameter_update_sub{ORB_ID(parameter_update), 1_s}; // subscription limited to 1 Hz updates
    //uORB::Subscription                 _vehicle_status_sub{ORB_ID(vehicle_status)};          // regular subscription for additional data

};
