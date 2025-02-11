#include "OmronCameraObject.h"
#include <StApi_TLFunctions.h>
#include <StApi_GUIDef.h>
#include <future>
#include <Shlwapi.h>
#include <direct.h>
#include <stdlib.h> // free, perror
#include <stdio.h>  // printf
#include <string.h> // strlen
#include <io.h> // strlen
#include <string> // strlen

using namespace StApi;



OmronCameraObject::OmronCameraObject()
{
}

OmronCameraObject::OmronCameraObject(IStDeviceReleasable* obj)
{

	_device = obj;
	_IsDeviceLost = false;
	_IsAcquisitionRunning = false;

	//規劃後續要用
	//_mapObj.insert("Channels", "1");
	//_mapObj.insert("GainRed", "1");
	//_mapObj.insert("GainGreen", "1");
	//_mapObj.insert("GainBlue", "1");
	//_mapObj.insert("GainAll", "1");
	//_mapObj.insert("Gamma", "1");
	//_mapObj.insert("PixelFormat", "Mono8");
	//_mapObj.insert("ExposureTime", "1");


	try
	{
		//Open the device.
		_pIStDevice.Reset(_device);

		if (_pIStDevice == NULL)
		{
			throw exception("Device is Null");
		}
		else
		{

			_strName = _pIStDevice->GetIStDeviceInfo()->GetDisplayName();
			_pIStDataStream.Reset(_pIStDevice->CreateIStDataStream(0));
			std::cout << "pIStDevice->GetIStDeviceInfo()->GetDisplayName() " << _pIStDevice->GetIStDeviceInfo()->GetDisplayName() << endl;


			Load();

			std::cout << "Initial Finish " << endl;
		}

	}
	catch (const GenICam::GenericException& e)
	{
		m_pIStRegisteredCallbackDeviceLost.Reset(NULL);
		_pIStDataStream.Reset(NULL);
		_pIStDevice.Reset(NULL);
	}
}


void OmronCameraObject::Initialize()
{

}

void OmronCameraObject::Close()
{
	_AcquisitionStop();
}

void OmronCameraObject::Grab_Int(unsigned int*& imgPtr)
{
	try
	{
		//-----設定觸發模式
		auto t_start = std::chrono::high_resolution_clock::now();

		_GetImgPtr(imgPtr); //因為現在是Free Run所以不需要 trigger

		auto t_end = std::chrono::high_resolution_clock::now();
		double elapsed_time_ms = std::chrono::duration<double, std::milli>(t_end - t_start).count();

	}
	catch (exception ex)
	{
		cout << ex.what() << endl;
		std::string strGe(ex.what());
	}
	catch (GenICam::GenericException& ge)
	{
		cout << ge.what() << endl;
		std::string strGe(ge.what());
	}
}

void OmronCameraObject::Grab(void*& imgPtr)
{
	try
	{
		//-----設定觸發模式
		auto t_start = std::chrono::high_resolution_clock::now();

		bool triggerArmed = false;

		_GetImgPtr(imgPtr); //因為現在是Free Run所以不需要 trigger

		auto t_end = std::chrono::high_resolution_clock::now();
		double elapsed_time_ms = std::chrono::duration<double, std::milli>(t_end - t_start).count();

		cout << "取像花費時間: " << elapsed_time_ms << " ms" << endl;
	}
	catch (exception ex)
	{
		cout << ex.what() << endl;
		std::string strGe(ex.what());
	}
	catch (GenICam::GenericException& ge)
	{
		cout << ge.what() << endl;
		std::string strGe(ge.what());
	}

}

