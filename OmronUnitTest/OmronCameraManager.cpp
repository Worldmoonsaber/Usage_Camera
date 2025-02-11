
// StViewer.cpp : Defines the class behaviors for the application.
//

#include "OmronCameraManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace StApi;
using namespace std;

//-----------------------------------------------------------------------------
OmronCameraManager::OmronCameraManager()
try :
	// Before using the library, perform the initialization. 
	m_objStApiAutoInit()
{
	InitInstance();

	for (size_t i = 0; i < m_objIStSystemPtrList.GetSize(); ++i)
	{
		IStSystem* pIStSystem = m_objIStSystemPtrList[i];
		for (size_t j = 0; j < pIStSystem->GetInterfaceCount(); ++j)
		{
			IStInterface* pIStInterface = pIStSystem->GetIStInterface(j);
			IStDeviceReleasable* pDevice = pIStInterface->CreateFirstIStDevice();

			if (pDevice->GetIStInterface())
			{
				OmronCameraObject* obj = new OmronCameraObject(pDevice);
				vOmronCamera.push_back(obj);
			}
			break;
		}
	}

}
catch (const GenICam::GenericException& e)
{
	std::cout << "OmronCameraManager::GenericException : "<<e.what() << endl;
}
catch (const GenICam_3_2_Sentech::RuntimeException& e)
{
	std::cout << "OmronCameraManager::RuntimeException : " << e.what() << endl;
}

OmronCameraManager theOmronCameraList;

bool OmronCameraManager::InitInstance()
{

	const uint32_t nCount = StSystemVendor_Count;
	for (uint32_t i = StSystemVendor_Default; i < nCount; ++i)
	{
		EStSystemVendor_t eStSystemVendor = (EStSystemVendor_t)i;
		try
		{
			m_objIStSystemPtrList.Register(CreateIStSystem(eStSystemVendor, StInterfaceType_All));
		}
		catch (const GenICam::GenericException& e)
		{
			if (eStSystemVendor == StSystemVendor_Default)
			{
				std::cout << "InitInstance::Exception : " << e.what() << endl;
			}
		}
	}

	return TRUE;
}


