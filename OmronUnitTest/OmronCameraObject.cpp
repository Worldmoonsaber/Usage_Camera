#include "OmronCameraObject.h"
#include <StApi_TLFunctions.h>
#include <StApi_GUIDef.h>
#include <future>
using namespace StApi;

void WorkingLoop_AcqStart(CIStDevicePtr& dev, CIStDataStreamPtr& stream,bool& isCameraOn)
{
	if (!isCameraOn)
	{
		stream->StartAcquisition();
		dev->AcquisitionStart();
		isCameraOn = true;
	}
}

void WorkingLoop_AcqStart(CIStDevicePtrArray& dev, CIStDataStreamPtrArray& stream, bool& isCameraOn)
{
	if (!isCameraOn)
	{
		stream.StartAcquisition();
		dev.AcquisitionStart();
		isCameraOn = true;
	}
}


void WorkingLoop_AcqStop(CIStDevicePtr& dev, CIStDataStreamPtr& stream,bool& isCameraOn)
{
	if (isCameraOn)
	{
		dev->AcquisitionStop();
		stream->StopAcquisition();
		isCameraOn = false;
	}
}


void thread_WorkingLoop_Test(IStDeviceReleasable*& Obj, queue<string>*& qCommand,mutex*& mu)
{
	try
	{
		bool isCameraOn = false;
		CIStDevicePtrArray pIStDeviceList;
		CIStDataStreamPtrArray pIStDataStreamList;

		pIStDeviceList.Register(Obj);
		pIStDataStreamList.Register(Obj->CreateIStDataStream(0));
		CNodeMapPtr pINodeMapRemote(pIStDeviceList[0]->GetRemoteIStPort()->GetINodeMap());

		WorkingLoop_AcqStart(pIStDeviceList, pIStDataStreamList, isCameraOn);

		while (true)
		{
			//if (qCommand.size() != 0)
			//{
			//	mu.lock();
			//	string strObj = qCommand.front();//取得資料
			//	qCommand.pop();//將資料從_QueueObj中清除		
			//	mu.unlock();
			//	std::cout << "Command  " << strObj << endl;

			//}



		}


		//WorkingLoop_AcqStart(pIStDeviceList, pIStDataStreamList,isCameraOn);

		//while (pIStDataStreamList.IsGrabbingAny())
		//{
		//	// Retrieve data buffer pointer of image data from any camera with a timeout of 5000ms.
		//	CIStStreamBufferPtr pIStStreamBuffer(pIStDataStreamList.RetrieveBuffer(5000));

		//	// Check if the acquired data contains image data.
		//	if (pIStStreamBuffer->GetIStStreamBufferInfo()->IsImagePresent())
		//	{
		//		cout
		//			<< pIStStreamBuffer->GetIStDataStream()->GetIStDevice()->GetIStDeviceInfo()->GetDisplayName()
		//			<< " : BlockId=" << dec << pIStStreamBuffer->GetIStStreamBufferInfo()->GetFrameID()
		//			<< " " << setprecision(4) << pIStStreamBuffer->GetIStDataStream()->GetCurrentFPS() << "FPS" << endl;
		//	}
		//	else
		//	{
		//		// If the acquired data contains no image data.
		//		cout << "Image data does not exist" << endl;
		//	}
		//}

		//WorkingLoop_AcqStop(pIStDeviceList, pIStDataStreamList, isCameraOn);
	}
	catch (const GenICam::GenericException& e)
	{
		cerr << endl << "An exception occurred." << endl << e.GetDescription() << endl;
	}

}

OmronCameraObject::OmronCameraObject()
{
}

