/// Main.cpp

#include <OpenNI.h>
#include <iostream>
#include <vector>
#include <librealsense2/rs.hpp> // Include RealSense Cross Platform API
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>     // Basic OpenCV structures (cv::Mat)
#include <opencv2/videoio.hpp>  // Video write

#include "DepthCamera.h"

#define NUM_FRAMES 1000

int main() {
    //initialize openni sdk and rs context
    openni::Status rc = openni::OpenNI::initialize();
    if (rc != openni::STATUS_OK)
    {
        printf("Initialization of OpenNi failed\n%s\n", openni::OpenNI::getExtendedError());
        return 1;
    }

    rs2::context ctx;

    // Get devices

    openni::Array<openni::DeviceInfo> orbbec_devices;
    rs2::device_list rs_devices = RealSenseCamera::getAvailableDevices(ctx);
    OrbbecCamera::getAvailableDevices(&orbbec_devices);

    // Initialise Devices
    std::vector<DepthCamera*> depthCameras;
    std::vector<std::string> windows;
    int id = 0;

    for (int i = 0; i < orbbec_devices.getSize(); i++) {
        auto dev = &orbbec_devices[i];
        try {
            depthCameras.push_back(new OrbbecCamera(dev));
            windows.push_back("Window " + std::to_string(id++));
        }
        catch (const std::system_error& ex) {
            std::cout << ex.code() << '\n';
            std::cout << ex.code().message() << '\n';
            std::cout << ex.what() << '\n';
        }
    }

    for (auto&& dev : rs_devices)
    {
        depthCameras.push_back(new RealSenseCamera(&ctx, &dev));
        windows.push_back("Window " + std::to_string(id++));
    }

    auto count = 0;
    std::vector<cv::Mat> frames {};
    cv::Mat result;
    while (cv::waitKey(1) < 0 && count++ < NUM_FRAMES) {
        std::cout << "\r" << count << " / " << NUM_FRAMES << " Frames (" << 100 * count / NUM_FRAMES << "%)";
        try {
            frames.clear();
            for (DepthCamera* cam : depthCameras) {
                frames.push_back(cam->getFrame());
            }

            for (int i = 0; i < frames.size(); i++) {
                cv::imshow(windows[i], frames[i]);
                cv::resizeWindow(windows[i], frames[i].size[1], frames[i].size[0]);
            }
        }
        catch (...) {
            std::cout << " | An Exception occured";
        }
        std::cout << std::flush;
    }

    //Shutdown

    openni::OpenNI::shutdown();
    
    return 0;
}
