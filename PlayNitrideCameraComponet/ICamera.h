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
	//-----�ثe�٨S���Q��ܦ��Ī����� �NLog�T���ߨ� SubVi �o�����d�쥼�ӧ�i �Ȯɤ��i��W��

protected:
	string _strName = "No Name"; //�~�ӫ�i�H�Q�ݨ�

private:
	int _Width;
	int _Height;
};