
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

StApi::CStApiAutoInit	_StApiAutoInit;
StApi::CIStSystemPtrArray _StSystemPtrList;
static vector< ICamera*> lstCameraOmronAll; //為了適應多種類相機的使用 必須為物件必須為指標 ,才能正常轉型成為各種相機,方便使用

bool _IsInitialed = false;

void CameraManager::GetDeviceFromStpApi()
{
	const uint32_t nCount = StSystemVendor_Count;
	for (uint32_t i = StSystemVendor_Default; i < nCount; ++i)
	{
		EStSystemVendor_t eStSystemVendor = (EStSystemVendor_t)i;
		try
		{
			_StSystemPtrList.Register(CreateIStSystem(eStSystemVendor, StInterfaceType_All));
		}
		catch (const GenICam::GenericException& e)
		{
			//if (eStSystemVendor == StSystemVendor_Default)
			//{
			std::cout << "Exception : " << e.what() << endl;

			string strErr(e.GetDescription());
			string str = "Exception : " + strErr;
			WriteLog(str);

		}
	}

	WriteLog("Omron Start Search Device...");


	for (size_t i = 0; i < _StSystemPtrList.GetSize(); ++i)
	{
		IStSystem* pIStSystem = _StSystemPtrList[i];
		for (size_t j = 0; j < pIStSystem->GetInterfaceCount(); ++j)
		{
			IStInterface* pIStInterface = pIStSystem->GetIStInterface(j);
			IStDeviceReleasable* pDevice = pIStInterface->CreateFirstIStDevice();

			if (pDevice->GetIStInterface())
			{
				OmronCameraObject* obj = new OmronCameraObject(pDevice);
				WriteLog("Omron Found : " + obj->CameraName());
				lstCameraOmronAll.push_back(obj);
			}
			break;
		}
	}
}

vector<ICamera*> CameraManager::GetCamera()
{
	__try
	{

		if (!_IsInitialed)
		{
			lstCameraOmronAll.clear();
			WriteLog("StApiInitialize...");

			StApiInitialize();

			WriteLog("Omron Start Search Interface ...");


			//----改成這樣的格式看看是否可以正常輸出
			GetDeviceFromStpApi();

			_IsInitialed = true;

		}

		vector<ICamera*> vCamera;

		for (int i = 0; i < lstCameraOmronAll.size(); i++)
			vCamera.push_back(lstCameraOmronAll[i]);

		WriteLog("Omron vCamera.size = " + to_string(vCamera.size()));

		std::cout << "OmronCamera Initial Success... \n";
		WriteLog("OmronCamera Initial Success...");


		return vCamera;

	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		WriteLog("StpApi Initial Fail.");
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
