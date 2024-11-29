#pragma once


__declspec(dllexport)  void CameraManager_InitializeAllCamera();

__declspec(dllexport)  int	CameraManager_CameraCount();
__declspec(dllexport)  void CameraManager_CloseAllCamera();


__declspec(dllexport)  void CameraManager_Grab(int cameraId, unsigned  int*& imgPtr);

__declspec(dllexport)  void CameraManager_SetCameraParam(int cameraId, const char* NodeName, const char* Value);
__declspec(dllexport)  void CameraManager_GetCameraParam(int cameraId, const char* NodeName, const char*& Value);


__declspec(dllexport)  void CameraManager_AcquisitionStart(int cameraId);
__declspec(dllexport)  void CameraManager_AcquisitionStop(int cameraId);

__declspec(dllexport) void CameraManager_FreeIntptrMemoryInt(unsigned  int* imgPtr);
__declspec(dllexport) void CameraManager_FreeIntptrMemoryChar(unsigned  char* imgPtr);

__declspec(dllexport) void CameraManager_GetAllCamera(const char**& array);

__declspec(dllexport) void CameraManager_GetErrorLog(const char**& array);


__declspec(dllexport)  void CameraManager_GetCameraName(int cameraId, const char*& Name);


__declspec(dllexport)  void CameraManager_SaveParam(int cameraId);

__declspec(dllexport)  void CameraManager_LoadParam(int cameraId);
