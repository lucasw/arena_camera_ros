// Copyright 2015 <Magazino GmbH>

#include <pylon_camera/pylon_camera_parameter.h>
#include <string>

namespace pylon_camera
{

PylonCameraParameter::PylonCameraParameter() :
        device_user_id_(""),
        camera_frame_(""),
        desired_frame_rate_(-1.),
        target_exposure_(3000),
        start_exposure_(2000.0),
        mtu_size_(3000),
        binning_(1)
{
}

PylonCameraParameter::~PylonCameraParameter()
{
}

bool PylonCameraParameter::readFromRosParameterServer(ros::NodeHandle& nh)
{
    nh.param<std::string>("device_user_id", device_user_id_, "");
    nh.param<double>("desired_framerate", desired_frame_rate_, -1.);
    nh.param<std::string>("camera_frame", camera_frame_, "pylon_camera");
    nh.param<int>("gige/mtu_size", mtu_size_, 3000);

    // -1: AutoExposureContinuous
    //  0: AutoExposureOff
    // > 0: Exposure in micro-seconds
    nh.param<double>("start_exposure", start_exposure_, 35000.0);
    nh.param<int>("binning", binning_, 1);

    return validateParameterSet(nh);
}

bool PylonCameraParameter::validateParameterSet(ros::NodeHandle& nh)
{
    if (!device_user_id_.empty())
    {
        ROS_INFO("Using Camera: %s", device_user_id_.c_str());
    }
    else
    {
        ROS_INFO("No Device User ID set -> Will use the camera device found fist");
    }

    if (binning_ > 4 || binning_ < 1)
    {
        ROS_ERROR("Invalid binning settings!");
        return false;
    }

    if (desired_frame_rate_ < 0 && desired_frame_rate_ != -1)
    {
        desired_frame_rate_ = -1.0;
        nh.setParam("desired_framerate", desired_frame_rate_);
        ROS_ERROR("Unexpected framerate (%f). Setting to -1 (max possible)",
                  desired_frame_rate_);
    }

    return true;
}

}  // namespace pylon_camera
