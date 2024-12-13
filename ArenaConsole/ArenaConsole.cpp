
//#include "ArenaApi.h"
//
//#include "stdafx.h"
#include "opencv2\opencv.hpp"
#include <math.h>
#include <iostream>
#include "conio.h"
#include "ArenaCamera.h"

#include <direct.h> // _getcwd
#include <stdlib.h> // free, perror
#include <stdio.h>  // printf
#include <string.h> // strlen
#include <io.h> // strlen
#include <string> // strlen

#include<fstream>
#include<cstring>
#include "MultiCameraManager.h"

using std::cout;
using namespace std;
using namespace Arena;
using namespace cv;
#define TAB "  "

void ImageGrabTest(int indx)
{
	CameraManager::AcquisitionStart(indx);

	string strVal;
	CameraManager::GetCameraParam(indx, "Width", strVal);
	int _Width = atoi(strVal.c_str());

	CameraManager::GetCameraParam(indx, "Height", strVal);
	int _Height = atoi(strVal.c_str());

	CameraManager::GetCameraParam(indx, "Channels", strVal);
	int _Channels = atoi(strVal.c_str());
	void* ptr=nullptr;
	while (true)
	{
		if(ptr!=nullptr)
			delete[] ptr;

		if (_Width == 0 || _Height == 0 || _Channels == 0)
		{
			std::thread::id threadId = std::this_thread::get_id();

			size_t threadHash = std::hash<std::thread::id>()(threadId);
			std::string threadIdStr = std::to_string(threadHash);

			cout << " \n threadId: " << threadIdStr <<" \n" << endl;
			std::this_thread::sleep_for(std::chrono::milliseconds(500));
			continue;
		}

		ptr = (void*)malloc(_Width * _Height * _Channels * 8); //必須先提供記憶大小

		CameraManager::Grab(indx,ptr);

		string str = "Camera " + to_string(indx);
		namedWindow("Camera " + to_string(indx));

		int cvType = CV_8UC1;

		if (_Channels == 3)
			cvType = CV_8UC3;
		else if (_Channels == 4)
			cvType = CV_8UC4;

		cv::Mat img(_Width, _Height, cvType, ptr);

		imshow(str,img);

	}
}

void ImageGrab(int cameraId, unsigned int*& imgPtr)
{
	string strVal;
	CameraManager::GetCameraParam(cameraId, "Width", strVal);
	int _Width = atoi(strVal.c_str());

	CameraManager::GetCameraParam(cameraId, "Height", strVal);
	int _Height = atoi(strVal.c_str());

	CameraManager::GetCameraParam(cameraId, "Channels", strVal);
	int Channels = atoi(strVal.c_str());

	void* ptr = (void*)malloc(_Width * _Height * Channels * 8); //必須先提供記憶大小

	WriteLog(" cameraId :" + to_string(cameraId) + " CameraManager_Grab_Int : Start");
	CameraManager::Grab(cameraId, ptr);

	int cvType = CV_8UC1;

	if (Channels == 3)
		cvType = CV_8UC3;
	else if (Channels == 4)
		cvType = CV_8UC4;


	cv::Mat img(_Width, _Height, cvType, ptr);
	cv::Mat image_output(_Width, _Height, CV_8UC4, &imgPtr[0]);

	CameraManager::GetCameraParam(0, "PixelFormat", strVal);

	int cvCvtType = cv::ColorConversionCodes::COLOR_GRAY2BGRA;

	if (Channels == 3)
		cvCvtType = cv::ColorConversionCodes::COLOR_BGR2BGRA;

	if (Channels != 4)
		cv::cvtColor(img, image_output, cvCvtType);
	else
		img.copyTo(image_output);

	std::string addressStr = std::to_string(reinterpret_cast<uintptr_t>(ptr));

	WriteLog(" cameraId :" + to_string(cameraId) + " Address:" + addressStr);//<---無法同時存取 兩隻相機的影像 待釐清
	img.release(); //<--- 在Labview SubVi被呼叫時 無法有效釋放記憶體
	delete[](ptr);//<--- 這個方法才能有效釋放記憶體

	WriteLog(" cameraId :" + to_string(cameraId) + " CameraManager_Grab_Int : Finished");

}


void CameraManager_Grab_Int(int cameraId, unsigned int* imgPtr)
{
	std::thread thr(ImageGrab, cameraId, imgPtr);
	thr.join();
}

int main()
{

	CameraManager::InitializeAllCamera();

	string strArr[20];

	CameraManager::GetAllCameraNames(strArr);

	thread thr = thread(ImageGrabTest,0);
	thread thr1 = thread(ImageGrabTest, 1);

	thr.join();
	thr1.join();


	string strVal;
	CameraManager::GetCameraParam(0, "Width", strVal);
	int _Width = atoi(strVal.c_str());

	CameraManager::GetCameraParam(0, "Height", strVal);
	int _Height = atoi(strVal.c_str());

	CameraManager::GetCameraParam(0, "Channels", strVal);
	int Channels = atoi(strVal.c_str());

	unsigned int* ptr = (unsigned int*)malloc(_Width * _Height * 4 * 8); //必須先提供記憶大小

	CameraManager_Grab_Int(0, ptr);

	cv::Mat image_output(_Width, _Height, CV_8UC4, &ptr[0]);

	//----測試取像是否還是正常
	return 0;
}
