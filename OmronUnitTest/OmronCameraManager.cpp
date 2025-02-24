
// StViewer.cpp : Defines the class behaviors for the application.
//

#include "OmronCameraManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include <direct.h>
#include <stdlib.h> // free, perror
#include <stdio.h>  // printf
#include <io.h> // strlen
#include <chrono>

using namespace StApi;
using namespace std;

//-----------------------------------------------------------------------------
OmronCameraManager::OmronCameraManager()
try :
	// Before using the library, perform the initialization. 
	m_objStApiAutoInit()
{
	InitInstance();

	for (size_t i = 0; i < m_objIStSystemPtrList.GetSize(); ++i)
	{
		IStSystem* pIStSystem = m_objIStSystemPtrList[i];
		for (size_t j = 0; j < pIStSystem->GetInterfaceCount(); ++j)
		{
			IStInterface* pIStInterface = pIStSystem->GetIStInterface(j);
			IStDeviceReleasable* pDevice = pIStInterface->CreateFirstIStDevice();

			if (pDevice->GetIStInterface())
			{
				OmronCameraObject* obj = new OmronCameraObject(pDevice);
				vOmronCamera.push_back(obj);
			}
			break;
		}
	}

}
catch (const GenICam::GenericException& e)
{
	std::cout << "OmronCameraManager::GenericException : "<<e.what() << endl;
}
catch (const GenICam_3_2_Sentech::RuntimeException& e)
{
	std::cout << "OmronCameraManager::RuntimeException : " << e.what() << endl;
}

OmronCameraManager theOmronCameraList;

bool OmronCameraManager::InitInstance()
{

	const uint32_t nCount = StSystemVendor_Count;
	for (uint32_t i = StSystemVendor_Default; i < nCount; ++i)
	{
		EStSystemVendor_t eStSystemVendor = (EStSystemVendor_t)i;
		try
		{
			m_objIStSystemPtrList.Register(CreateIStSystem(eStSystemVendor, StInterfaceType_All));
		}
		catch (const GenICam::GenericException& e)
		{
			if (eStSystemVendor == StSystemVendor_Default)
			{
				std::cout << "InitInstance::Exception : " << e.what() << endl;
			}
		}
	}

	return TRUE;
}


bool isExistPath = false;

void CameraManager::WriteLog(const std::string& message) {

	SYSTEMTIME st;
	GetLocalTime(&st);

	if (!isExistPath)
	{
		string strPath;
		char* buffer;

		// Get the current working directory:
		if ((buffer = _getcwd(NULL, 0)) != NULL)
		{
			strPath.assign(buffer, strlen(buffer));
			free(buffer);
		}

		strPath = strPath + "\\Log\\";

		if (_access(strPath.c_str(), 0) == -1)
		{
			_mkdir(strPath.c_str());
			isExistPath = true;
		}
		else
			isExistPath = true;
	}


	std::ofstream logFile("Log\\camera_manager_log_" + to_string(st.wMonth) + "-" + to_string(st.wDay) + ".txt", std::ios::app); // 以追加模式打開文件

	if (logFile.is_open())
	{
		SYSTEMTIME st;
		GetLocalTime(&st);
		string str = to_string(st.wMonth) + "-" + to_string(st.wDay) + " " + to_string(st.wHour) + ":" + to_string(st.wMinute) + ":" + to_string(st.wSecond) + ":" + to_string(st.wMilliseconds);
		logFile << str << ":: Omron ::" << message << std::endl; // 寫入日誌內容並換行
	}
	else
	{
		std::cerr << "Unable to open log file." << std::endl; // 文件打開失敗時打印錯誤
	}
}


static vector< ICamera*> lstCameraOmronAll; //為了適應多種類相機的使用 必須為物件必須為指標 ,才能正常轉型成為各種相機,方便使用


vector<ICamera*> CameraManager::GetCamera()
{
	try
	{

		for (int i = 0; i < theOmronCameraList.vOmronCamera.size(); i++)
		{
			lstCameraOmronAll.push_back((ICamera*)theOmronCameraList.vOmronCamera[i]);
		}
		std::cout << "Initialize OmronCamera ... \n";

		WriteLog("Initialize OmronCamera ...");

	}
	catch (exception ex)
	{
		WriteLog(ex.what());
	}

	return lstCameraOmronAll;
}

void CameraManager::CloseCamera()
{
	std::cout << "Omron::Close ArenaCamera ... \n";

	for (int i = 0; i < lstCameraOmronAll.size(); i++)
		lstCameraOmronAll[i]->Close();

	lstCameraOmronAll.clear();
}