OmronCameraObject::OmronCameraObject(IStDeviceReleasable* obj)
{

	_device = obj;
	_strName = _device->GetIStDeviceInfo()->GetDisplayName();
	std::cout << "strName " << _strName << endl;
	//thread_Work = thread(&OmronCameraObject::thread_WorkingLoop, obj,&qCommand,&mu);
	_pIStDevice.Reset(_device);
	pIStDataStream.Reset(_pIStDevice->CreateIStDataStream(0));

	//pIStDataStream->StartAcquisition();
	//_pIStDevice->AcquisitionStart();

	//while (pIStDataStream->IsGrabbing())
	//{
	//// Retrieve data buffer pointer of image data from any camera with a timeout of 5000ms.
	//	CIStStreamBufferPtr pIStStreamBuffer(pIStDataStream->RetrieveBuffer(5000));

	//// Check if the acquired data contains image data.
	//	if (pIStStreamBuffer->GetIStStreamBufferInfo()->IsImagePresent())
	//	{
	//		cout
	//		<< pIStStreamBuffer->GetIStDataStream()->GetIStDevice()->GetIStDeviceInfo()->GetDisplayName()
	//		<< " : BlockId=" << dec << pIStStreamBuffer->GetIStStreamBufferInfo()->GetFrameID()
	//		<< " " << setprecision(4) << pIStStreamBuffer->GetIStDataStream()->GetCurrentFPS() << "FPS" << endl;
	//	}
	//	else
	//	{
	//		// If the acquired data contains no image data.
	//		cout << "Image data does not exist" << endl;
	//	}
	//}
 
	//CIStDevicePtrArray pIStDeviceList;
	//CIStDataStreamPtrArray pIStDataStreamList;

	//pIStDeviceList.Register(obj);
	//pIStDataStreamList.Register(obj->CreateIStDataStream(0));
	//CNodeMapPtr pINodeMapRemote(pIStDeviceList[0]->GetRemoteIStPort()->GetINodeMap());

	//thread_Work = thread(&OmronCameraObject::thread_WorkingLoop_test1, pIStDeviceList, pIStDataStreamList, pINodeMapRemote);

	//-----不能使用 async 程式會卡在這個函式裡面

	//_pIStDeviceList.Register(obj);

	//// Create a DataStream object for handling image stream data then add into DataStream list for later usage.
	//_pIStDataStreamList.Register(obj->CreateIStDataStream(0));
	//thread_Work = thread(thread_WorkingLoop_Test);

	//std::future<void> Thread_Working_Loop = std::async(std::launch:async, thread_WorkingLoop_Test, std::ref(obj),std::ref(qCommand),std::ref(mu));
	//cout << "Thread_Working_Loop Start \n";
//std:thread Thread_Working_Loop = std::thread(thread_WorkingLoop_Test, std::ref(obj), std::ref(qCommand), std::ref(mu));

	//result.get();
	//_pIStDataStreamList.StartAcquisition();
	//_pIStDeviceList.AcquisitionStart();
	//pIStDeviceReleasable = obj;
	//CIStDevicePtr pIStDevice(pIStDeviceReleasable);
	//_strName = pIStDeviceReleasable->GetIStDeviceInfo()->GetDisplayName();
	//uint32_t ps = pIStDeviceReleasable->GetDataStreamCount();

	//IStDeviceReleasable* device = (IStDeviceReleasable*)_pIStDeviceList[0];

	//	static void thread_WorkingLoop(CIStDevicePtr* obj, CIStDataStreamPtrArray* Stream , int indx, bool* _isCameraOn, bool* _isDispose, queue<string>* qCommand, queue<string>* qAck, mutex* mutex);// int* indx, IStDeviceReleasable** obj, queue<string>* qCommand, queue<string>* qAck, bool* _isCameraOn, bool* _isDispose);
	//thread_Work = thread(OmronCameraObject::thread_WorkingLoop, &_pIStDeviceList);

	//if (indx < ps)
	//{
	//	_isCameraOn = false;
	//	_isDoDispose = false;
	//	//queue<string> qCommand;
	//	//queue<string> qAck_ReturnValue;

	//	//	static void thread_WorkingLoop(IStDeviceReleasable** obj, int indx, bool* _isCameraOn, bool* _isDispose, queue<string>* qCommand, queue<string>* qAck);// int* indx, IStDeviceReleasable** obj, queue<string>* qCommand, queue<string>* qAck, bool* _isCameraOn, bool* _isDispose);
	//	//thread_Work = thread(OmronCameraObject::thread_WorkingLoop, &pIStDeviceReleasable,indx,&_isCameraOn,&_isDoDispose,&qCommand,&qAck_ReturnValue,&mu);// , & pIStDeviceReleasable, & qCommand, & qAck_ReturnValue, & _isCameraOn, & _isDispose);

	//	//_StreamPtr = pIStDevice->CreateIStDataStream(indx);
	//	//CNodeMapPtr pINodeMapRemote = pIStDevice->GetRemoteIStPort()->GetINodeMap();

	//	//SetEnumeration(pINodeMapRemote, USER_SET_SELECTOR, USER_SET_TARGET);
	//	//Execute(pINodeMapRemote, USER_SET_LOAD);

	//	//CIStFeatureBagPtr pIStFeatureBagPtr = CreateIStFeatureBag();

	//	//// Save the current settings to FeatureBag.
	//	//pIStFeatureBagPtr->StoreNodeMapToBag(pINodeMapRemote);

	//	//string strVal;
	//	//GetEnumeration(pIStDeviceReleasable->GetRemoteIStPort()->GetINodeMap(), "PixelFormat", strVal);
	//}
	//else
	//{
	//	_strName = "";
	//	//pIStDevice.~CStAutoPtr();
	//}
}

