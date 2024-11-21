#pragma once


#include "ArenaApi.h"
#include "SaveApi.h"
#include<fstream>
#include<cstring>
using namespace std;

class ArenaCameraObj
{

public:
	ArenaCameraObj();
	~ArenaCameraObj();

	void Initialize();
	std::vector<Arena::DeviceInfo> DeviceInfos();

	void Grab(int cameraId, unsigned int*& imgPtr);
	void Grab(unsigned int*& imgPtr);
	void SelectCameraId(int cameraId);
	void Close();
	void ConsolePrintDeviceInfo();

	void SetCameraParam(string NodeName, string Value);
	void GetCameraParam(string NodeName, string &Value);

	void SetCameraParam(int cameraId,string NodeName, string Value);
	void GetCameraParam(int cameraId,string NodeName, string& Value);
	void Excute(string ExcuteCmd);

	void AcquisitionStart();
	void AcquisitionStop();

private:
	void _GetImgPtr(Arena::IDevice* pDevice, unsigned int*& imgPtr);
	Arena::ISystem* _pSystem;
	std::vector<Arena::DeviceInfo> _deviceInfos;
	std::vector<Arena::IDevice*> _deviceObj;
	std::vector<bool> _IsStreamStart;

	bool _isInitialized = false;
	int _SelectIndx = -1;
	std::vector<std::string> split(const std::string& str, const std::string& pattern);
	void _LoadConfig(Arena::IDevice* pDevice, Arena::DeviceInfo info);
	//----ConfigºÞ²z
	void _ResetCamera();
};


