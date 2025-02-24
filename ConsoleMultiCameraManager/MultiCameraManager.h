#pragma once
//#include <string>
//#include <iostream>
//#include<fstream>

//dll\\PlayNitrideCameraComponet.lib
#pragma comment(lib,"C:\\Git\\Usage_Camera_Workspace\\ArenaConsole\\x64\\Release\\dll\\PlayNitrideCameraComponet2.0.dll")

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

	__declspec(dllexport) void FreeIntptrMemoryInt(unsigned  int* imgPtr);
	__declspec(dllexport) void FreeIntptrMemoryChar(unsigned  char* imgPtr);
	__declspec(dllexport) void FreeIntptrMemoryImage(void* imgPtr);

	__declspec(dllexport)  void SaveCurrentCameraParam(int cameraId);
	__declspec(dllexport)  void LoadSavedCameraParam(int cameraId);
}

