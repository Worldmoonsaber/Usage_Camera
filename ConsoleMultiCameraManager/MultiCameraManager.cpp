#include "MultiCameraManager.h"
#include "ICamera.h"
#include <vector>
#include "FakeCamera.h"
#include "PersudoCamera.h"

static vector< ICamera*> lstCamera;

void CameraManager::InitializeAllCamera()
{
	if (lstCamera.size() != 0)
		return;

	FakeCamera* fc=new FakeCamera();
	lstCamera.push_back((ICamera*)fc);
	ICamera* ic=new ICamera();
	lstCamera.push_back(ic);

}

void CameraManager::CloseAllCamera()
{
	for (int i = 0; i < lstCamera.size(); i++)
	{
		lstCamera[i]->Close();
	}
	lstCamera.clear();
}

void CameraManager::Grab(int cameraId, unsigned int*& imgPtr)
{
	if (cameraId < 0 || cameraId >= lstCamera.size())
		return;

	lstCamera[cameraId]->Grab(imgPtr);
}

void CameraManager::Grab(int cameraId, unsigned char*& imgPtr)
{
	if (cameraId < 0 || cameraId >= lstCamera.size())
		return;

	lstCamera[cameraId]->Grab(imgPtr);
}

void CameraManager::GetAllCameraNames(string strCameraNameArray[])
{
	for (int i = 0; i < lstCamera.size(); i++)
	{
		strCameraNameArray[i] = lstCamera[i]->CameraName();
	}
}


void CameraManager::SetCameraParam(int cameraId, string NodeName, string Value)
{
	if (cameraId < 0 || cameraId >= lstCamera.size())
		return;

	lstCamera[cameraId]->SetCameraParam(NodeName,Value);
}

void CameraManager::GetCameraParam(int cameraId, string NodeName, string& Value)
{
	if (cameraId < 0 || cameraId >= lstCamera.size())
		return;

	lstCamera[cameraId]->SetCameraParam(NodeName, Value);
}

void CameraManager::AcquisitionStart(int cameraId)
{
	if (cameraId < 0 || cameraId >= lstCamera.size())
		return;

	lstCamera[cameraId]->AcquisitionStart();
}

void CameraManager::AcquisitionStop(int cameraId)
{
	if (cameraId < 0 || cameraId >= lstCamera.size())
		return;

	lstCamera[cameraId]->AcquisitionStop();
}
