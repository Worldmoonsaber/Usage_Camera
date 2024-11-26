#pragma once
#include <string>
#include <iostream>
#include<fstream>
//#include "pch.h"


using namespace std;


namespace CameraManager
{
	 __declspec(dllexport)  void InitializeAllCamera();
	__declspec(dllexport)  void CloseAllCamera();

	__declspec(dllexport)  void Grab(int cameraId, unsigned  int*& imgPtr);
	__declspec(dllexport)  void Grab(int cameraId, unsigned  char*& imgPtr);

	__declspec(dllexport)  void GetAllCameraNames(string strCameraNameArray[]);

	__declspec(dllexport)  void SetCameraParam(int cameraId, string NodeName, string Value);
	__declspec(dllexport)  void GetCameraParam(int cameraId, string NodeName, string& Value);

	__declspec(dllexport)  void SetCameraParam(int cameraId, string NodeName[], string Value[]);
	__declspec(dllexport)  void GetCameraParam(int cameraId, string NodeName[], string Value[]);

	__declspec(dllexport)  void AcquisitionStart(int cameraId);
	__declspec(dllexport)  void AcquisitionStop(int cameraId);

}

#pragma region CSharp 使用的對接口

extern "C" __declspec(dllexport)  void CSharp_InitializeAllCamera();
extern "C" __declspec(dllexport)  void CSharp_CloseAllCamera();

extern "C" __declspec(dllexport)  unsigned  int* CSharp_GrabInt(int cameraId);
extern "C" __declspec(dllexport)  unsigned  char* CSharp_GrabChar(int cameraId);


extern "C" __declspec(dllexport)  void CSharp_SetCameraParam(int cameraId, const char* NodeName, const char* Value);
extern "C" __declspec(dllexport)  const char* CSharp_GetCameraParam(int cameraId, const char* NodeName);

extern "C" __declspec(dllexport)  void CSharp_AcquisitionStart(int cameraId);
extern "C" __declspec(dllexport)  void CSharp_AcquisitionStop(int cameraId);

extern "C" __declspec(dllexport) void CSharp_GetAllCamera(const char** array);

extern "C" __declspec(dllexport) void CSharp_FreeIntptrMemory(void* str);

#pragma endregion