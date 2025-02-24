#pragma once
#include <StApi_TL.h>
#include "OmronCameraObject.h"

class OmronCameraManager 
{
public:
	OmronCameraManager();

	// Overrides
public:
	bool InitInstance();

public:

protected:
	StApi::CIStSystemPtrArray m_objIStSystemPtrList;

	StApi::CStApiAutoInit	m_objStApiAutoInit;

public:
	vector< OmronCameraObject*> vOmronCamera;

};

extern OmronCameraManager theOmronCameraList;


namespace CameraManager
{
	extern "C" __declspec(dllexport)  vector< ICamera*> GetCamera();
	extern "C" __declspec(dllexport)  void CloseCamera();
	void WriteLog(const std::string& message);
}