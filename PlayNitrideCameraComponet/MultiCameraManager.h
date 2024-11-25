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

//--�H�U�O���Q�w�q�� NodeName (�Ҧ��۾��q�Ϊ�����) �H��Ҧ������۾����M�ΥH�U�W�h
//
//  Red			:  Red Gain
//  Green		:  Green Gain
//  Blue		:  Blue Gain
//  Gain
// ��L���w�q��,�̷ӦU�۾��쥻���ѼƦW��