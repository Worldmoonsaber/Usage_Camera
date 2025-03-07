#pragma once
#include "OmronCameraObject.h"

namespace CameraManager
{
	extern "C" __declspec(dllexport)  vector< ICamera*> GetCamera();
	extern "C" __declspec(dllexport)  void CloseCamera();
	void WriteLog(const std::string& message);
	void GetDeviceFromStpApi();
}
