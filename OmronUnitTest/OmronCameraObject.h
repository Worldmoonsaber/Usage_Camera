#pragma once
#include "ICamera.h"
#include <queue>


#include <StApi_TL.h>
#ifdef ENABLED_ST_GUI
#include <StApi_GUI.h>
#include <iomanip>	//std::setprecision
#endif

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
	
	OmronCameraObject(IStDeviceReleasable* obj, int indx);


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

	bool _isCameraOn;
	bool _isDispose;
	/// <summary>
	/// 
	/// </summary>
	/// <param name="qCommand">指令</param>
	/// <param name="qAack">取得數值</param>
	/// <param name="_isDispose"></param>
	static void thread_WorkingLoop(IStDeviceReleasable** obj, int indx, bool* _isCameraOn, bool* _isDispose, queue<string>* qCommand, queue<string>* qAck, mutex* mutex);// int* indx, IStDeviceReleasable** obj, queue<string>* qCommand, queue<string>* qAck, bool* _isCameraOn, bool* _isDispose);
	mutex mu;
	thread thread_Work;

	queue<string> qCommand;
	queue<string> qAck_ReturnValue;

	IStDeviceReleasable* pIStDeviceReleasable;

	bool _IsStreamStart = false;
	bool _isInitialized = false;
	//CNodeMapPtr pINodeMapRemote;

	static void Execute(INodeMap* pINodeMap, const char* szCommandName);
	static void SetEnumeration(INodeMap* pINodeMap, const char* szEnumerationName, const char* szValueName);
	static void GetEnumeration(INodeMap* pINodeMap, const char* szEnumerationName, string& strVal);
	static void GetInteger(INodeMap* pINodeMap, const char* szEnumerationName, string& strVal);

	StateMechine _status;
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
		//for (;;)
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
			//else
			//	break;

			//indx++;
			//break;
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