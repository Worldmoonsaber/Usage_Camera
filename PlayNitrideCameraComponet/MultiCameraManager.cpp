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
#include <direct.h>
#include <stdlib.h> // free, perror
#include <stdio.h>  // printf
#include <io.h> // strlen
#include <chrono>

#pragma region 靜態物件
static map<string, int> map_CameraName_Indx;
static vector< ICamera*> lstCamera; //為了適應多種類相機的使用 必須為物件必須為指標 ,才能正常轉型成為各種相機,方便使用
#pragma region Arena相機 共用物件
static Arena::ISystem* _System; //同時只能存在一個必須放在外面 令其為static
static bool _isArenaSystemOpened=false;

#pragma endregion

static string _StrSimulationFile;
static cv::Mat _ImgSimulation;


void _returnSimulationImg(void*& imgptr)
{
	if (_ImgSimulation.empty())
		return;

	int _Width = _ImgSimulation.size().width;
	int _Height = _ImgSimulation.size().height;

	uint8_t* data = new uint8_t[_Width * _Height * 4];

	int count = 0;

	for (int j = 0; j < _Height; j++)
		for (int i = 0; i < _Width; i++)
		{
			cv::Vec4b pixel = _ImgSimulation.at<cv::Vec4b>(j, i);

			data[count * 4] = pixel[0];     // B
			data[count * 4 + 1] = pixel[1];   // G
			data[count * 4 + 2] = pixel[2];   // R
			data[count * 4 + 3] = pixel[3]; // A

			count++;
		}

	imgptr = (unsigned int*)data;
}

#pragma endregion

void CameraManager::InitializeAllCamera()
{
	if (lstCamera.size() != 0)
		return;

#pragma region 初始化 Arena相機
	try
	{

		if (!_isArenaSystemOpened)
		{
			_System = Arena::OpenSystem();
			_isArenaSystemOpened = true;
		}

		_System->UpdateDevices(100);

		std::vector<Arena::DeviceInfo> _deviceInfos = _System->GetDevices();

		for (int u = 0; u < _deviceInfos.size(); u++)
		{
			ArenaCameraObject* ACO = new ArenaCameraObject(_System, _deviceInfos[u]);

			lstCamera.push_back(ACO);

			map_CameraName_Indx.insert(pair<string, int>(ACO->CameraName(), lstCamera.size() - 1));
		}
	}
	catch (exception ex)
	{
		WriteLog(ex.what());
	}

#pragma endregion

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

	//if (_isArenaSystemOpened)
	//{
	try
	{
		_isArenaSystemOpened = false;
		Arena::CloseSystem(_System);
	}
	catch (exception ex)
	{
		WriteLog(ex.what());

	}
	//}
#pragma endregion 關閉 Arena相機

	lstCamera.clear();

	WriteLog("All Camera Closed");

}

void CameraManager::Grab(int cameraId, unsigned int*& imgPtr)
{
	if (cameraId < 0 || cameraId >= lstCamera.size())
	{
		WriteLog("the Camera Index " + to_string(cameraId) + " Not Exist");
		_returnSimulationImg((void*&)imgPtr);

		return;
	}


	lstCamera[cameraId]->Grab((void*&)imgPtr);
}

void CameraManager::Grab(int cameraId, void*& imgPtr)
{
	if (cameraId < 0 || cameraId >= lstCamera.size())
	{
		WriteLog("the Camera Index "+to_string(cameraId)+" Not Exist");

		//------取得模擬影像
		_returnSimulationImg(imgPtr);
		return;
	}

	lstCamera[cameraId]->Grab(imgPtr);
}


void CameraManager::GetAllCameraNames(string strCameraNameArray[])
{
	for (int i = 0; i < lstCamera.size(); i++)
		strCameraNameArray[i] = lstCamera[i]->CameraName();
}

void CameraManager::GetAllLog(string strLog[])
{
	_icamera_getLog(strLog);
}

void CameraManager::GetCameraName(int cameraId,string& strCameraNameArray)
{
	if (cameraId < 0 || cameraId >= lstCamera.size())
	{
		WriteLog("the Camera Index " + to_string(cameraId) + " Not Exist");
		return;
	}

	strCameraNameArray = lstCamera[cameraId]->CameraName();


}


