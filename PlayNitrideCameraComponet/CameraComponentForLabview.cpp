#include "CameraComponentForLabview.h"
#include "MultiCameraManager.h"
#include <opencv2/core/mat.hpp>
#include <opencv2/imgproc.hpp>


const char* CameraManager_GetCurrentVersion()
{
	return CSharp_GetCurrntVersion();
}

bool CameraManager_DoInitialize(int* CameraCount)
{
	CameraManager::InitializeAllCamera();
	CameraCount[0] = CameraManager::CameraCount();

	WriteLog("CameraCount :"+to_string(CameraCount[0]));

	if (CameraCount [0]> 0)
		return true;
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

void CameraManager_Grab_Int(int cameraId, unsigned int* imgPtr)
{
	std::thread::id threadId = std::this_thread::get_id();

	size_t threadHash = std::hash<std::thread::id>()(threadId);
	std::string threadIdStr = std::to_string(threadHash);

	string strVal;
	CameraManager::GetCameraParam(cameraId, "Width", strVal);
	int _Width = atoi(strVal.c_str());

	CameraManager::GetCameraParam(cameraId, "Height", strVal);
	int _Height = atoi(strVal.c_str());

	CameraManager::GetCameraParam(cameraId, "Channels", strVal);
	int Channels = atoi(strVal.c_str());

	void* ptr = (void*)malloc(_Width * _Height * Channels * 8); //必須先提供記憶大小
	
	WriteLog("Thread ID: "+ threadIdStr +" cameraId :"+ to_string(cameraId)  +" CameraManager_Grab_Int : Start");
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

	if (Channels != 4)
		cv::cvtColor(img, image_output, cvCvtType);
	else
		img.copyTo(image_output);

	std::string addressStr = std::to_string(reinterpret_cast<uintptr_t>(ptr));

	WriteLog("Thread ID: " + threadIdStr + " cameraId :" + to_string(cameraId) + " Address:"+ addressStr);//<---無法同時存取 兩隻相機的影像 待釐清
	img.release(); //<--- 在Labview SubVi被呼叫時 無法有效釋放記憶體
	delete[] (ptr);//<--- 這個方法才能有效釋放記憶體

	WriteLog("Thread ID: " + threadIdStr + " cameraId :" + to_string(cameraId) + " CameraManager_Grab_Int : Finished");

}


void CameraManager_Grab_Char(int cameraId, unsigned char* imgPtr)
{
	void* imgPtrV= (void*)imgPtr;
	CameraManager::Grab(cameraId, imgPtrV);
}

bool CameraManager_SetCameraParam(int cameraId, char* NodeName,char* Value)
{
	CameraManager::SetCameraParam(cameraId, NodeName, Value);
	//WriteLog("Set cameraId :" + to_string(cameraId) + " NodeName " + NodeName + " Value:" + Value);
	return true;
}

void CameraManager_GetCameraParam(int cameraId,char* NodeName,char* Value)
{
	const char* cChar=CSharp_GetCameraParam(cameraId,NodeName);

	std::strcpy(Value, cChar); // 複製內容到緩衝區
	//WriteLog("Get cameraId :" +to_string(cameraId) +" NodeName "+ NodeName+" Value:"+ Value);

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
