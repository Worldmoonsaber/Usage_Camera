#include "OmronCameraObject.h"
#include <StApi_TLFunctions.h>
#include <StApi_GUIDef.h>
using namespace StApi;


OmronCameraObject::OmronCameraObject()
{
}

OmronCameraObject::OmronCameraObject(IStDeviceReleasable* obj, int indx)
{
	pIStDeviceReleasable = obj;
	//CIStDevicePtr pIStDevice(pIStDeviceReleasable);
	_strName = pIStDeviceReleasable->GetIStDeviceInfo()->GetDisplayName();
	uint32_t ps = pIStDeviceReleasable->GetDataStreamCount();


	if (indx < ps)
	{

		//_StreamPtr = pIStDevice->CreateIStDataStream(indx);
		//CNodeMapPtr pINodeMapRemote = pIStDevice->GetRemoteIStPort()->GetINodeMap();

		//SetEnumeration(pINodeMapRemote, USER_SET_SELECTOR, USER_SET_TARGET);
		//Execute(pINodeMapRemote, USER_SET_LOAD);

		//CIStFeatureBagPtr pIStFeatureBagPtr = CreateIStFeatureBag();

		//// Save the current settings to FeatureBag.
		//pIStFeatureBagPtr->StoreNodeMapToBag(pINodeMapRemote);

		string strVal;
		GetEnumeration(pIStDeviceReleasable->GetRemoteIStPort()->GetINodeMap(), "PixelFormat", strVal);
	}
	else
	{
		_strName = "";
		//pIStDevice.~CStAutoPtr();
	}
}

void OmronCameraObject::Initialize()
{
}

void OmronCameraObject::Close()
{
}

void OmronCameraObject::Grab_Int(unsigned int*& imgPtr)
{
}

void OmronCameraObject::Grab(void*& imgPtr)
{
}

void OmronCameraObject::SetCameraParam(string NodeName, string Value)
{
	if (NodeName == "PixelFormat")
	{
		SetEnumeration(pIStDeviceReleasable->GetRemoteIStPort()->GetINodeMap(), "PixelFormat", Value.c_str());
	}
	else if (NodeName == "ExposureTime")
	{

	}
	else if (NodeName == "Width")
	{

	}
	else if (NodeName == "Height")
	{

	}


}

void OmronCameraObject::GetCameraParam(string NodeName, string& Value)
{
	Value = "";

	if (NodeName == "PixelFormat")
	{
		GetEnumeration(pIStDeviceReleasable->GetRemoteIStPort()->GetINodeMap(), "PixelFormat", Value);

	}
	else if (NodeName == "ExposureTime")
	{

		GetEnumeration(pIStDeviceReleasable->GetRemoteIStPort()->GetINodeMap(), "ExposureTime", Value);

	}
	else if (NodeName == "Width")
	{
		//pIStDeviceReleasable->GetRemoteIStPort()->GetINodeMap()->GetNode("Width")-

		//GetEnumeration(pIStDeviceReleasable->GetRemoteIStPort()->GetINodeMap(), "Width", Value);
		GetInteger(pIStDeviceReleasable->GetRemoteIStPort()->GetINodeMap(), "Width", Value);
	}
	else if (NodeName == "Height")
	{
		//GetEnumeration(pIStDeviceReleasable->GetRemoteIStPort()->GetINodeMap(), "Height", Value);
		GetInteger(pIStDeviceReleasable->GetRemoteIStPort()->GetINodeMap(), "Height", Value);
	}

}

void OmronCameraObject::Excute(string ExcuteCmd)
{
}

void OmronCameraObject::AcquisitionStart()
{
	//pIStDeviceReleasable->AcquisitionStart();
}

void OmronCameraObject::AcquisitionStop()
{
	//pIStDeviceReleasable->AcquisitionStop();
}

void OmronCameraObject::Save()
{
}

void OmronCameraObject::Load()
{
}

void OmronCameraObject::Execute(INodeMap* pINodeMap, const char* szCommandName)
{
	CCommandPtr pICommand(pINodeMap->GetNode(szCommandName));
	pICommand->Execute();
}

void OmronCameraObject::SetEnumeration(INodeMap* pINodeMap, const char* szEnumerationName, const char* szValueName)
{
	try {


		CIStFeatureBagPtr pIStFeatureBagPtr = CreateIStFeatureBag();
		// Save the current settings to FeatureBag.
		pIStFeatureBagPtr->StoreNodeMapToBag(pINodeMap);
		CEnumerationPtr pIEnumeration(pINodeMap->GetNode(szEnumerationName));

		GenApi::NodeList_t sNodeList;
		pIEnumeration->GetEntries(sNodeList);

		//pIEnumeration->SetIntValue(pIEnumEntry->GetValue());

		for (int i = 0; i < sNodeList.size(); i++)
		{
			if (sNodeList[i]->GetDisplayName() == szValueName)
			{
				CEnumEntryPtr pIEnumEntry(sNodeList[i]);
				int64_t val = pIEnumEntry->GetValue();
				cout << "pIEnumEntry->GetValue() :" << val << endl;
				pIEnumeration->SetIntValue(pIEnumEntry->GetValue(),false);
				break;
			}

		}

	}
	catch (const GenICam::GenericException& e)
	{
		cerr << endl << "An exception occurred." << endl << e.GetDescription() << endl;
	}


}

void OmronCameraObject::GetEnumeration(INodeMap* pINodeMap, const char* szEnumerationName, string& strVal)
{
	try
	{
		//GenApi::CNodePtr pINode(pINodeMap->GetNode(szEnumerationName));

		//pINode->

		CEnumerationPtr pIEnumeration(pINodeMap->GetNode(szEnumerationName));
		strVal = pIEnumeration->GetCurrentEntry()->GetSymbolic();
	}
	catch (const GenICam::GenericException& e)
	{
		cerr << endl << "An exception occurred." << endl << e.GetDescription() << endl;
	}
}

void OmronCameraObject::GetInteger(INodeMap* pINodeMap, const char* szEnumerationName, string& strVal)
{
	try
	{

	GenApi::CEnumerationPtr pRegionSelector(pINodeMap->GetNode("RegionSelector"));
	GenApi::CEnumEntryPtr pRegion0 = pRegionSelector->GetEntryByName("Region0");
	int64_t iRegion0Value = pRegion0->GetValue();
	pRegionSelector->SetIntValue(iRegion0Value);

	GenApi::CNodePtr pINode(pINodeMap->GetNode(szEnumerationName));

	GenApi::CIntegerPtr pIInteger(pINode);

	cout << GetInterfaceName(pINode) << " : " << pINode->GetName() << endl;


	cout << "pIInteger->GetUnit()  : " << pIInteger->GetUnit() << endl;


	int64_t nValue = pIInteger->GetValue();
	}
	catch (const GenICam::GenericException& e)
	{
		cerr << endl << "An exception occurred." << endl << e.GetDescription() << endl;
	}



}
