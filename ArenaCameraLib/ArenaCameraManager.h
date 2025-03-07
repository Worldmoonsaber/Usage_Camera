#pragma once
#include <string>
#include <iostream>
#include<fstream>
#include <windows.h>
#include "ICamera.h"

using namespace std;


namespace CameraManager
{
	extern "C" __declspec(dllexport)  vector< ICamera*> GetCamera();
	extern "C" __declspec(dllexport)  void CloseCamera();
	void WriteLog(const std::string& message);
}
