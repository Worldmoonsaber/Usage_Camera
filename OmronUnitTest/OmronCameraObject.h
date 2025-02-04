#pragma once
#include "ICamera.h"
#include <queue>

#include <StApi_TL.h>
#include <StApi_GUI.h>
#include <iomanip>	//std::setprecision
//#include <StApi_TLFunctions.h>
//#include <StApi_GUIDef.h>
//#include <future>

using namespace StApi;
using namespace std;
using namespace GenApi;

enum StateMechine {
	idle,    // 預設狀態
	waitGrabImage,  // 可以取像的狀態
	acquistionStart,// 呼叫 acquistionStart 呼叫完成後 改狀態為 waitGrabImage
	acquistionStop  // 呼叫 acquistionStop 呼叫完成後 改狀態為 idle
};

class OmronCameraObject : public ICamera
{

public:
	OmronCameraObject();
	
	OmronCameraObject(IStDeviceReleasable* obj);


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
	void AddToTheQueue(string str);

	//std::future<void> Thread_Working_Loop;
	bool _isCameraOn;
	bool _isDoDispose;
	/// <summary>
	/// 
	/// </summary>
	/// <param name="qCommand">指令</param>
	/// <param name="qAack">取得數值</param>
	/// <param name="_isDispose"></param>
	static void thread_WorkingLoop(IStDeviceReleasable* obj, queue <string>* queue, mutex* mutex);// int* indx, IStDeviceReleasable** obj, queue<string>* qCommand, queue<string>* qAck, bool* _isCameraOn, bool* _isDispose);

	static void thread_WorkingLoop_test1(CIStDevicePtrArray pIStDeviceList, CIStDataStreamPtrArray pIStDataStreamList, CNodeMapPtr pINodeMapRemote);// int* indx, IStDeviceReleasable** obj, queue<string>* qCommand, queue<string>* qAck, bool* _isCameraOn, bool* _isDispose);


	mutex mu;
	thread thread_Work;

	queue<string> qCommand;
	queue<string> qAck_ReturnValue;

	bool _IsStreamStart = false;
	bool _isInitialized = false;
	//CNodeMapPtr pINodeMapRemote;

	static void Execute(INodeMap* pINodeMap, const char* szCommandName);
	static void SetEnumeration(INodeMap* pINodeMap, const char* szEnumerationName, const char* szValueName);
	static void GetEnumeration(INodeMap* pINodeMap, const char* szEnumerationName, string& strVal);
	static void GetInteger(INodeMap* pINodeMap, const char* szEnumerationName, string& strVal);

	StateMechine _status;

	CIStDevicePtr _pIStDevice;
	CIStDataStreamPtr pIStDataStream;// (pIStDevice->CreateIStDataStream(0));
	CNodeMapPtr pINodeMapRemote;

	CIStRegisteredCallbackPtr _pIStRegisteredCallbackDeviceLost;
	IStDeviceReleasable* _device;
};


static void InitializeOmron(vector<ICamera*>& vCamera)
{

	// Initialize StApi before using.
	CStApiAutoInit objStApiAutoInit;

	// Create a system object for device scan and connection.
	CIStSystemPtr pIStSystem(CreateIStSystem());


	int count = pIStSystem->GetInterfaceCount();
	std::cout << "pIStSystem->GetInterfaceCount() " << pIStSystem->GetInterfaceCount() << endl;

	for (size_t j = 0; j < count; ++j)
	{
		IStInterface* pIStInterface = pIStSystem->GetIStInterface(j);
		pIStInterface->StopEventAcquisitionThread();

	}

	vector< IStDeviceReleasable*> vDevice;

	int indx = 0;

	try
	{
		//for (size_t iInterface = 0; iInterface < count; ++iInterface)
		//{
		//	// Get the IStInterface interface pointer.
		//	IStInterface* pIStInterface(pIStSystem->GetIStInterface(iInterface));
		//	pIStInterface->StopEventAcquisitionThread();

		//	try
		//	{

		//		CIStDevicePtr pIStDevice;

		//		// When the camera of the specified ID exists, get the IStDeviceRelesable interface pointer
		//		pIStDevice.Reset(pIStInterface->CreateIStDevice(strDeviceId));
		//		break;
		//	}
		//	catch (...)
		//	{
		//	}

		//}



		for (;;)
		{
			IStDeviceReleasable* pIStDeviceReleasable = NULL;

			try
			{
				// Create a camera device object and connect to first detected device.
				pIStDeviceReleasable = pIStSystem->CreateFirstIStDevice();
			}
			catch (...)
			{
				//---無法取得 相機
				break;
			}

			bool existSame = false;

			for (int i = 0; i < vDevice.size(); i++)
			{
				string str0 = vDevice[i]->GetIStDeviceInfo()->GetDisplayName();
				string str1 = pIStDeviceReleasable->GetIStDeviceInfo()->GetDisplayName();

				if (str0 == str1 || str0 == "")
				{
					existSame = true;
					break;
				}

			}
			

			if (!existSame)
			{
				vDevice.push_back(pIStDeviceReleasable);

				//CIStDevicePtr pIStDevice;
				//pIStDevice.Reset(pIStDeviceReleasable);
			}
			else
				break;

		}


		for (int j = 0; j < vDevice.size(); j++)
		{
			OmronCameraObject* obj = new OmronCameraObject(vDevice[j]);
			vCamera.push_back((ICamera*)obj);
		}

	}
	catch (const GenICam::GenericException& e)
	{
		// Display the description of the error of exception.
		cerr << endl << "An exception occurred." << endl << e.GetDescription() << endl;
	}


	//for (int i = 0; i < vOmron.size(); i++)
	//	vCamera.push_back((ICamera*)vOmron[i]);

	//

	//vOmron.clear();

	cout << "vCamera Count=" << vCamera.size() << endl;

}