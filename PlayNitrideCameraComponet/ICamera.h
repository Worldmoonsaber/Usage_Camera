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

	virtual void Grab(void*& imgPtr);


	virtual void SetCameraParam(string NodeName, string Value);
	virtual void GetCameraParam(string NodeName, string& Value);

	virtual void AcquisitionStart();
	virtual void AcquisitionStop();

	string CameraName();
	//-----ヘ玡临⊿Τ稱Τ诀 盢Log癟┻ SubVi 硂场だ痙ゼㄓэ秈 既ぃ秈︽砏购
	void Save();
	void Load();
protected:
	string _strName; //膥┯砆

private:
	int _Width=5320;//代刚ノ计
	int _Height=4600;//代刚ノ计
	int _Channels=4;//代刚ノ计
};