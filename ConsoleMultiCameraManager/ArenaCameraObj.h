#pragma once

#include "ICamera.h"
#include "ArenaApi.h"
#include "SaveApi.h"
#include<fstream>
#include<cstring>

using namespace std;

class ArenaCameraObj:public ICamera
{

public:
	ArenaCameraObj(Arena::DeviceInfo device);
	~ArenaCameraObj();

	void Initialize();
	void Close();

	void Grab(unsigned int*& imgPtr);
	virtual void Grab(unsigned  int*& imgPtr);
	virtual void Grab(unsigned char*& imgPtr);

	void ConsolePrintDeviceInfo();

	void SetCameraParam(string NodeName, string Value);
	void GetCameraParam(string NodeName, string& Value);

	void Excute(string ExcuteCmd);

	void AcquisitionStart();
	void AcquisitionStop();

private:
	Arena::DeviceInfo _deviceInfos;

	void _GetImgPtr(Arena::IDevice* pDevice, unsigned int*& imgPtr);
	Arena::ISystem* _pSystem;
	std::vector<Arena::DeviceInfo> _deviceInfos;
	std::vector<bool> _IsStreamStart;
	bool _isInitialized = false;
	std::vector<std::string> split(const std::string& str, const std::string& pattern);
};