void CameraManager::SetCameraParam(int cameraId, string NodeName, string Value)
{
	WriteLog("SetCameraParam: " + to_string(cameraId) + ":" + NodeName+":"+ Value);

	if (cameraId < 0 || cameraId >= lstCamera.size())
	{
		WriteLog("the Camera Index " + to_string(cameraId) + " Not Exist");
		return;
	}

	lstCamera[cameraId]->SetCameraParam(NodeName,Value);

}

void CameraManager::GetCameraParam(int cameraId, string NodeName, string& Value)
{

	if (cameraId < 0 || cameraId >= lstCamera.size())
	{
		WriteLog("the Camera Index " + to_string(cameraId) + " Not Exist");
		return;
	}

	lstCamera[cameraId]->GetCameraParam(NodeName, Value);

//	WriteLog("GetCameraParam: " + to_string(cameraId) + ":" + NodeName + ":" + Value);

}

void CameraManager::SetCameraParam(int cameraId, string NodeName[], string Value[])
{
	if (cameraId < 0 || cameraId >= lstCamera.size())
	{
		WriteLog("the Camera Index " + to_string(cameraId) + " Not Exist");
		return;
	}

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
	{
		WriteLog("the Camera Index " + to_string(cameraId) + " Not Exist");
		return;
	}

	if (Value->size() < NodeName->size())
		return;

	for (int i = 0; i < NodeName->size(); i++)
		lstCamera[cameraId]->GetCameraParam(NodeName[i], Value[i]);

}

void CameraManager::AcquisitionStart(int cameraId)
{
	if (cameraId < 0 || cameraId >= lstCamera.size())
	{
		WriteLog("the Camera Index " + to_string(cameraId) + " Not Exist");
		return;
	}

	lstCamera[cameraId]->AcquisitionStart();
}

void CameraManager::AcquisitionStop(int cameraId)
{
	if (cameraId < 0 || cameraId >= lstCamera.size())
	{
		WriteLog("the Camera Index " + to_string(cameraId) + " Not Exist");
		return;
	}

	lstCamera[cameraId]->AcquisitionStop();
}

void CameraManager::ExcuteCmd(int cameraId, string Command)
{
	if (cameraId < 0 || cameraId >= lstCamera.size())
	{
		WriteLog("the Camera Index " + to_string(cameraId) + " Not Exist");
		return;
	}

	lstCamera[cameraId]->Excute(Command);
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
	{
		WriteLog("the Camera Index " + to_string(cameraId) + " Not Exist");
		return;
	}

	lstCamera[cameraId]->Save();

}

void CameraManager::LoadSavedCameraParam(int cameraId)
{
	if (cameraId < 0 || cameraId >= lstCamera.size())
	{
		WriteLog("the Camera Index " + to_string(cameraId) + " Not Exist");
		return;
	}

	lstCamera[cameraId]->Load();

}

void CameraManager::SetSimulationImageSource(string strSourceFile)
{
	_StrSimulationFile = strSourceFile;

	if (!_ImgSimulation.empty())
		_ImgSimulation.release();

	if ((_access(_StrSimulationFile.c_str(), 0)) != -1)
	{
		_ImgSimulation = cv::imread(_StrSimulationFile);

		if (_ImgSimulation.channels() == 1)
			cv::cvtColor(_ImgSimulation, _ImgSimulation, cv::COLOR_GRAY2RGBA);
		else if (_ImgSimulation.channels() == 3)
			cv::cvtColor(_ImgSimulation, _ImgSimulation, cv::COLOR_RGB2RGBA);
	}
}

void CameraManager::Grab_byCameraNickName(string strCameraNickname, unsigned int*& imgPtr)
{
	CameraManager::Grab(map_CameraName_Indx[strCameraNickname], imgPtr);
}

void CameraManager::Grab_byCameraNickName(string strCameraNickname, void*& imgPtr)
{
	CameraManager::Grab(map_CameraName_Indx[strCameraNickname], imgPtr);
}

void CameraManager::SetCameraParam_byCameraNickName(string strCameraNickname, string NodeName, string Value)
{
	CameraManager::SetCameraParam(map_CameraName_Indx[strCameraNickname], NodeName,Value);
}

void CameraManager::GetCameraParam_byCameraNickName(string strCameraNickname, string NodeName, string& Value)
{
	CameraManager::GetCameraParam(map_CameraName_Indx[strCameraNickname], NodeName, Value);

}

void CameraManager::SetCameraParam_byCameraNickName(string strCameraNickname, string NodeName[], string Value[])
{
	CameraManager::SetCameraParam(map_CameraName_Indx[strCameraNickname], NodeName, Value);
}

