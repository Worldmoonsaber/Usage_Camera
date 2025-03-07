#pragma once
#include "ICamera.h"
#include <queue>

#include <StApi_TL.h>
#include <StApi_GUI.h>
#include <iomanip>	//std::setprecision
//#include <StApi_TLFunctions.h>
//#include <StApi_GUIDef.h>
//#include <future>

using namespace StApi;
using namespace std;
using namespace GenApi;


class OmronCameraObject : public ICamera
{

public:
	OmronCameraObject();
	
	OmronCameraObject(IStDeviceReleasable* obj);

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
	int _NumBuffer = 20;
	bool _containsSubstring(const std::string& mainStr, const std::string& subStr);

	void _AcquisitionStart();
	void _AcquisitionStop();
	bool _IsDeviceLost;
	bool _IsAcquisitionRunning;
	bool _IsExistPath = false;
	void OnDeviceLost(GenApi::INode* pINode, void*);

	void Execute(INodeMap* pINodeMap, const char* szCommandName); 

	void SetEnumeration(const char* szEnumerationName, const char* szValueName);
	void GetEnumeration(const char* szEnumerationName, string& strVal);

	void SetInteger(const char* szIntName, const char* szValueName);
	void GetInteger(const char* szIntName, string& strVal);

	void SetFloat(const char* szFloatName, const char* szValueName);
	void GetFloat(const char* szFloatName, string& strVal);

	void SetSpecial(const char* szName, const char* szValueName);
	void GetSpecial(const char* szName, string& strVal);

	void SetBool(const char* szBoolName, const char* szValueName);
	void GetBool(const char* szBoolName, string& strVal);

	bool str_to_bool(std::string str) 
	{
		std::transform(str.begin(), str.end(), str.begin(), ::tolower);
		std::istringstream is(str);bool b;is >> std::boolalpha >> b;
		return b;
	}


	CIStDevicePtr _pIStDevice;
	CIStDataStreamPtr _pIStDataStream;// (pIStDevice->CreateIStDataStream(0));
	CIStRegisteredCallbackPtr m_pIStRegisteredCallbackDeviceLost;
	IStDeviceReleasable* _device;
	//CIStImageCallbackList m_objCIStImageCallbackList;

	void mLoadSaveNodeMapSettingFile(bool isLoad);

	GenICam::gcstring _strDeviceID;
	void _GetImgPtr(void*& _imgPtr);
	void _GetImgPtr(unsigned int*& _imgPtr);
	void WriteLog(const std::string& message);

#pragma region 癸莱ARENA ㄏノよΑ
	std::vector<string> _ParamKey_ValueIsEnum{ "PixelFormat","ExposureTimeSelector","ExposureMode","RegionSelector","RegionMode",
		"ComponentSelector","MultiROIsInMultiPayloads","AcquisitionMode","TriggerSelector","TriggerMode",
		"TriggerSource","GainSelector","GainAuto","UserSetSelector","UserSetDefault","ZoomMode","StretchMode","BackgroundStyle",
		"AveragingArea","ThresholdTargetSelector","ThresholdRatio","ThresholdValue"};// ち传家Α┮ ぃ惠璶痙

	std::vector<string> _ParamKey_ValueIsInt{ "Width","WidthMax","Height","HeightMax","AutoLightTarget","StreamSelector",
		"OffsetX","OffsetY","AcquisitionBurstFrameCount","RefreshInterval","ScrollHPosition","ScrollVPosition",
		"MaximumPixelCountToDetect","PeripheralAreaSize"};// ち传家Α┮ ぃ惠璶痙

	std::vector<string> _ParamKey_ValueIsFloat{ "ExposureTime","AcquisitionFrameRate","TriggerDelay","Gain","BlackLevel","Gamma","LinePeriod","GainAutoLimitMax","GainAutoLimitMin","FixedZoomMagnification" };

	std::vector<string> _ParamKey_ValueIsBool{ "ComponentEnable"};

	std::vector<string> _ParamKey_ValueIsCmd{ "AcquisitionStart","AcquisitionStop","TriggerSoftware","UserSetLoad","UserSetSave"};

	std::vector<string> _ParamKey_SpecialKey{ "GainRed","GainGreen","GainBlue","GainAll","Channels",
		"ShadingCorrectionNodeIndexY","ShadingCorrectionNodeIndexX","ShadingCorrectionValue","ShadingCorrectionEnable",
		"TargetBrightness","ExposureAuto","BalanceWhiteAuto"};



#pragma endregion


};




//"StreamSelector","OffsetX","OffsetY","AcquisitionBurstFrameCount","RefreshInterval","ScrollHPosition","ScrollVPosition","MaximumPixelCountToDetect","PeripheralAreaSize"


/*
Int

Enum
	RegionSelector
	RegionMode
	ComponentSelector
	MultiROIsInMultiPayloads
	AcquisitionMode
	TriggerSelector
	TriggerMode
	TriggerSource
	GainSelector
	GainAuto
	UserSetSelector
	UserSetDefault
	ZoomMode
	StretchMode
	BackgroundStyle
	AveragingArea
	ThresholdTargetSelector
	ThresholdRatio
	ThresholdValue
Float
	LinePeriod
	GainAutoLimitMax
	GainAutoLimitMin
	FixedZoomMagnification
bool
	ComponentEnable

cmd
	AcquisitionStart
	AcquisitionStop
	TriggerSoftware
	UserSetLoad
	UserSetSave
*/