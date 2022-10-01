#pragma once
#include <opencv2/imgproc.hpp>
#include <imgui.h>
#include <DepthCamera.h>

namespace Params
{
	class Parameters {
	public:
		Parameters() = default;
		virtual void displayParameters() = 0;
	};

	class GlobalParameters : Parameters {
	public:
		GlobalParameters(std::vector<DepthCamera*>* depth_cameras) : depth_cameras(depth_cameras) {};
		void displayParameters() override {
			ImGui::Begin("Global Settings");
			ImGui::Text("%d Devices Available", depth_cameras->size());

			ImGui::Text("");

			for (DepthCamera* cam : *depth_cameras) {
				ImGui::SameLine();
				ImGui::Checkbox(cam->getCameraName().c_str(), &cam->is_enabled);
			}

			ImGui::Checkbox("Display edges", &display_edges);
			ImGui::Checkbox("Calculate Surface Normals", &calculate_surface_normals);
			bool reset_walking_frames;
			reset_walking_frames |= ImGui::Checkbox("Use Walking Average", &walking_average);

			ImGui::End();
		}
		float sphere_radius;
		bool display_edges = false;
		bool walking_average = false;
		bool calculate_surface_normals = false;	
		std::vector<DepthCamera*>* depth_cameras;
	};

	// TODO Implement all parameters
	/*
	@param src Source 8 - bit single - channel image.
	@param dst Destination image of the same size and the same type as src.
	@param maxValue Non - zero value assigned to the pixels for which the condition is satisfied
	@param adaptiveMethod Adaptive thresholding algorithm to use, see #AdaptiveThresholdTypes.
	The #BORDER_REPLICATE | #BORDER_ISOLATED is used to process boundaries.
	@param thresholdType Thresholding type that must be either #THRESH_BINARY or #THRESH_BINARY_INV,
	see #ThresholdTypes.
	@param blockSize Size of a pixel neighborhood that is used to calculate a threshold value for the
	pixel : 3, 5, 7, and so on.
	@param C Constant subtracted from the mean or weighted mean(see the details below).Normally, it
	is positive but may be zero or negative as well.
	*/

	// TODO: Add save capability
	// TODO: Investigate if this should be camera specific

	class SphereDetectionParameters : Parameters {
	public:
		void displayParameters() override {
			ImGui::Begin("SphereDetectionParameters");

			ImGui::Text("Edge Detection Settings");

			const char* adaptive_threshold_types[] = {
				"Mean",
				"Gaussian"
			};
			const char* threshold_types[] = {
				"Binary",
				"Binary Inverted",
				"Trunc",
				"To Zero",
				"To Zero Inverted",
				"Mask",
				"Otsu",
				"Triangle"
			};

			if (ImGui::Combo("Adaptive Threshold Type", &current_adaptive_threshold, adaptive_threshold_types, IM_ARRAYSIZE(adaptive_threshold_types))) {
				adapriveThresholdType = adapriveThresholdTypes[current_adaptive_threshold];
			}

			if (ImGui::Combo("Threshold Type", &current_threshold, threshold_types, IM_ARRAYSIZE(threshold_types))) {
				thresholdType = thresholdTypes[current_threshold];
			}

			ImGui::Separator();
			ImGui::Text("Sphere Detector Settings");

			ImGui::SliderFloat("Sphere Radius", &sphere_radius, 0, 100);
			ImGui::DragIntRange2("Circle Radius", &min_radius, &max_radius, 5, 0, 100, "Min: %d", "Max: %d");
			ImGui::SliderFloat("Canny edge detector threshold", &param1, 0, 500);
			ImGui::SliderFloat("Accumulator threshold", &param2, 0, 500);

			ImGui::Separator();
			ImGui::End();
		}

		float sphere_radius{ 50 };

		float param1{ 10 };
		float param2{ 10 };
		int min_radius{ 0 };
		int max_radius{ 10 };

		int block_size{};

		int current_adaptive_threshold = 0;
		int current_threshold = 0;

		std::vector<cv::AdaptiveThresholdTypes> adapriveThresholdTypes{
			cv::AdaptiveThresholdTypes::ADAPTIVE_THRESH_MEAN_C ,
			cv::AdaptiveThresholdTypes::ADAPTIVE_THRESH_GAUSSIAN_C
		};

		std::vector<cv::ThresholdTypes> thresholdTypes{
			cv::ThresholdTypes::THRESH_BINARY,
			cv::ThresholdTypes::THRESH_BINARY_INV,
			cv::ThresholdTypes::THRESH_TRUNC,
			cv::ThresholdTypes::THRESH_BINARY_INV,
			cv::ThresholdTypes::THRESH_TOZERO,
			cv::ThresholdTypes::THRESH_TOZERO_INV,
			cv::ThresholdTypes::THRESH_MASK,
			cv::ThresholdTypes::THRESH_OTSU,
			cv::ThresholdTypes::THRESH_TRIANGLE
		};

		cv::AdaptiveThresholdTypes adapriveThresholdType{ cv::AdaptiveThresholdTypes::ADAPTIVE_THRESH_MEAN_C };
		cv::ThresholdTypes thresholdType{ cv::ThresholdTypes::THRESH_BINARY };
	};

	class NormalParameters : Parameters {
	public:
		void displayParameters() override {
			ImGui::Separator();

			ImGui::Text("Normal Settings");

			ImGui::SliderInt("Whats Up", &whatsUp, 0, 2);
			ImGui::SliderFloat("How Up", &upnessFilter, 0, 2 * 3.45f);
			ImGui::SliderInt("Number of Samples", &num_samples, 0, 50);
			ImGui::SliderFloat("Edge cutoff", &edgeCutoff, 1, 50);

			ImGui::Separator();
		}

		float upnessFilter{ 1 };
		float edgeCutoff{ 10 };
		int whatsUp;
		int num_samples;
	}; 
}