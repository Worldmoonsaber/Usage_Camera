#pragma once

#include "ICamera.h"
#include "ArenaApi.h"
#include "SaveApi.h"
#include<fstream>
#include<cstring>
#include <map>
//#include "pch.h"

using namespace std;

class ArenaCameraObject:public ICamera
{

public:
	ArenaCameraObject(Arena::ISystem* pSystem,Arena::DeviceInfo device);
	~ArenaCameraObject();

	void Initialize();
	void Close();

	void Grab(unsigned int*& imgPtr);
	void Grab(unsigned  char*& imgPtr);

	void SetCameraParam(string NodeName, string Value);
	void GetCameraParam(string NodeName, string& Value);

	void Excute(string ExcuteCmd);

	void AcquisitionStart();
	void AcquisitionStop();

	void Save();

private:

	void SetCameraParamDouble(string NodeName, double Value);
	void GetCameraParamDouble(string NodeName, double& Value);

	void SetCameraParamInt(string NodeName, int Value);
	void GetCameraParamInt(string NodeName, int& Value);

	void SetCameraParamBool(string NodeName, bool Value);
	void GetCameraParamBool(string NodeName, bool& Value);

	bool _IsCurrentWriteSpecialNode(string NodeName, string Value);
	bool _IsCurrentReadSpecialNode(string NodeName, string& Value);
	bool _IsNodeValueDouble(string str);
	Arena::DeviceInfo _deviceInfo;
	Arena::IDevice* _Device;

	void _GetImgPtr(unsigned int*& imgPtr);
	void _GetImgPtr(unsigned char*& imgPtr);


	Arena::ISystem* _pSystem;
	bool _IsStreamStart=false;
	bool _isInitialized = false;
	void _LoadConfig();
	bool _isNumeric(std::string const& str);
	std::vector<std::string> _split(const std::string& str, const std::string& pattern);

#pragma region 對應ARENA 的使用方式建表

	std::vector<string> _ParamKey_ValueIsDouble{ "ExposureTime","AcquisitionFrameRate","TriggerDelay","ExposureAutoDamping","Gain","BlackLevel","Gamma","BalanceRatio"};

	std::vector<string> _ParamKey_ValueIsInt{
		"DeviceLinkSelector","GevStreamChannelSelector",
		"DeviceLinkThroughputReserve","DeviceStreamChannelSelector",
		"Width","Height","AcquisitionFrameCount","AcquisitionBurstFrameCount","PacketResendWindowFrameCount","GevSCPSPacketSize","GevSCPD",
		"ActionSelector","ActionGroupKey","ActionGroupMask","CounterValue","CounterDuration","OffsetX","OffsetY",
		"DecimationHorizontal","DecimationVertical","TargetBrightness","DeviceStreamChannelPacketSize"};

	std::vector<string> _ParamKey_ValueIsBool{ "AcquisitionFrameRateEnable","ShortExposureEnable","ShortIntervalShutterEnable",
		"GevSCPSDoNotFragment","LineInverter","VoltageExternalEnable","UserOutputValue","ChunkEnable","MultipleROIRowsEnable","ReverseX","ReverseY","GammaEnable","SharpeningEnable",
		"AwbAOIEnable","LUTEnable","DefectCorrectionEnable","ShadingCorrectionEnable","AutoExposureAOIEnable","ChunkModeActive" };

#pragma endregion

};