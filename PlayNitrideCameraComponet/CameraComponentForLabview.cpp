#include "CameraComponentForLabview.h"
#include "MultiCameraManager.h"

void CameraManager_InitializeAllCamera()
{
	CameraManager::InitializeAllCamera();
}

int CameraManager_CameraCount()
{
	return CameraManager::CameraCount();
}

void CameraManager_CloseAllCamera()
{
	return CameraManager::CloseAllCamera();
}

void CameraManager_Grab(int cameraId, unsigned int*& imgPtr)
{
	CameraManager::Grab(cameraId,imgPtr);
}

void CameraManager_Grab(int cameraId, unsigned char*& imgPtr)
{
	void* imgPtrV= (void*)imgPtr;
	CameraManager::Grab(cameraId, imgPtrV);
	

}

void CameraManager_SetCameraParam(int cameraId, const char* NodeName, const char* Value)
{
	CameraManager::SetCameraParam(cameraId, NodeName, Value);
}

void CameraManager_GetCameraParam(int cameraId, const char* NodeName, const char*& Value)
{
	Value=CSharp_GetCameraParam(cameraId,NodeName);
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

void CameraManager_GetAllCamera(const char**& array)
{
	CSharp_GetAllCamera(array);
}

void CameraManager_GetErrorLog(const char**& array)
{
	CSharp_GetErrorLog(array);
}

void CameraManager_GetCameraName(int cameraId, const char*& Name)
{
	string str;
	CameraManager::GetCameraName(cameraId, str);
	Name = str.c_str();
}

void CameraManager_SaveParam(int cameraId)
{
	CameraManager::SaveCurrentCameraParam(cameraId);
}

void CameraManager_LoadParam(int cameraId)
{
	CameraManager::LoadSavedCameraParam(cameraId);
}

void CameraManager_SimulationImageSource(const char* strImageSource)
{
	CameraManager::SetSimulationImageSource(strImageSource);
}
