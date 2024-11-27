#include "MultiCameraManager.h"
#include "ICamera.h"
#include <vector>
#include "ArenaCameraObject.h"
#include <cstring>
#include <cstdlib>
#include<opencv2/opencv.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp> //mophorlogical operation
#include<opencv2/core.hpp>
#include <opencv2/core/mat.hpp>


static vector< ICamera*> lstCamera; //為了適應多種類相機的使用 必須為物件必須為指標 ,才能正常轉型成為各種相機,方便使用
#pragma region Arena相機 共用物件
static Arena::ISystem* _System; //同時只能存在一個必須放在外面 令其為static
#pragma endregion

void CameraManager::InitializeAllCamera()
{
	if (lstCamera.size() != 0)
		return;

#pragma region 初始化 Arena相機

	_System = Arena::OpenSystem();
	_System->UpdateDevices(100);

	std::vector<Arena::DeviceInfo> _deviceInfos= _System->GetDevices();

	for (int u = 0; u < _deviceInfos.size(); u++)
	{
		ArenaCameraObject* ACO =new ArenaCameraObject(_System,_deviceInfos[u]);
		lstCamera.push_back(ACO);
	}

#pragma endregion

	ICamera* ic = new ICamera();
	lstCamera.push_back(ic);


	cout << "已偵測相機數量: " << lstCamera.size() << endl;
}

int CameraManager::CameraCount()
{
	return lstCamera.size();
}

void CameraManager::CloseAllCamera()
{

#pragma region 關閉 Arena相機

	for (int i = 0; i < lstCamera.size(); i++)
		lstCamera[i]->Close();

	Arena::CloseSystem(_System);
#pragma endregion 關閉 Arena相機

	lstCamera.clear();
}

void CameraManager::Grab(int cameraId, unsigned int*& imgPtr)
{
	if (cameraId < 0 || cameraId >= lstCamera.size())
		return;

	lstCamera[cameraId]->Grab(imgPtr);
}

void CameraManager::Grab(int cameraId, void*& imgPtr)
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

	for (int i = 0; i < NodeName->size(); i++)
	{
		lstCamera[cameraId]->SetCameraParam(NodeName[i], Value[i]);
		cout << "NodeName:" << NodeName[i] << " Value:"<< Value[i] << endl;

	}
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

void CameraManager::FreeIntptrMemoryInt(unsigned int* imgPtr)
{
	CSharp_FreeIntptrMemory((void*)imgPtr);
}

void CameraManager::FreeIntptrMemoryChar(unsigned char* imgPtr)
{
	CSharp_FreeIntptrMemory((void*)imgPtr);
}

void CameraManager::FreeIntptrMemoryImage(void* imgPtr)
{
	delete[] static_cast<char*>(imgPtr);
}

void CameraManager::SaveCurrentCameraParam(int cameraId)
{
	if (cameraId < 0 || cameraId >= lstCamera.size())
		return;

	lstCamera[cameraId]->Save();

}

void CameraManager::LoadSavedCameraParam(int cameraId)
{
	if (cameraId < 0 || cameraId >= lstCamera.size())
		return;

	lstCamera[cameraId]->Load();

}


#pragma region CSharp

void CSharp_InitializeAllCamera()
{
	CameraManager::InitializeAllCamera();
}

void CSharp_CloseAllCamera()
{
	CameraManager::CloseAllCamera();
}

void* CSharp_Grab(int cameraId)
{	
	string strVal;
	CameraManager::GetCameraParam(0, "Channels", strVal);

	int channels = atoi(strVal.c_str());

	CameraManager::GetCameraParam(0, "Width", strVal);
	int Width = atoi(strVal.c_str());

	CameraManager::GetCameraParam(0, "Height", strVal);
	int Height = atoi(strVal.c_str());

	unsigned int* ptr = (unsigned int*)malloc(Width * Height * 8 * channels); //必須先提供記憶大小
		
	CameraManager::Grab(cameraId, ptr);

	return ptr;
}

void CSharp_SetCameraParam(int cameraId, const char* NodeName, const char* Value)
{
	CameraManager::SetCameraParam(cameraId, NodeName, Value);
}

const char* CSharp_GetCameraParam(int cameraId, const char* NodeName)
{
	string strVal = "";
	CameraManager::GetCameraParam(cameraId, NodeName, strVal);

	cout << "strVal:" << strVal << endl;

	char* res = (char*)malloc(strVal.size() + 1);
	strcpy(res, strVal.c_str());

	return res;
}

void CSharp_AcquisitionStart(int cameraId)
{
	CameraManager::AcquisitionStart(cameraId);
}

void CSharp_AcquisitionStop(int cameraId)
{
	CameraManager::AcquisitionStop(cameraId);
}

void CSharp_GetAllCamera(const char** array) 
{
	string strCameraNameArray[20];
	CameraManager::GetAllCameraNames(strCameraNameArray);
	
	for (int i = 0; i < strCameraNameArray->length(); i++)
	{
		if (strCameraNameArray[i] != "")
		{
			char* result = (char*)malloc(strCameraNameArray[i].size() + 1);
			strcpy(result, strCameraNameArray[i].c_str());
			array[i] = result;
		}
	}

}

void CSharp_FreeIntptrMemory(void* str)
{
	free((void*)str);
}

#pragma endregion
