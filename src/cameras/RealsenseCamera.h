#pragma once

#include <librealsense2/rs.hpp>
#include <filesystem>
#include <memory>

#include "GLCore/Renderer.h"
#include "obj/Logger.h"
#include "DepthCamera.h"

class RealSenseCamera : public DepthCamera {
public:
	RealSenseCamera(rs2::context* ctx, rs2::device* device, Camera *cam, Renderer *renderer, int camera_id, Logger::Logger* logger);
	RealSenseCamera(Camera* cam, Renderer* renderer, Logger::Logger* logger, std::filesystem::path recording);

	~RealSenseCamera() override;

	const void *getDepth() override;

	static std::string getType() { return "Realsense"; }

	inline std::string getWindowName() const override {
		return "Display: " + this->getCameraName();
	}

	inline std::string getCameraName() const override {
		return this->getType() + " Camera " + std::to_string(this->m_CameraId);
	}

	void printDeviceInfo() const;

	static rs2::device_list getAvailableDevices(rs2::context ctx);
	static std::vector<RealSenseCamera*> initialiseAllDevices(Camera *cam, Renderer *renderer, int *starting_id, Logger::Logger* logger);

	inline unsigned int getDepthStreamWidth() const override
	{
		return m_DepthWidth;
	}

	inline unsigned int getDepthStreamHeight() const override
	{
		return m_DepthHeight;
	}

	std::string startRecording(std::string sessionName) override;
	void showCameraInfo() override;
	void saveFrame() override;
	void stopRecording() override;

	void OnUpdate() override;
	void OnRender() override;
	void OnImGuiRender() override;

	inline float getIntrinsics(INTRINSICS intrin) const override
	{
		switch (intrin)
		{
			case INTRINSICS::FX:
				return m_Intrinsics.fx;
			case INTRINSICS::FY:
				return m_Intrinsics.fy;
			case INTRINSICS::CX:
				return m_Intrinsics.ppx;
			case INTRINSICS::CY:
				return m_Intrinsics.ppy;
			default:
				break;
		}
	}

	inline glm::mat3 getIntrinsics() const override
	{
		return { m_Intrinsics.fx,		     0.0f, m_Intrinsics.ppx,
						    0.0f, m_Intrinsics.fy, m_Intrinsics.ppy,
						    0.0f,		     0.0f,             1.0f };
	}

private:
	std::shared_ptr<rs2::pipeline> mp_Pipe;
	rs2::context* mp_Context{};
	rs2::device m_Device{};
	rs2::config m_Config{};

	rs2_intrinsics m_Intrinsics;

	// Declare depth colorizer for pretty visualization of depth data
	rs2::colorizer m_ColorMap{};

	Logger::Logger* mp_Logger;

	unsigned int m_DepthWidth;
	unsigned int m_DepthHeight;
	std::unique_ptr<GLObject::PointCloud> m_PointCloud;
};