void OmronCameraObject::SetCameraParam(string NodeName, string Value)
{
	INodeMap* NodeMap = _pIStDevice->GetRemoteIStPort()->GetINodeMap();

	if (std::find(_ParamKey_ValueIsEnum.begin(), _ParamKey_ValueIsEnum.end(), NodeName) != _ParamKey_ValueIsEnum.end())
		SetEnumeration(NodeName.c_str(), Value.c_str());
	else if (std::find(_ParamKey_ValueIsInt.begin(), _ParamKey_ValueIsInt.end(), NodeName) != _ParamKey_ValueIsInt.end())
		SetInteger(NodeName.c_str(), Value.c_str());
	else if (std::find(_ParamKey_ValueIsFloat.begin(), _ParamKey_ValueIsFloat.end(), NodeName) != _ParamKey_ValueIsFloat.end())
		SetFloat(NodeName.c_str(), Value.c_str());
	else if (std::find(_ParamKey_SpecialKey.begin(), _ParamKey_SpecialKey.end(), NodeName) != _ParamKey_SpecialKey.end())
		SetSpecial(NodeName.c_str(), Value.c_str());

}


// 測試OK
void OmronCameraObject::GetCameraParam(string NodeName, string& Value)
{
	Value = "";
	INodeMap* NodeMap = _pIStDevice->GetRemoteIStPort()->GetINodeMap();

	if (std::find(_ParamKey_ValueIsEnum.begin(), _ParamKey_ValueIsEnum.end(), NodeName) != _ParamKey_ValueIsEnum.end())
		GetEnumeration(NodeName.c_str(), Value);
	else if (std::find(_ParamKey_ValueIsInt.begin(), _ParamKey_ValueIsInt.end(), NodeName) != _ParamKey_ValueIsInt.end())
		GetInteger(NodeName.c_str(), Value);
	else if (std::find(_ParamKey_ValueIsFloat.begin(), _ParamKey_ValueIsFloat.end(), NodeName) != _ParamKey_ValueIsFloat.end())
		GetFloat(NodeName.c_str(), Value);
	else if (std::find(_ParamKey_SpecialKey.begin(), _ParamKey_SpecialKey.end(), NodeName) != _ParamKey_SpecialKey.end())
		GetSpecial(NodeName.c_str(), Value);
}

void OmronCameraObject::Excute(string ExcuteCmd)
{

}

void OmronCameraObject::AcquisitionStart()
{
	_AcquisitionStart();
}

void OmronCameraObject::AcquisitionStop()
{
	_AcquisitionStop();
}

void OmronCameraObject::Save()
{
	mLoadSaveNodeMapSettingFile(false);
}

void OmronCameraObject::Load()
{
	mLoadSaveNodeMapSettingFile(true);
}

bool OmronCameraObject::_containsSubstring(const std::string& mainStr, const std::string& subStr)
{
	return mainStr.find(subStr) != std::string::npos;
}

void OmronCameraObject::_AcquisitionStart()
{
	_AcquisitionStop();

	try
	{

		_pIStDataStream->StartAcquisition();
		_IsAcquisitionRunning = true;

		if (_pIStDevice->GetRemoteIStPort()->GetIStPortInfo()->IsAccessWrite())
		{
			_pIStDevice->AcquisitionStart();
		}

		cout << "_AcquisitionStart Succeed!!! " << endl;
	}
	catch (const GenICam::GenericException& e)
	{
		cerr << endl << "_AcquisitionStart  exception occurred." << endl << e.GetDescription() << endl;
	}
}

void OmronCameraObject::_AcquisitionStop()
{
	if (!_IsAcquisitionRunning)
		return;

	if (_pIStDevice == NULL) return;

	try
	{
		if (!_IsDeviceLost)
		{
			if (_pIStDevice->GetRemoteIStPort()->GetIStPortInfo()->IsAccessWrite())
			{
				_pIStDevice->AcquisitionStop();
			}
		}

		_pIStDataStream->StopAcquisition();
		_IsAcquisitionRunning = false;

		cout << "_AcquisitionStop Succeed!!! " << endl;

	}
	catch (const GenICam::GenericException& e)
	{
		cerr << endl << "_AcquisitionStop exception occurred." << endl << e.GetDescription() << endl;
	}
}



