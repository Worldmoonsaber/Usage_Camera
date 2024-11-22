#pragma once
#include <string>
#include <iostream>

using namespace std;

class ICamera
{

public:
	ICamera();
	~ICamera();

	virtual void Initialize();
	virtual void Close();

	virtual void Grab(unsigned  int*& imgPtr);

	virtual void SetCameraParam(string NodeName, string Value);
	virtual void GetCameraParam(string NodeName, string& Value);

	virtual void AcquisitionStart();
	virtual void AcquisitionStop();

	string CameraName();
protected:
	string _strName = "No Name"; //繼承後可以被看到

};