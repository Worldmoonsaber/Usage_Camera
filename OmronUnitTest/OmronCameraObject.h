#pragma once
#include "ICamera.h"


#include <StApi_TL.h>
#ifdef ENABLED_ST_GUI
#include <StApi_GUI.h>
#include <iomanip>	//std::setprecision
#endif

//Namespace for using StApi.
using namespace StApi;

//Namespace for using cout
using namespace std;



class OmronCameraObject : public ICamera
{

public:
	OmronCameraObject();

	//OmronCameraObject(CIStDevicePtr device);
	
	OmronCameraObject(IStDeviceReleasable* obj, int indx);

	
	//~OmronCameraObject() = delete;;

	void Initialize();
	void Close();

	void Grab_Int(unsigned int*& imgPtr);
	void Grab(void*& imgPtr);

	void SetCameraParam(string NodeName, string Value);
	void GetCameraParam(string NodeName, string& Value);

	void Excute(string ExcuteCmd);

	void AcquisitionStart();
	void AcquisitionStop();

	void Save();
	void Load();

private:
	IStDeviceReleasable* pIStDeviceReleasable;
	bool _IsStreamStart = false;
	bool _isInitialized = false;
	CIStDevicePtr _device;
	CIStDataStreamPtr _StreamPtr;

};


static void InitializeOmron(vector<ICamera*>& vCamera)
{

	// Initialize StApi before using.
	CStApiAutoInit objStApiAutoInit;

	// Create a system object for device scan and connection.
	CIStSystemPtr pIStSystem(CreateIStSystem());

	vector< IStDeviceReleasable*> vDevice;
	vector< OmronCameraObject*> vOmron;

	int indx = 0;

	for (;;)
	{
		IStDeviceReleasable* pIStDeviceReleasable = NULL;

		try
		{
			pIStDeviceReleasable = pIStSystem->CreateFirstIStDevice();
		}
		catch (...)
		{
		}
		
		OmronCameraObject* obj = new OmronCameraObject(pIStDeviceReleasable, indx);

		bool existSame = false;

		for (int i = 0; i < vOmron.size(); i++)
		{
			string str0 = obj->CameraName();
			string str1 = vOmron[i]->CameraName();

			if (str0 == str1)
			{
				existSame = true;
				break;
			}

		}

		if (!existSame)
			vOmron.push_back(obj);
		else
			break;

		indx++;
	}

	for (int i = 0; i < vOmron.size(); i++)
		vCamera.push_back((ICamera*)vOmron[i]);

	

	vOmron.clear();

	cout << "vCamera Count=" << vCamera.size() << endl;

}