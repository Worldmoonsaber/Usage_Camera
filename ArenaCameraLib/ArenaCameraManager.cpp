#include "ArenaCameraManager.h"
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
#include <direct.h>
#include <stdlib.h> // free, perror
#include <stdio.h>  // printf
#include <io.h> // strlen
#include <chrono>

#pragma region 靜態物件
static map<string, int> map_CameraName_Indx;
static vector< ICamera*> lstCameraArenaAll; //為了適應多種類相機的使用 必須為物件必須為指標 ,才能正常轉型成為各種相機,方便使用
#pragma region Arena相機 共用物件
static Arena::ISystem* _System; //同時只能存在一個必須放在外面 令其為static
static bool _isArenaSystemOpened=false;
#pragma endregion

#pragma endregion
static bool isExistPath = false;

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
		logFile << str << ":: Arena ::" << message << std::endl; // 寫入日誌內容並換行
	}
	else
	{
		std::cerr << "Unable to open log file." << std::endl; // 文件打開失敗時打印錯誤
	}
}

vector<ICamera*> CameraManager::GetCamera()
{
#pragma region 初始化 Arena相機
	try
	{
		std::cout << "Initialize ArenaCamera ... \n";

		WriteLog("Initialize ArenaCamera ...");

		if (!_isArenaSystemOpened)
		{
			_System = Arena::OpenSystem();
			_isArenaSystemOpened = true;
			_System->UpdateDevices(100);
			std::vector<Arena::DeviceInfo> _deviceInfos = _System->GetDevices();

			for (int u = 0; u < _deviceInfos.size(); u++)
			{
				ArenaCameraObject* ACO = new ArenaCameraObject(_System, _deviceInfos[u]);
				lstCameraArenaAll.push_back(ACO);
				map_CameraName_Indx.insert(pair<string, int>(ACO->CameraName(), lstCameraArenaAll.size() - 1));
			}

		}
	}
	catch (exception ex)
	{
		WriteLog(ex.what());
	}

#pragma endregion

	WriteLog("Get Arena Camera Count: " + to_string(lstCameraArenaAll.size()));
	std::cout << "Get ArenaCamera Count " + to_string(lstCameraArenaAll.size()) << endl;

	vector< ICamera*> vCamera;

	for (int u = 0; u < lstCameraArenaAll.size(); u++)
		vCamera.push_back(lstCameraArenaAll[u]);

	return vCamera;
}

void CameraManager::CloseCamera()
{
#pragma region 關閉 Arena相機

	std::cout << "Arena::Close ArenaCamera ... \n";


	for (int i = 0; i < lstCameraArenaAll.size(); i++)
		lstCameraArenaAll[i]->Close();

	if (_isArenaSystemOpened)
	{
		_isArenaSystemOpened = false;
		Arena::CloseSystem(_System);
	}
#pragma endregion 關閉 Arena相機

	lstCameraArenaAll.clear();
	WriteLog("All Camera Closed");
}