void OmronCameraObject::Execute(INodeMap* pINodeMap, const char* szCommandName)
{
	CCommandPtr pICommand(pINodeMap->GetNode(szCommandName));
	pICommand->Execute();
}

#pragma region 照抄Omron 範例部分

/// <summary>
/// 照抄範例程式碼
/// </summary>
/// <param name="pINode"></param>
/// <param name=""></param>
void OmronCameraObject::OnDeviceLost(GenApi::INode* pINode, void*)
{
	if (GenApi::IsAvailable(pINode))
	{
		if (_pIStDevice->IsDeviceLost())
		{
			_IsDeviceLost = true;
			//-----未來想到怎麼修改再加 目前保持空白
		}
	}
}


void OmronCameraObject::SetEnumeration(const char* szEnumerationName, const char* szValueName)
{
	try
	{
		INodeMap* pINodeMap = _pIStDevice->GetRemoteIStPort()->GetINodeMap();

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
		cerr << endl << "SetEnumeration Exception :" << endl << e.GetDescription() << endl;
	}
}

void OmronCameraObject::GetEnumeration(const char* szEnumerationName, string& strVal)
{
	try
	{
		INodeMap* pINodeMap = _pIStDevice->GetRemoteIStPort()->GetINodeMap();
		CEnumerationPtr pIEnumeration(pINodeMap->GetNode(szEnumerationName));
		strVal = pIEnumeration->GetCurrentEntry()->GetSymbolic();
	}
	catch (const GenICam::GenericException& e)
	{
		cerr << endl << "GetEnumeration Exception :" << endl << e.GetDescription() << endl;
	}
}

void OmronCameraObject::SetInteger(const char* szIntName, const char* szValueName)
{
	try
	{
		INodeMap* pINodeMap = _pIStDevice->GetRemoteIStPort()->GetINodeMap();
		GenApi::CNodePtr pINode(pINodeMap->GetNode(szIntName));
		GenApi::CIntegerPtr pIInteger(pINode);
		int64_t value = atoi(szValueName);
		pIInteger->SetValue(value);
	}
	catch (const GenICam::GenericException& e)
	{
		cerr << endl << "SetInteger Exception:" << endl << e.GetDescription() << endl;
	}
}

void OmronCameraObject::GetInteger(const char* szIntName, string& strVal)
{
	try
	{
		INodeMap* pINodeMap = _pIStDevice->GetRemoteIStPort()->GetINodeMap();
		GenApi::CNodePtr pINode(pINodeMap->GetNode(szIntName));
		GenApi::CIntegerPtr pIInteger(pINode);
		int64_t nValue = pIInteger->GetValue();
		strVal = to_string(nValue);
	}
	catch (const GenICam::GenericException& e)
	{
		cerr << endl << "GetInteger Exception:" << endl << e.GetDescription() << endl;
	}
}

void OmronCameraObject::SetFloat(const char* szFloatName, const char* szValueName)
{
	try
	{
		INodeMap* pINodeMap = _pIStDevice->GetRemoteIStPort()->GetINodeMap();
		GenApi::CNodePtr pINode(pINodeMap->GetNode(szFloatName));
		CFloatPtr pIFloat(pINodeMap->GetNode(szFloatName));
		double dVal = atof(szValueName);
		pIFloat->SetValue(dVal);
	}
	catch (const GenICam::GenericException& e)
	{
		cerr << endl << "SetFloat Exception:" << endl << e.GetDescription() << endl;
	}
}

void OmronCameraObject::GetFloat(const char* szFloatName, string& strVal)
{
	try
	{
		INodeMap* pINodeMap = _pIStDevice->GetRemoteIStPort()->GetINodeMap();
		GenApi::CNodePtr pINode(pINodeMap->GetNode(szFloatName));
		CFloatPtr pIFloat(pINodeMap->GetNode(szFloatName));
		double dVal = pIFloat->GetValue();
		strVal = to_string(dVal);
	}
	catch (const GenICam::GenericException& e)
	{
		cerr << endl << "GetFloat Exception:" << endl << e.GetDescription() << endl;
	}
}

