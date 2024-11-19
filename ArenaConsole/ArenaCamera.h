#pragma once


#include "ArenaApi.h"
#include "SaveApi.h"
#include<opencv2/opencv.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp> //mophorlogical operation
#include<opencv2/core.hpp>
#include <opencv2/core/mat.hpp>

using namespace std;
using namespace cv;


Mat GetCvImg(Arena::IDevice* pDevice);
Mat Unpack12BitImage(Arena::IImage* image);


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

private:
	void _GetImgPtr(Arena::IDevice* pDevice, unsigned int*& imgPtr);
	Arena::ISystem* _pSystem;
	std::vector<Arena::DeviceInfo> _deviceInfos;
	std::vector<Arena::IDevice*> _deviceObj;
	std::vector<bool> _IsStreamStart;

	bool _isInitialized = false;
	int _SelectIndx = -1;

	void _LoadConfig(Arena::IDevice* pDevice, Arena::DeviceInfo info);
	//----ConfigºÞ²z
};


