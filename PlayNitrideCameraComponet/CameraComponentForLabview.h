#pragma once


//---------------≈Á√“

__declspec(dllexport) bool CameraManager_DoInitialize(int* CameraCount);

__declspec(dllexport) void CameraManager_UnInitialize();

__declspec(dllexport)  int	CameraManager_CameraCount();

__declspec(dllexport)  bool CameraManager_SetCameraParam(int cameraId, char* NodeName, char* Value);
__declspec(dllexport)  void CameraManager_GetCameraParam(int cameraId, char* NodeName, char* Value);

__declspec(dllexport)  void CameraManager_AcquisitionStart(int cameraId);
__declspec(dllexport)  void CameraManager_AcquisitionStop(int cameraId);


__declspec(dllexport)  void CameraManager_GetCameraName(int cameraId, char* Name);

//---------------•º≈Á√“

__declspec(dllexport)  void CameraManager_Grab_Int(int cameraId, unsigned  int* imgPtr);
__declspec(dllexport)  void CameraManager_Grab_Char(int cameraId, unsigned char* imgPtr);

__declspec(dllexport) void CameraManager_FreeIntptrMemoryInt(unsigned  int* imgPtr);
__declspec(dllexport) void CameraManager_FreeIntptrMemoryChar(unsigned  char* imgPtr);

__declspec(dllexport)  void CameraManager_SaveParam(int cameraId);

__declspec(dllexport)  void CameraManager_LoadParam(int cameraId);

__declspec(dllexport)  void CameraManager_SimulationImageSource(char* chrArrImageSource);
