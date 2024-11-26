#pragma once
#include <string>
#include <iostream>
//#include "pch.h"

using namespace std;

class ICamera
{

public:
	ICamera();
	~ICamera();

	virtual void Initialize();
	virtual void Close();

	virtual void Grab(unsigned  int*& imgPtr);

	virtual void Grab(unsigned char*& imgPtr);


	virtual void SetCameraParam(string NodeName, string Value);
	virtual void GetCameraParam(string NodeName, string& Value);

	virtual void AcquisitionStart();
	virtual void AcquisitionStop();

	string CameraName();
	//-----目前還沒有想到很有效的機制 將Log訊息拋到 SubVi 這部分留到未來改進 暫時不進行規劃

protected:
	string _strName = "No Name"; //繼承後可以被看到

private:
	int _Width;
	int _Height;
};