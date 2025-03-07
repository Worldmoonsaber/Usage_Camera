#pragma once

#include "ICamera.h"
#include "ArenaApi.h"

using namespace std;

class ArenaCameraObject:public ICamera
{

public:
	ArenaCameraObject(Arena::ISystem* pSystem,Arena::DeviceInfo device);
	~ArenaCameraObject();

	void Initialize();
	void Close();

	void Grab_Int(unsigned int*& imgPtr);
	void Grab(void*& imgPtr);

	void SetCameraParam(string NodeName, string Value);
	void GetCameraParam(string NodeName, string& Value);

	void Excute(string ExcuteCmd);

	void AcquisitionStart();
	void AcquisitionStop();

	void Save();
	void Load();

private:

	bool _containsSubstring(const std::string& mainStr, const std::string& subStr);
	void _UpdateMap(string NodeName, string Value);
	void _CreateMap(string NodeName, string Value);

	bool _IsParamInMap(string NodeName, string& CurrentValue);

	void _SetCameraParamDouble(string NodeName, double Value);
	void _GetCameraParamDouble(string NodeName, double& Value);

	void _SetCameraParamInt(string NodeName, int Value);
	void _GetCameraParamInt(string NodeName, int& Value);

	void _SetCameraParamBool(string NodeName, bool Value);
	void _GetCameraParamBool(string NodeName, bool& Value);

	bool _IsCurrentWriteSpecialNode(string NodeName, string Value);
	bool _IsCurrentReadSpecialNode(string NodeName, string& Value);
	Arena::DeviceInfo _deviceInfo;
	Arena::IDevice* _Device;

	void _GetImgPtr(unsigned int*& imgPtr);
	void _GetImgPtr(void*& imgPtr);

	Arena::ISystem* _pSystem;
	bool _IsStreamStart=false;
	bool _isInitialized = false;
	void _LoadConfig();
	void _SaveConfig();
	bool _isNumeric(std::string str);

	/// <summary>
	/// 需要組合指令
	/// </summary>
	/// <param name="NodeName"></param>
	/// <param name="Value"></param>
	/// <returns></returns>
	bool _IsSpecialCommand(string NodeName, string Value);

	/// <summary>
	/// 需要組合指令
	/// </summary>
	/// <param name="NodeName"></param>
	/// <param name="Value"></param>
	/// <returns></returns>
	bool _IsSpecialReturnValue(string NodeName, string& Value);

	std::vector<std::string> _split(const std::string& str, const std::string& pattern);

	map<string, string> _mapParam;
	vector<tuple<string, string>> _vStringPm;


#pragma region 對應ARENA 的使用方式建表
	std::vector<string> _ParamKey_OffenRepeatKey{"BalanceRatioSelector","BalanceRatio","GainSelector"};// 切換模式所下的指令 不需要存留值


	std::vector<string> _ParamKey_ValueIsDouble{ "ExposureTime","AcquisitionFrameRate","TriggerDelay","ExposureAutoDamping","Gain","BlackLevel","Gamma","BalanceRatio","ShadingCorrectionValue"};

	std::vector<string> _ParamKey_ValueIsInt{
		"DeviceLinkSelector","GevStreamChannelSelector",
		"DeviceLinkThroughputReserve","DeviceStreamChannelSelector",
		"Width","Height","AcquisitionFrameCount","AcquisitionBurstFrameCount","PacketResendWindowFrameCount","GevSCPSPacketSize","GevSCPD",
		"ActionSelector","ActionGroupKey","ActionGroupMask","CounterValue","CounterDuration","OffsetX","OffsetY",
		"DecimationHorizontal","DecimationVertical","TargetBrightness","DeviceStreamChannelPacketSize","ShadingCorrectionCellWidth","ShadingCorrectionCellHeight","WidthMax","HeightMax",
		"ShadingCorrectionNodeIndexY","ShadingCorrectionNodeIndexX"};

	std::vector<string> _ParamKey_ValueIsBool{ "AcquisitionFrameRateEnable","ShortExposureEnable","ShortIntervalShutterEnable",
		"GevSCPSDoNotFragment","LineInverter","VoltageExternalEnable","UserOutputValue","ChunkEnable","MultipleROIRowsEnable","ReverseX","ReverseY","GammaEnable","SharpeningEnable",
		"AwbAOIEnable","LUTEnable","DefectCorrectionEnable","ShadingCorrectionEnable","AutoExposureAOIEnable","ChunkModeActive","BalanceWhiteEnable"};


	std::vector<string> _ParamKey_SpecialKey{ "GainRed","GainGreen","GainBlue","GainAll","Channels"};


	std::vector<string> _ParamKey_NeedStopAcqWhenSet{ "PixelFormat" };

#pragma endregion

};