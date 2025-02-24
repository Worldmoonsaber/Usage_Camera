#include "MultiCameraManager.h"
#include "ICamera.h"
#include <vector>
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
#include <iostream>
#include "DllResouceImporter.h"

#pragma region 靜態物件
static map<string, int> map_CameraName_Indx;
static vector< ICamera*> lstAllCamera; //為了適應多種類相機的使用 必須為物件必須為指標 ,才能正常轉型成為各種相機,方便使用
static vector <HMODULE> lstDynamicDllResource;

static string _StrSimulationFile;
static cv::Mat _ImgSimulation;


vector<wstring> vGetDLLPath()
{

	vector<wstring> vStr;

	std::ifstream file("dll\\CameraDll.txt"); // 開啟檔案
	if (!file) {
		std::cerr << "無法開啟檔案！" << std::endl;
		return vStr;
	}

	std::string line;
	while (std::getline(file, line)) 
	{ // 逐行讀取
		if (line.find("CameraLib") != std::string::npos)
		{
			cout << "Dectect CameraLib :"+line << endl;

			std::wstring wsName(line.begin(), line.end());

			wstring wstr = L"dll\\" + wsName;

			vStr.push_back(wstr);
		}
	}

	file.close(); // 關閉檔案

	return vStr;
}


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
	cout << "InitializeAllCamera " << endl;

	if (lstAllCamera.size() != 0)
		return;

	vector<wstring> vWStrPath= vGetDLLPath();

	for (int i = 0; i < vWStrPath.size(); i++)
	{
		std::string strTitle(vWStrPath[i].begin(), vWStrPath[i].end());

		WriteLog("開始載入 "+ strTitle);
		HMODULE hDLL = LoadLibrary((LPCWSTR)vWStrPath[i].c_str());// (LPCWSTR)vDllPath[0].c_str());

		if (hDLL != NULL)
		{
			lstDynamicDllResource.push_back(hDLL);
			vector< ICamera*> vObj = DllResourceImporter::GetCameraFromDll(hDLL);

			for (int j = 0; j < vObj.size(); j++)
				lstAllCamera.push_back(vObj[j]);

			WriteLog("載入成功 " + strTitle);
		}
		else
		{
			WriteLog("載入失敗 " + strTitle);
			continue;
		}
	}

	cout << "InitializeFinished " << endl;

}

int CameraManager::CameraCount()
{
	return (int)lstAllCamera.size();
}

void CameraManager::CloseAllCamera()
{
	for (int i = 0; i < lstDynamicDllResource.size(); i++)
	{
		DllResourceImporter::CloseDllCamera(lstDynamicDllResource[i]);
	}

	for (int i = 0; i < lstAllCamera.size(); i++)
		lstAllCamera[i]->Close();

	lstDynamicDllResource.clear();
	lstAllCamera.clear();
	WriteLog("All Camera Closed");
}

void CameraManager::Grab(int cameraId, unsigned int*& imgPtr)
{
	if (cameraId < 0 || cameraId >= lstAllCamera.size())
	{
		WriteLog("the Camera Index " + to_string(cameraId) + " Not Exist");
		_returnSimulationImg((void*&)imgPtr);

		return;
	}


	lstAllCamera[cameraId]->Grab((void*&)imgPtr);
}

void CameraManager::Grab(int cameraId, void*& imgPtr)
{
	if (cameraId < 0 || cameraId >= lstAllCamera.size())
	{
		WriteLog("the Camera Index "+to_string(cameraId)+" Not Exist");

		//------取得模擬影像
		_returnSimulationImg(imgPtr);
		return;
	}

	lstAllCamera[cameraId]->Grab(imgPtr);
}

void CameraManager::GetAllCameraNames(string strCameraNameArray[])
{
	for (int i = 0; i < lstAllCamera.size(); i++)
		strCameraNameArray[i] = lstAllCamera[i]->CameraName();
}

