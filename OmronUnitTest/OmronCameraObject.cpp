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
		_isCameraOn = false;
		_isDispose = false;
		//queue<string> qCommand;
		//queue<string> qAck_ReturnValue;

		//	static void thread_WorkingLoop(IStDeviceReleasable** obj, int indx, bool* _isCameraOn, bool* _isDispose, queue<string>* qCommand, queue<string>* qAck);// int* indx, IStDeviceReleasable** obj, queue<string>* qCommand, queue<string>* qAck, bool* _isCameraOn, bool* _isDispose);
		thread_Work = thread(OmronCameraObject::thread_WorkingLoop, &pIStDeviceReleasable,indx,&_isCameraOn,&_isDispose,&qCommand,&qAck_ReturnValue,&mu);// , & pIStDeviceReleasable, & qCommand, & qAck_ReturnValue, & _isCameraOn, & _isDispose);

		//_StreamPtr = pIStDevice->CreateIStDataStream(indx);
		//CNodeMapPtr pINodeMapRemote = pIStDevice->GetRemoteIStPort()->GetINodeMap();

		//SetEnumeration(pINodeMapRemote, USER_SET_SELECTOR, USER_SET_TARGET);
		//Execute(pINodeMapRemote, USER_SET_LOAD);

		//CIStFeatureBagPtr pIStFeatureBagPtr = CreateIStFeatureBag();

		//// Save the current settings to FeatureBag.
		//pIStFeatureBagPtr->StoreNodeMapToBag(pINodeMapRemote);

		//string strVal;
		//GetEnumeration(pIStDeviceReleasable->GetRemoteIStPort()->GetINodeMap(), "PixelFormat", strVal);
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
	//mu.lock();
	////qCommand.push("SetCameraParam:" + NodeName+":" + Value);
	//mu.unlock();

	/*
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
	*/


}

void OmronCameraObject::GetCameraParam(string NodeName, string& Value)
{
	Value = "";
	//mu.lock();
	////qCommand.push("GetCameraParam:" + NodeName);
	//mu.unlock();
	//------等待 qAck_ReturnValue 取得資料
	//while(true)

	/*
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
	*/

}

void OmronCameraObject::Excute(string ExcuteCmd)
{
	//qCommand.push("Excute:"+ ExcuteCmd);
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

void OmronCameraObject::thread_WorkingLoop(IStDeviceReleasable** obj, int indx, bool* _isCameraOn, bool* _isDispose, queue<string>* qCommand, queue<string>* qAck, mutex* mutex)// int* indx, IStDeviceReleasable** obj, queue<string>* qCommand, queue<string>* qAck, bool* _isCameraOn, bool* _isDispose)
{
	try
	{

		IStDeviceReleasable* _obj = obj[0];
		CIStDevicePtr pIStDevice(_obj);
		CIStDataStreamPtr pStreamPtr = pIStDevice->CreateIStDataStream(indx);
		CNodeMapPtr pINodeMapRemote = pIStDevice->GetRemoteIStPort()->GetINodeMap();

		while (true)
		{

			//if (qCommand->size() != 0)
			//{
			//	string obj;

			//	mutex->lock();
			//	obj = qCommand->front();//取得指令
			//	qCommand->pop();//將資料從_QueueObj中清除		
			//	mutex->unlock();

			//	//---進行工作
			//	cout << "obj :" << obj << endl;


			//}





			if (_isDispose)
				break;
		}


		//	if (_isDispose[0])
		//		true;
		//}


	}
	catch (const GenICam::GenericException& e)
	{
		cerr << endl << "An exception occurred." << endl << e.GetDescription() << endl;
	}
}

void OmronCameraObject::Execute(INodeMap* pINodeMap, const char* szCommandName)
{
	CCommandPtr pICommand(pINodeMap->GetNode(szCommandName));
	pICommand->Execute();
}

void OmronCameraObject::SetEnumeration(INodeMap* pINodeMap, const char* szEnumerationName, const char* szValueName)
{
	try
	{
		CIStFeatureBagPtr pIStFeatureBagPtr = CreateIStFeatureBag();
		pIStFeatureBagPtr->StoreNodeMapToBag(pINodeMap);
		CEnumerationPtr pIEnumeration(pINodeMap->GetNode(szEnumerationName));
		GenApi::NodeList_t sNodeList;
		pIEnumeration->GetEntries(sNodeList);

		for (int i = 0; i < sNodeList.size(); i++)
			if (sNodeList[i]->GetDisplayName() == szValueName)
			{
				CEnumEntryPtr pIEnumEntry(sNodeList[i]);
				int64_t val = pIEnumEntry->GetValue();
				cout << "pIEnumEntry->GetValue() :" << val << endl;
				pIEnumeration->SetIntValue(pIEnumEntry->GetValue(), false);
				break;
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
		int64_t nValue = pIInteger->GetValue();
	}
	catch (const GenICam::GenericException& e)
	{
		cerr << endl << "An exception occurred." << endl << e.GetDescription() << endl;
	}

}
