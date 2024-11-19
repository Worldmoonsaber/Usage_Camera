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

	void Grab(int cameraId,uint8_t* ImgPtr);
	void Grab(uint8_t* ImgPtr);
	void SelectCameraId(int cameraId);
	void Close();
	void ConsolePrintDeviceInfo();
	void ConfigureTriggerAndAcquireImage(Arena::IDevice* pDevice);

	void SetCameraParam(string NodeName, string Value);
	void GetCameraParam(string NodeName, string &Value);

	void SetCameraParam(int cameraId,string NodeName, string Value);
	void GetCameraParam(int cameraId,string NodeName, string& Value);
	void Excute(string ExcuteCmd);
private:
	Arena::ISystem* pSystem;
	std::vector<Arena::DeviceInfo> deviceInfos;
	std::vector<Arena::IDevice*> deviceObj;
	bool isInitialized = false;
	int _SelectIndx = -1;
};


