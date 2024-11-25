#include "MultiCameraManager.h"
#include "ICamera.h"
#include <vector>
#include "ArenaCameraObject.h"


static vector< ICamera*> lstCamera; //���F�A���h�����۾����ϥ� ���������󥲶������� ,�~�ॿ�`�૬�����U�ج۾�,��K�ϥ�

#pragma region Arena�۾� �@�Ϊ���
static Arena::ISystem* _System; //�P�ɥu��s�b�@�ӥ�����b�~�� �O�䬰static

#pragma endregion

void CameraManager::InitializeAllCamera()
{
	if (lstCamera.size() != 0)
		return;

#pragma region ��l�� Arena�۾�

	_System = Arena::OpenSystem();
	_System->UpdateDevices(100);

	std::vector<Arena::DeviceInfo> _deviceInfos= _System->GetDevices();

	for (int u = 0; u < _deviceInfos.size(); u++)
	{
		ArenaCameraObject* ACO =new ArenaCameraObject(_System,_deviceInfos[u]);
		lstCamera.push_back(ACO);
	}

#pragma endregion



}

void CameraManager::CloseAllCamera()
{
#pragma region ���� Arena�۾�

	for (int i = 0; i < lstCamera.size(); i++)
		lstCamera[i]->Close();

	lstCamera.clear();
	Arena::CloseSystem(_System);

#pragma region ���� Arena�۾�

}

void CameraManager::Grab(int cameraId, unsigned int*& imgPtr)
{
	if (cameraId < 0 || cameraId >= lstCamera.size())
		return;

	lstCamera[cameraId]->Grab(imgPtr);
}


void CameraManager::GetAllCameraNames(string strCameraNameArray[])
{
	for (int i = 0; i < lstCamera.size(); i++)
		strCameraNameArray[i] = lstCamera[i]->CameraName();
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

	lstCamera[cameraId]->GetCameraParam(NodeName, Value);
}

void CameraManager::SetCameraParam(int cameraId, string NodeName[], string Value[])
{
	if (cameraId < 0 || cameraId >= lstCamera.size())
		return;

	if (Value->size() < NodeName->size())
		return;

	for(int i=0;i<NodeName->size();i++)
		lstCamera[cameraId]->SetCameraParam(NodeName[i], Value[i]);
}

void CameraManager::GetCameraParam(int cameraId, string NodeName[], string Value[])
{
	if (cameraId < 0 || cameraId >= lstCamera.size())
		return;

	if (Value->size() < NodeName->size())
		return;

	for (int i = 0; i < NodeName->size(); i++)
		lstCamera[cameraId]->GetCameraParam(NodeName[i], Value[i]);
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
