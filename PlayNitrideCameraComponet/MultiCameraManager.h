#pragma once
#include <string>
#include <iostream>
#include<fstream>
//#include "pch.h"


using namespace std;


namespace CameraManager
{
	__declspec(dllexport)  void InitializeAllCamera();
	__declspec(dllexport)  int	CameraCount();
	__declspec(dllexport)  void CloseAllCamera();

	__declspec(dllexport)  void Grab(int cameraId, unsigned  int*& imgPtr);
	__declspec(dllexport)  void Grab(int cameraId, void*& imgPtr);

	__declspec(dllexport)  void GetAllCameraNames(string strCameraNameArray[]);

	__declspec(dllexport)  void SetCameraParam(int cameraId, string NodeName, string Value);
	__declspec(dllexport)  void GetCameraParam(int cameraId, string NodeName, string& Value);

	__declspec(dllexport)  void SetCameraParam(int cameraId, string NodeName[], string Value[]);
	__declspec(dllexport)  void GetCameraParam(int cameraId, string NodeName[], string Value[]);

	__declspec(dllexport)  void AcquisitionStart(int cameraId);
	__declspec(dllexport)  void AcquisitionStop(int cameraId);

	__declspec(dllexport) void FreeIntptrMemoryInt(unsigned  int* imgPtr);
	__declspec(dllexport) void FreeIntptrMemoryChar(unsigned  char* imgPtr);
	__declspec(dllexport) void FreeIntptrMemoryImage(void* imgPtr);

	__declspec(dllexport)  void SaveCurrentCameraParam(int cameraId);
	__declspec(dllexport)  void LoadSavedCameraParam(int cameraId);



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


extern "C" __declspec(dllexport)  void CSharp_SetCameraParam(int cameraId, const char* NodeName, const char* Value);
extern "C" __declspec(dllexport)  const char* CSharp_GetCameraParam(int cameraId, const char* NodeName);

extern "C" __declspec(dllexport)  void CSharp_AcquisitionStart(int cameraId);
extern "C" __declspec(dllexport)  void CSharp_AcquisitionStop(int cameraId);

extern "C" __declspec(dllexport) void CSharp_GetAllCamera(const char** array);

extern "C" __declspec(dllexport) void CSharp_FreeIntptrMemory(void* str);

#pragma endregion