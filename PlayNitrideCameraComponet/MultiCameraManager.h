#pragma once
#include <string>
#include <iostream>
#include<fstream>
//#include "pch.h"


using namespace std;

namespace CameraManager
{
	__declspec(dllexport)  void InitializeAllCamera();
	__declspec(dllexport)  void CloseAllCamera();

	__declspec(dllexport)  void Grab(int cameraId, unsigned  int*& imgPtr);

	__declspec(dllexport)  void GetAllCameraNames(string strCameraNameArray[]);

	__declspec(dllexport)  void SetCameraParam(int cameraId, string NodeName, string Value);
	__declspec(dllexport)  void GetCameraParam(int cameraId, string NodeName, string& Value);

	__declspec(dllexport)  void SetCameraParam(int cameraId, string NodeName[], string Value[]);
	__declspec(dllexport)  void GetCameraParam(int cameraId, string NodeName[], string Value[]);

	__declspec(dllexport)  void AcquisitionStart(int cameraId);
	__declspec(dllexport)  void AcquisitionStop(int cameraId);

}

//--以下是有被定義的 NodeName (所有相機通用的介面) 以後所有種類相機都套用以下規則
//
//  Red			:  Red Gain
//  Green		:  Green Gain
//  Blue		:  Blue Gain
//  Gain
// 其他未定義者,依照各相機原本的參數名稱