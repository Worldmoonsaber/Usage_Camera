#pragma once
#include "CameraComponentForLabview.h"
#include "MultiCameraManager.h"
#include <opencv2/core/mat.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>


const char* CameraManager_GetCurrentVersion()
{
	return CSharp_GetCurrntVersion();
}

bool CameraManager_DoInitialize(int* CameraCount)
{
	CameraManager::InitializeAllCamera();
	CameraCount[0] = CameraManager::CameraCount();

	WriteLog("CameraCount :"+to_string(CameraCount[0]));



	if (CameraCount[0] > 0)
	{
		for (int i = 0; i < CameraCount[0]; i++)
		{
			string str;
			CameraManager::GetCameraName(i, str);
			WriteLog("Camera"+to_string(i) + " :" + str);
		}

		return true;
	}
	else
		return false;
}

void CameraManager_UnInitialize()
{
	CameraManager::CloseAllCamera();
}

int CameraManager_CameraCount()
{
	return CameraManager::CameraCount();
}

void CameraManager_InitializeAllCamera()
{
	CameraManager::InitializeAllCamera();
}


void CameraManager_CloseAllCamera()
{
	return CameraManager::CloseAllCamera();
}


bool containsSubstring(const std::string& mainStr, const std::string& subStr) {
	return mainStr.find(subStr) != std::string::npos;
}

/// <summary>
/// 
/// </summary>
/// <param name="cameraId"></param>
/// <param name="imgPtr">不外部輸入,內部消化</param>
void CameraManager_Grab_Int(int cameraId, unsigned int* imgPtr)
{

	string strVal;
	CameraManager::GetCameraParam(cameraId, "Width", strVal);
	int _Width = atoi(strVal.c_str());

	CameraManager::GetCameraParam(cameraId, "Height", strVal);
	int _Height = atoi(strVal.c_str());

	CameraManager::GetCameraParam(cameraId, "Channels", strVal);
	int Channels = atoi(strVal.c_str());


	void* ptr = (void*)malloc(_Width * _Height * Channels * 8); //必須先提供記憶大小
	
	
	CameraManager::Grab(cameraId, ptr);
	int cvType = CV_8UC1;

	if(Channels==3)
		cvType = CV_8UC3;
	else if(Channels == 4)
		cvType = CV_8UC4;


	cv::Mat img(_Width, _Height, cvType, ptr);
	cv::Mat image_output(_Width, _Height, CV_8UC4, &imgPtr[0]);

	CameraManager::GetCameraParam(0, "PixelFormat", strVal);

	int cvCvtType = cv::ColorConversionCodes::COLOR_GRAY2BGRA;

	if (Channels == 3 && containsSubstring(strVal,"BGR"))
		cvCvtType = cv::ColorConversionCodes::COLOR_BGR2BGRA;
	else if (Channels == 3 && containsSubstring(strVal, "RGB"))
		cvCvtType = cv::ColorConversionCodes::COLOR_RGB2RGBA;
	else if (Channels == 1 && containsSubstring(strVal, "BayerRG"))
		cvCvtType = cv::ColorConversionCodes::COLOR_BayerRG2BGRA;

	if (Channels != 4)
		cv::cvtColor(img, image_output, cvCvtType);
	else
		img.copyTo(image_output);
	
	img.release(); //<--- 在Labview SubVi被呼叫時 無法有效釋放記憶體
	delete[] (ptr);//<--- 這個方法才能有效釋放記憶體


}


void CameraManager_Grab_Char(int cameraId, unsigned char* imgPtr)
{
	void* imgPtrV= (void*)imgPtr;
	CameraManager::Grab(cameraId, imgPtrV);
}

bool CameraManager_SetCameraParam(int cameraId, char* NodeName,char* Value)
{
	CameraManager::SetCameraParam(cameraId, NodeName, Value);
	return true;
}

void CameraManager_GetCameraParam(int cameraId,char* NodeName,char* Value)
{
	const char* cChar=CSharp_GetCameraParam(cameraId,NodeName);
	std::strcpy(Value, cChar); // 複製內容到緩衝區

}

void CameraManager_AcquisitionStart(int cameraId)
{
	CameraManager::AcquisitionStart(cameraId);
}

void CameraManager_AcquisitionStop(int cameraId)
{
	CameraManager::AcquisitionStop(cameraId);
}

void CameraManager_FreeIntptrMemoryInt(unsigned int* imgPtr)
{
	CameraManager::FreeIntptrMemoryInt(imgPtr);
}

void CameraManager_FreeIntptrMemoryChar(unsigned char* imgPtr)
{
	CameraManager::FreeIntptrMemoryChar(imgPtr);
}

void CameraManager_GetCameraName(int cameraId, char* Name)
{
	string str;
	CameraManager::GetCameraName(cameraId, str);
	std::strcpy(Name, str.c_str()); // 複製內容到緩衝區
}

void CameraManager_ExcuteCommand(int cameraId, char* Command)
{
	CameraManager::ExcuteCmd(cameraId, Command);
}

void CameraManager_SaveParam(int cameraId)
{
	CameraManager::SaveCurrentCameraParam(cameraId);
}

void CameraManager_LoadParam(int cameraId)
{
	CameraManager::LoadSavedCameraParam(cameraId);
}

void CameraManager_SimulationImageSource(char* chrArrImageSource)
{
	std::string strImg(chrArrImageSource); // 轉換為 std::string

	CameraManager::SetSimulationImageSource(strImg);
}
