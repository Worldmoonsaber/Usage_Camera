#pragma once

#include "ICamera.h"
#include "ArenaApi.h"
#include "SaveApi.h"
#include<fstream>
#include<cstring>
#include <map>

using namespace std;

class ArenaCameraObject:public ICamera
{

public:
	ArenaCameraObject(Arena::ISystem* pSystem,Arena::DeviceInfo device);
	~ArenaCameraObject();

	void Initialize();
	void Close();

	void Grab(unsigned int*& imgPtr);

	void SetCameraParam(string NodeName, string Value);
	void GetCameraParam(string NodeName, string& Value);

	void SetCameraParam(string NodeName, double Value);
	void GetCameraParam(string NodeName, double& Value);

	void Excute(string ExcuteCmd);

	void AcquisitionStart();
	void AcquisitionStop();

	void Save();

private:
	bool _IsNodeValueDouble(string str);
	Arena::DeviceInfo _deviceInfo;
	Arena::IDevice* _Device;
	void _GetImgPtr(unsigned int*& imgPtr);
	Arena::ISystem* _pSystem;
	bool _IsStreamStart=false;
	bool _isInitialized = false;
	void _LoadConfig();
	bool _isNumeric(std::string const& str);
	std::vector<std::string> _split(const std::string& str, const std::string& pattern);
	std::map<string,string> _ParamtMap;
	std::vector<string> _ParamtValueIsDouble;

};