void CameraManager::GetCameraParam_byCameraNickName(string strCameraNickname, string NodeName[], string Value[])
{
	CameraManager::GetCameraParam(map_CameraName_Indx[strCameraNickname], NodeName, Value);
}

void CameraManager::AcquisitionStart_byCameraNickName(string strCameraNickname)
{
	CameraManager::AcquisitionStart(map_CameraName_Indx[strCameraNickname]);
}

void CameraManager::AcquisitionStop_byCameraNickName(string strCameraNickname)
{
	CameraManager::AcquisitionStop(map_CameraName_Indx[strCameraNickname]);
}

void CameraManager::SaveCurrentCameraParam_byCameraNickName(string strCameraNickname)
{
	CameraManager::SaveCurrentCameraParam(map_CameraName_Indx[strCameraNickname]);
}

void CameraManager::LoadSavedCameraParam_byCameraNickName(string strCameraNickname)
{
	CameraManager::LoadSavedCameraParam(map_CameraName_Indx[strCameraNickname]);
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

	void* ptr = malloc(Width * Height * 8 * channels); //必須先提供記憶大小
		
	CameraManager::Grab(cameraId, ptr);

	//if (channels == 1)
	//{
	//	uint8_t* data = new uint8_t[Width * Height * 4];

	//	int count = 0;
	//	byte* bArr = (byte*)ptr;
	//	for (int j = 0; j < Height; j++)
	//		for (int i = 0; i < Width; i++)
	//		{
	//			data[count * 4] = bArr[count];     // B
	//			data[count * 4 + 1] = bArr[count];   // G
	//			data[count * 4 + 2] = bArr[count];   // R
	//			data[count * 4 + 3] = 0; // A

	//			count++;
	//		}

	//	CSharp_FreeIntptrMemory(ptr);
	//	return bArr;
	//}

	return ptr;
}

void CSharp_ExcuteCmd(int cameraId, const char* Command)
{
	CameraManager::ExcuteCmd(cameraId, Command);
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

void CSharp_GetErrorLog(const char** array)
{
	string strLog[100];
	CameraManager::GetAllLog(strLog);

	for (int i = 0; i < strLog->length(); i++)
	{
		if (strLog[i] != "")
		{
			char* result = (char*)malloc(strLog[i].size() + 1);
			strcpy(result, strLog[i].c_str());
			array[i] = result;
		}
	}
}

const char* CSharp_GetCurrntVersion()
{
	string strVal = "1.0.0.3";
	char* res = (char*)malloc(strVal.size() + 1);
	strcpy(res, strVal.c_str());

	return res;
}

void CSharp_SaveCurrentParameter(int cameraId)
{
	CameraManager::SaveCurrentCameraParam(cameraId);
}

void CSharp_LoadDefaultParameter(int cameraId)
{
	CameraManager::LoadSavedCameraParam(cameraId);
}



#pragma endregion


//bool isExistPath = false;
//
///// <summary>
///// Debug時使用
///// </summary>
///// <param name="message"></param>
//void WriteLog(const std::string& message) {
//
//	SYSTEMTIME st;
//	GetLocalTime(&st);
//
//	if (!isExistPath)
//	{
//		string strPath;
//		char* buffer;
//
//		// Get the current working directory:
//		if ((buffer = _getcwd(NULL, 0)) != NULL)
//		{
//			strPath.assign(buffer, strlen(buffer));
//			free(buffer);
//		}
//
//		strPath = strPath + "\\Log\\";
//
//		if (_access(strPath.c_str(), 0) == -1)
//		{
//			_mkdir(strPath.c_str());
//			isExistPath = true;
//		}
//		else
//			isExistPath = true;
//	}
//
//	std::ofstream logFile("Log\\camera_manager_log_" + to_string(st.wMonth) + "-" + to_string(st.wDay) + ".txt", std::ios::app); // 以追加模式打開文件
//	if (logFile.is_open())
//	{
//		string str = to_string(st.wMonth) + "-" + to_string(st.wDay) + " " + to_string(st.wHour) + ":" + to_string(st.wMinute) + ":" + to_string(st.wSecond) + ":" + to_string(st.wMilliseconds);
//
//		logFile << str << " " << message << std::endl; // 寫入日誌內容並換行
//	}
//	else {
//		std::cerr << "Unable to open log file." << std::endl; // 文件打開失敗時打印錯誤
//	}
//}