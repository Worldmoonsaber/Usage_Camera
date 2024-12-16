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



class OmronCameraObject :
    public ICamera
{

public:
	OmronCameraObject();

	OmronCameraObject(CIStDevicePtr device);
	
	~OmronCameraObject();

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
	CIStDataStreamPtr pIStDataStream;
	bool _IsStreamStart = false;
	bool _isInitialized = false;
	CIStDevicePtr _device;


};


static void InitializeOmron(vector<ICamera*>& vCamera)
{
	// Initialize StApi before using.
	CStApiAutoInit objStApiAutoInit;

	CIStSystemPtrArray pIStSystemList;
	for (EStSystemVendor_t eSystemVendor = StSystemVendor_Default; eSystemVendor < StSystemVendor_Count; eSystemVendor = (EStSystemVendor_t)(eSystemVendor + 1))
	{
		try
		{
			CIStSystemPtr pIStSystem(CreateIStSystem(eSystemVendor, StInterfaceType_All));

			// Create a camera device object and connect to first detected device.
			CIStDevicePtr pIStDevice(pIStSystem->CreateFirstIStDevice());


			//----¥[¤JÀÉ®×

		}
		catch (const GenICam::GenericException& e)
		{
			// Display a description of error if any happens.
			cerr << "An exception occurred." << endl << e.GetDescription() << endl;
		}
	}
}