void OmronCameraObject::Initialize()
{
}

void OmronCameraObject::Close()
{
	_isDoDispose = true;


}

void OmronCameraObject::Grab_Int(unsigned int*& imgPtr)
{
}

void OmronCameraObject::Grab(void*& imgPtr)
{
}

void OmronCameraObject::SetCameraParam(string NodeName, string Value)
{
	//AddToTheQueue("SetCameraParam:" + NodeName + ":" + Value);

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
	//AddToTheQueue("GetCameraParam:" + NodeName);
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
	try{
	//CIStDevicePtr _pIStDevice;
	//CIStDataStreamPtr pIStDataStream;// (pIStDevice->CreateIStDataStream(0));
	pIStDataStream->StartAcquisition();
	_pIStDevice->AcquisitionStart();

	//AddToTheQueue("ACQ:Start");
	//_pIStDevice.Reset(_device);
	//pIStDataStream.Reset(_pIStDevice->CreateIStDataStream(0));

	}
	catch (const GenICam::GenericException& e)
	{
		cerr << endl << "An exception occurred." << endl << e.GetDescription() << endl;
	}
}

void OmronCameraObject::AcquisitionStop()
{
	_pIStDevice->AcquisitionStop();
	pIStDataStream->StopAcquisition();
	//AddToTheQueue("ACQ:Stop");
	//_pIStDevice.Reset(_device);

}

void OmronCameraObject::Save()
{
}

void OmronCameraObject::Load()
{
}

void OmronCameraObject::AddToTheQueue(string str)
{
	mu.lock();
	qCommand.push(str);
	mu.unlock();
}

void OmronCameraObject::thread_WorkingLoop(IStDeviceReleasable* obj, queue <string>* queue, mutex* mutex)
{
	try
	{
		bool isCameraOn = false;

		CIStDevicePtr pIStDevice;
		CIStDataStreamPtr pIStDataStream;
		CIStRegisteredCallbackPtr pIStRegisteredCallbackDeviceLost;



		pIStDevice.Reset(obj);
		pIStDataStream.Reset(pIStDevice->CreateIStDataStream(0));
		pIStRegisteredCallbackDeviceLost.Reset(NULL);


		CNodeMapPtr pINodeMapRemote(pIStDevice->GetRemoteIStPort()->GetINodeMap());

		std::cout << "pIStDevice->GetIStDeviceInfo()->GetDisplayName() " << pIStDevice->GetIStDeviceInfo()->GetDisplayName() << endl;

		pIStDataStream->StartAcquisition();
		pIStDevice->AcquisitionStart();

		while (true)
		{
			if (queue->size() != 0)
			{
				mutex->lock();
				string strObj = queue->front();//取得資料
				queue->pop();//將資料從_QueueObj中清除		
				mutex->unlock();
				std::cout << "Command  " << strObj << endl;

			}

			//std::cout << "queue->size() "<< queue->size() << endl;


		}


		//WorkingLoop_AcqStart(pIStDeviceList, pIStDataStreamList,isCameraOn);

		//while (pIStDataStreamList.IsGrabbingAny())
		//{
		//	// Retrieve data buffer pointer of image data from any camera with a timeout of 5000ms.
		//	CIStStreamBufferPtr pIStStreamBuffer(pIStDataStreamList.RetrieveBuffer(5000));

		//	// Check if the acquired data contains image data.
		//	if (pIStStreamBuffer->GetIStStreamBufferInfo()->IsImagePresent())
		//	{
		//		cout
		//			<< pIStStreamBuffer->GetIStDataStream()->GetIStDevice()->GetIStDeviceInfo()->GetDisplayName()
		//			<< " : BlockId=" << dec << pIStStreamBuffer->GetIStStreamBufferInfo()->GetFrameID()
		//			<< " " << setprecision(4) << pIStStreamBuffer->GetIStDataStream()->GetCurrentFPS() << "FPS" << endl;
		//	}
		//	else
		//	{
		//		// If the acquired data contains no image data.
		//		cout << "Image data does not exist" << endl;
		//	}
		//}

		//WorkingLoop_AcqStop(pIStDeviceList, pIStDataStreamList, isCameraOn);
	}
	catch (const GenICam::GenericException& e)
	{
		cerr << endl << "An exception occurred." << endl << e.GetDescription() << endl;
	}
}

void OmronCameraObject::thread_WorkingLoop_test1(CIStDevicePtrArray pIStDeviceList, CIStDataStreamPtrArray pIStDataStreamList, CNodeMapPtr pINodeMapRemote)
{

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
