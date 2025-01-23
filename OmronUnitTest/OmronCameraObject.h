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

//Namespace for using GenApi.
using namespace GenApi;

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
	static CIStDataStreamPtr _StreamPtr;
	//CNodeMapPtr pINodeMapRemote;

	void Execute(INodeMap* pINodeMap, const char* szCommandName);
	//-----------------------------------------------------------------------------
	// Set the setting of indicated enumeration of the node map.
	//-----------------------------------------------------------------------------
	void SetEnumeration(INodeMap* pINodeMap, const char* szEnumerationName, const char* szValueName);


	void GetEnumeration(INodeMap* pINodeMap, const char* szEnumerationName, string& strVal);
	void GetInteger(INodeMap* pINodeMap, const char* szEnumerationName, string& strVal);

	const char* USER_SET_SELECTOR = "UserSetSelector";						//Standard
	const char* USER_SET_TARGET = "UserSet1";								//Standard
	const char* USER_SET_LOAD = "UserSetLoad";								//Standard
	const char* USER_SET_SAVE = "UserSetSave";								//Standard
	const char* USER_SET_DEFAULT = "UserSetDefault";						//Standard
	const char* USER_SET_DEFAULT_SELECTOR = "UserSetDefaultSelector";		//Standard(Deprecated)
	const char* PIXEL_FORMAT = "PixelFormat";								//Standard

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

	try
	{
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

				if (str0 == str1 || str0 == "")
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
			break;
		}
	}
	catch (const GenICam::GenericException& e)
	{
		// Display the description of the error of exception.
		cerr << endl << "An exception occurred." << endl << e.GetDescription() << endl;
	}


	for (int i = 0; i < vOmron.size(); i++)
		vCamera.push_back((ICamera*)vOmron[i]);

	

	vOmron.clear();

	cout << "vCamera Count=" << vCamera.size() << endl;

}