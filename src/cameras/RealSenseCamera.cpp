#include "RealsenseCamera.h"
#include <iostream>
#include <exception>
#include "obj/PointCloud.h"
#include <utilities/Consts.h>

// TODO: Add depth tuning

rs2::device_list RealSenseCamera::getAvailableDevices(rs2::context ctx) {
	return ctx.query_devices();
}

std::vector<RealSenseCamera*> RealSenseCamera::initialiseAllDevices(Camera* cam, Renderer *renderer, int *starting_id) {
	rs2::context ctx;

	std::vector<RealSenseCamera*> depthCameras;

	for (auto&& dev : RealSenseCamera::getAvailableDevices(ctx))
	{
		depthCameras.push_back(new RealSenseCamera(&ctx, &dev, cam, renderer, (*starting_id)++));
		std::cout << "Initialised " << depthCameras.back()->getCameraName() << std::endl;
	}

	return depthCameras;
}

RealSenseCamera::RealSenseCamera(rs2::context *ctx, rs2::device *device, Camera *cam, Renderer *renderer, int camera_id)
	:
	mp_Context(ctx),
	m_Device(*device)
{
	mp_Pipe = std::make_shared<rs2::pipeline>(*ctx);
	m_CameraId = camera_id;

	printDeviceInfo();
	m_Device.query_sensors();
	m_Config.enable_device(m_Device.get_info(RS2_CAMERA_INFO_SERIAL_NUMBER));
	
	auto profile = mp_Pipe->start(m_Config);

	rs2::frameset data = mp_Pipe->wait_for_frames(); // Wait for next set of frames from the camera
	rs2::frame depth = data.get_depth_frame();

	// Get Intrinsics
	auto depth_profile = depth.get_profile().as<rs2::video_stream_profile>();
	m_Intrinsics = depth_profile.get_intrinsics();
	
	// Query frame size (width and height)
	m_DepthWidth = m_Intrinsics.width;
	m_DepthHeight = m_Intrinsics.height;

	rs2::depth_frame depth_frame = depth.as<rs2::depth_frame>();

	m_PointCloud = std::make_unique<GLObject::PointCloud>(this, cam, renderer, depth_frame.get_units());
}

RealSenseCamera::~RealSenseCamera() {
	printf("Shutting down [Realsense] %s...\n", getCameraName().c_str()); 

	if (m_Device.as<rs2::recorder>()) {
		stopRecording();
	}

	try {
		mp_Pipe->stop();
	}
	catch (...) {
		std::cout << "An exception occured while shutting down [Realsense] Camera " << getCameraName();
	}
}

const void *RealSenseCamera::getDepth()
{
	if (!m_Device.as<rs2::playback>()) {
		rs2::frameset data = mp_Pipe->wait_for_frames(); // Wait for next set of frames from the camera
		rs2::depth_frame depth = data.get_depth_frame();
		if (m_Device.as<rs2::recorder>())
			rs2::video_frame color = data.get_color_frame();
		//m_PixelSize = depth.get_data_size();
		return depth.get_data();
	}
	else {

	}
}

// https://dev.intelrealsense.com/docs/rs-record-playback
std::string RealSenseCamera::startRecording(std::string sessionName, unsigned int numFrames)
{
	auto cameraName = getCameraName();
	std::ranges::replace(cameraName, ' ', '_');

	std::filesystem::path filepath = m_RecordingDirectory / (sessionName + "_" + cameraName + ".bag");
	if (!(m_Device).as<rs2::recorder>())
	{
		mp_Pipe->stop();
		mp_Pipe = std::make_shared<rs2::pipeline>();
		rs2::config cfg;
		std::cout << "[INFO] Saving " << getCameraName() << "'s stream to " << filepath.string() << std::endl;
		cfg.enable_record_to_file(filepath.string());
		mp_Pipe->start(cfg);
		m_Device = mp_Pipe->get_active_profile().get_device();
	}
	else {
		// if its already recording resume
		// Not implemented pause function so a bit useless
		m_Device.as<rs2::recorder>().resume();
	}

	m_selectedForRecording = true;
	m_isEnabled = true;

	return filepath.filename().string();
}

void RealSenseCamera::saveFrame() {
	rs2::frameset data = mp_Pipe->wait_for_frames();

	data.get_depth_frame();
	data.get_color_frame();
}


void RealSenseCamera::stopRecording()
{
	mp_Pipe->stop();
	mp_Pipe = std::make_shared<rs2::pipeline>();

	m_Config.enable_device(m_Device.get_info(RS2_CAMERA_INFO_SERIAL_NUMBER));
	mp_Pipe->start(m_Config);
	m_Device = mp_Pipe->get_active_profile().get_device();

	m_selectedForRecording = false;
}

void RealSenseCamera::OnUpdate()
{
	m_PointCloud->OnUpdate();
}

inline void RealSenseCamera::OnRender()
{
	m_PointCloud->OnRender();
}

inline void RealSenseCamera::OnImGuiRender()
{
	m_PointCloud->OnImGuiRender();
}

// Utils
void RealSenseCamera::printDeviceInfo() const {
	printf("---\nDevice: %s\n",		m_Device.get_info(RS2_CAMERA_INFO_NAME));
	printf("Produc Line: %s\n",		m_Device.get_info(RS2_CAMERA_INFO_PRODUCT_LINE));
	printf("Serial Number: %s\n",	m_Device.get_info(RS2_CAMERA_INFO_SERIAL_NUMBER));
	printf("Physical Port: %s\n\n", m_Device.get_info(RS2_CAMERA_INFO_PHYSICAL_PORT));
}