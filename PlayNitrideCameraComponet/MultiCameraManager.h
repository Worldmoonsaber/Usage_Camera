#pragma once
#include <string>
#include <iostream>
#include<fstream>
#include <windows.h>
#include <stdlib.h> // free, perror
#include <stdio.h>  // printf
#include <string.h> // strlen
#include <io.h> // strlen
#include <string> // strlen
#include <chrono>
#include <direct.h>


using namespace std;


namespace CameraManager
{
	__declspec(dllexport)  void InitializeAllCamera();
	__declspec(dllexport)  int	CameraCount();
	__declspec(dllexport)  void CloseAllCamera();

	__declspec(dllexport)  void Grab(int cameraId, unsigned  int*& imgPtr);
	__declspec(dllexport)  void Grab(int cameraId, void*& imgPtr);

	__declspec(dllexport)  void GetAllCameraNames(string strCameraNameArray[]);
	__declspec(dllexport)  void GetAllLog(string strLog[]);

	__declspec(dllexport)  void GetCameraName(int cameraId, string& strCameraNameArray);


	__declspec(dllexport)  void SetCameraParam(int cameraId, string NodeName, string Value);
	__declspec(dllexport)  void GetCameraParam(int cameraId, string NodeName, string& Value);

	__declspec(dllexport)  void SetCameraParam(int cameraId, string NodeName[], string Value[]);
	__declspec(dllexport)  void GetCameraParam(int cameraId, string NodeName[], string Value[]);

	__declspec(dllexport)  void AcquisitionStart(int cameraId);
	__declspec(dllexport)  void AcquisitionStop(int cameraId);

	__declspec(dllexport)  void ExcuteCmd(int cameraId, string Command);

	__declspec(dllexport) void FreeIntptrMemoryInt(unsigned  int* imgPtr);
	__declspec(dllexport) void FreeIntptrMemoryChar(unsigned  char* imgPtr);
	__declspec(dllexport) void FreeIntptrMemoryImage(void* imgPtr);

	__declspec(dllexport)  void SaveCurrentCameraParam(int cameraId);
	__declspec(dllexport)  void LoadSavedCameraParam(int cameraId);

	__declspec(dllexport)  void SetSimulationImageSource(string strSourceFile);

	__declspec(dllexport)  void Grab_byCameraNickName(string strCameraNickname, unsigned  int*& imgPtr);
	__declspec(dllexport)  void Grab_byCameraNickName(string strCameraNickname, void*& imgPtr);
	__declspec(dllexport)  void SetCameraParam_byCameraNickName(string strCameraNickname, string NodeName, string Value);
	__declspec(dllexport)  void GetCameraParam_byCameraNickName(string strCameraNickname, string NodeName, string& Value);

	__declspec(dllexport)  void SetCameraParam_byCameraNickName(string strCameraNickname, string NodeName[], string Value[]);
	__declspec(dllexport)  void GetCameraParam_byCameraNickName(string strCameraNickname, string NodeName[], string Value[]);

	__declspec(dllexport)  void AcquisitionStart_byCameraNickName(string strCameraNickname);
	__declspec(dllexport)  void AcquisitionStop_byCameraNickName(string strCameraNickname);

	__declspec(dllexport)  void SaveCurrentCameraParam_byCameraNickName(string strCameraNickname);
	__declspec(dllexport)  void LoadSavedCameraParam_byCameraNickName(string strCameraNickname);

}

#pragma region CSharp 使用的對接口

extern "C" __declspec(dllexport)  void CSharp_InitializeAllCamera();
extern "C" __declspec(dllexport)  void CSharp_CloseAllCamera();

extern "C" __declspec(dllexport)  void* CSharp_Grab(int cameraId);

extern "C" __declspec(dllexport)  void CSharp_ExcuteCmd(int cameraId, const char* Command);


extern "C" __declspec(dllexport)  void CSharp_SetCameraParam(int cameraId, const char* NodeName, const char* Value);
extern "C" __declspec(dllexport)  const char* CSharp_GetCameraParam(int cameraId, const char* NodeName);

extern "C" __declspec(dllexport)  void CSharp_AcquisitionStart(int cameraId);
extern "C" __declspec(dllexport)  void CSharp_AcquisitionStop(int cameraId);

extern "C" __declspec(dllexport) void CSharp_GetAllCamera(const char** array);

extern "C" __declspec(dllexport) void CSharp_FreeIntptrMemory(void* str);

extern "C" __declspec(dllexport) void CSharp_GetErrorLog(const char** array);

extern "C" __declspec(dllexport) const char* CSharp_GetCurrntVersion();

extern "C" __declspec(dllexport)  void CSharp_SaveCurrentParameter(int cameraId);

extern "C" __declspec(dllexport)  void CSharp_LoadDefaultParameter(int cameraId);


#pragma endregion


/// <summary>
/// Debug時使用
/// </summary>
/// <param name="message"></param>
static bool isExistPath = false;

/// <summary>
/// Debug時使用
/// </summary>
/// <param name="message"></param>
static void WriteLog(const std::string& message) {


	cout << message << endl;

	SYSTEMTIME st;
	GetLocalTime(&st);

	if (!isExistPath)
	{
		string strPath;
		char* buffer;

		// Get the current working directory:
		if ((buffer = _getcwd(NULL, 0)) != NULL)
		{
			strPath.assign(buffer, strlen(buffer));
			free(buffer);
		}

		strPath = strPath + "\\Log\\";

		if (_access(strPath.c_str(), 0) == -1)
		{
			_mkdir(strPath.c_str());
			isExistPath = true;
		}
		else
			isExistPath = true;

	}

	std::ofstream logFile("Log\\camera_manager_log_" + to_string(st.wMonth) + "-" + to_string(st.wDay) + ".txt", std::ios::app); // 以追加模式打開文件
	if (logFile.is_open())
	{
		string str = to_string(st.wMonth) + "-" + to_string(st.wDay) + " " + to_string(st.wHour) + ":" + to_string(st.wMinute) + ":" + to_string(st.wSecond) + ":" + to_string(st.wMilliseconds);

		logFile << str << " " << message << std::endl; // 寫入日誌內容並換行
	}
	else {
		std::cerr << "Unable to open log file." << std::endl; // 文件打開失敗時打印錯誤
	}
}