#include "CameraComponentForLabview.h"
#include "MultiCameraManager.h"


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

void CameraManager_Grab_Int(int cameraId, unsigned int* imgPtr)
{
	CameraManager::Grab(cameraId,imgPtr);
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