void OmronCameraObject::SetSpecial(const char* szName, const char* szValueName)
{
	std::string strKey(szName);

	std::vector<string> _ParamKey_SpecialKey{ "GainRed","GainGreen","GainBlue","GainAll","Channels" };

}

void OmronCameraObject::GetSpecial(const char* szName, string& strVal)
{
	std::string strKey(szName);

	if (strKey == "Channels")
	{
		string val;
		GetEnumeration("PixelFormat", val);

		if (_containsSubstring(val, "Mono"))
		{
			strVal = "1";
		}
		else
		{
			strVal = "3";

		}
	}
	else if (strKey == "GainAll")
	{
		GetFloat("Gain", strVal);

	}

}

void OmronCameraObject::mLoadSaveNodeMapSettingFile( bool isLoad)
{
	try
	{
		INodeMap* pINodeMap = _pIStDevice->GetRemoteIStPort()->GetINodeMap();

		string strPath;
		char* buffer;

		// Get the current working directory:
		if ((buffer = _getcwd(NULL, 0)) != NULL)
		{
			strPath.assign(buffer, strlen(buffer));
			free(buffer);
		}

		strPath = strPath + "\\config\\";

		if (_access(strPath.c_str(), 0) == -1)
			_mkdir(strPath.c_str());

		strPath = strPath + _strName + ".txt";

		// Set up file name with gcstring.
		GenICam::gcstring gcStrFileName(strPath.c_str());

		// Create a FeatureBag object for acquiring/saving camera settings.
		StApi::CIStFeatureBagPtr pIStFeatureBagPtr(StApi::CreateIStFeatureBag());

		if (isLoad)
		{
			if ((_access(strPath.c_str(), 0)) != -1)
			{
				pIStFeatureBagPtr->StoreFileToBag(gcStrFileName);
				pIStFeatureBagPtr->Load(pINodeMap, true);
				cout << "Load Config Finished. " << endl;

			}
		}
		else
		{
			pIStFeatureBagPtr->StoreNodeMapToBag(pINodeMap);
			pIStFeatureBagPtr->SaveToFile(gcStrFileName);
		}


	}
	catch (const GenICam::GenericException& e)
	{
		cerr << endl << "An exception occurred." << endl << e.GetDescription() << endl;
	}
}

void OmronCameraObject::_GetImgPtr(void*& _imgPtr)
{

	while (_pIStDataStream->IsGrabbing())
	{
		CIStStreamBufferPtr pIStStreamBuffer(_pIStDataStream->RetrieveBuffer(5000));

		if (pIStStreamBuffer->GetIStStreamBufferInfo()->IsImagePresent())
		{
			IStImage* pIStImage = pIStStreamBuffer->GetIStImage();
			_imgPtr = (void*)pIStImage->GetImageBuffer();

			cout << "BlockId=" << pIStStreamBuffer->GetIStStreamBufferInfo()->GetFrameID()
				<< " Size:" << pIStImage->GetImageWidth() << " x " << pIStImage->GetImageHeight()
				<< " First byte =" << (uint32_t) * (uint8_t*)pIStImage->GetImageBuffer() << endl;



			break;
		}
		else
		{
			cout << "Image data does not exist" << endl;
		}
	}
}

void OmronCameraObject::_GetImgPtr(unsigned int*& _imgPtr)
{
	while (_pIStDataStream->IsGrabbing())
	{
		CIStStreamBufferPtr pIStStreamBuffer(_pIStDataStream->RetrieveBuffer(5000));

		if (pIStStreamBuffer->GetIStStreamBufferInfo()->IsImagePresent())
		{
			IStImage* pIStImage = pIStStreamBuffer->GetIStImage();
			_imgPtr = (unsigned int*)pIStImage->GetImageBuffer();

			break;
		}
		else
		{
			cout << "Image data does not exist" << endl;
		}
	}
}

#pragma endregion