void CameraManager::GetAllLog(string strLog[])
{
	_icamera_getLog(strLog);
}

void CameraManager::GetCameraName(int cameraId,string& strCameraNameArray)
{
	if (cameraId < 0 || cameraId >= lstAllCamera.size())
	{
		WriteLog("the Camera Index " + to_string(cameraId) + " Not Exist");
		return;
	}

	strCameraNameArray = lstAllCamera[cameraId]->CameraName();


}


void CameraManager::SetCameraParam(int cameraId, string NodeName, string Value)
{
	WriteLog("SetCameraParam: " + to_string(cameraId) + ":" + NodeName+":"+ Value);

	if (cameraId < 0 || cameraId >= lstAllCamera.size())
	{
		WriteLog("the Camera Index " + to_string(cameraId) + " Not Exist");
		return;
	}

	lstAllCamera[cameraId]->SetCameraParam(NodeName,Value);

}

void CameraManager::GetCameraParam(int cameraId, string NodeName, string& Value)
{

	if (cameraId < 0 || cameraId >= lstAllCamera.size())
	{
		WriteLog("the Camera Index " + to_string(cameraId) + " Not Exist");
		return;
	}

	lstAllCamera[cameraId]->GetCameraParam(NodeName, Value);

//	WriteLog("GetCameraParam: " + to_string(cameraId) + ":" + NodeName + ":" + Value);

}

void CameraManager::SetCameraParam(int cameraId, string NodeName[], string Value[])
{
	if (cameraId < 0 || cameraId >= lstAllCamera.size())
	{
		WriteLog("the Camera Index " + to_string(cameraId) + " Not Exist");
		return;
	}

	if (Value->size() < NodeName->size())
		return;

	for (int i = 0; i < NodeName->size(); i++)
	{
		lstAllCamera[cameraId]->SetCameraParam(NodeName[i], Value[i]);
		cout << "NodeName:" << NodeName[i] << " Value:"<< Value[i] << endl;

	}
}

void CameraManager::GetCameraParam(int cameraId, string NodeName[], string Value[])
{
	if (cameraId < 0 || cameraId >= lstAllCamera.size())
	{
		WriteLog("the Camera Index " + to_string(cameraId) + " Not Exist");
		return;
	}

	if (Value->size() < NodeName->size())
		return;

	for (int i = 0; i < NodeName->size(); i++)
		lstAllCamera[cameraId]->GetCameraParam(NodeName[i], Value[i]);

}

void CameraManager::AcquisitionStart(int cameraId)
{
	if (cameraId < 0 || cameraId >= lstAllCamera.size())
	{
		WriteLog("the Camera Index " + to_string(cameraId) + " Not Exist");
		return;
	}

	lstAllCamera[cameraId]->AcquisitionStart();
}

void CameraManager::AcquisitionStop(int cameraId)
{
	if (cameraId < 0 || cameraId >= lstAllCamera.size())
	{
		WriteLog("the Camera Index " + to_string(cameraId) + " Not Exist");
		return;
	}

	lstAllCamera[cameraId]->AcquisitionStop();
}

void CameraManager::ExcuteCmd(int cameraId, string Command)
{
	if (cameraId < 0 || cameraId >= lstAllCamera.size())
	{
		WriteLog("the Camera Index " + to_string(cameraId) + " Not Exist");
		return;
	}

	lstAllCamera[cameraId]->Excute(Command);
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
	if (cameraId < 0 || cameraId >= lstAllCamera.size())
	{
		WriteLog("the Camera Index " + to_string(cameraId) + " Not Exist");
		return;
	}

	lstAllCamera[cameraId]->Save();

}

void CameraManager::LoadSavedCameraParam(int cameraId)
{
	if (cameraId < 0 || cameraId >= lstAllCamera.size())
	{
		WriteLog("the Camera Index " + to_string(cameraId) + " Not Exist");
		return;
	}

	lstAllCamera[cameraId]->Load();

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
	string strVal = "2.0.1.3";
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

