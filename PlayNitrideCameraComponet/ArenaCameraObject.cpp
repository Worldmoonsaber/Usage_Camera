#include "ArenaCameraObject.h"
#include <direct.h>
#include <stdlib.h> // free, perror
#include <stdio.h>  // printf
#include <string.h> // strlen
#include <io.h> // strlen
#include <string> // strlen
#include <chrono>
#include "MultiCameraManager.h"

//void WriteLog(const std::string& message) {
//
//	SYSTEMTIME st;
//	GetLocalTime(&st);
//
//	std::ofstream logFile("Log\\camera_manager_log_" + to_string(st.wMonth) + "-" + to_string(st.wDay) + ".txt", std::ios::app); // 以追加模式打開文件
//	if (logFile.is_open())
//	{
//		string str = to_string(st.wMonth) + "-" + to_string(st.wDay) + " " + to_string(st.wHour) + ":" + to_string(st.wMinute) + ":" + to_string(st.wSecond)+ ":" + to_string(st.wMilliseconds);
//		logFile << str << " " << message << std::endl; // 寫入日誌內容並換行
//	}
//	else 
//	{
//		std::cerr << "Unable to open log file." << std::endl; // 文件打開失敗時打印錯誤
//	}
//}


// Arena::ISystem* pSystem 同時只能存在一個
ArenaCameraObject::ArenaCameraObject(Arena::ISystem* pSystem, Arena::DeviceInfo device)
{
	_deviceInfo = device;

	_pSystem = pSystem;
	_Device = _pSystem->CreateDevice(_deviceInfo);
	_IsStreamStart = false;

	try
	{
		//----目前還沒有有效的方法判斷是否Stream已經被開啟 只能先用此方式
		_Device->StopStream();
		//----如果已經 StartStream 再呼叫一次 相機會掛掉
		//		StopStream 再呼叫一次只會出現例外
	}
	catch (exception ex)
	{
		cout << ex.what() << endl;
		std::string strGe(ex.what());
		WriteLog(strGe);
	}
	catch (GenICam::GenericException& ge)
	{
		cout << ge.what() << endl;
		std::string strGe(ge.what());
		WriteLog(strGe);
	}

	_strName = _deviceInfo.ModelName() + " " + _deviceInfo.SerialNumber();

	string str = "Find Camera : " + _strName + "  IP ADDRESS:"+ string(_deviceInfo.IpAddressStr());

	std::cout << "\n Find Camera : " << _strName << " IP ADDRESS: " + _deviceInfo.IpAddressStr() << "\n";
	WriteLog(str);

	_LoadConfig();
}

ArenaCameraObject::~ArenaCameraObject()
{
}

void ArenaCameraObject::Initialize()
{
}

void ArenaCameraObject::Close()
{
	if (_IsStreamStart)
	{
		_Device->StopStream();
		_IsStreamStart = false;
	}

	_pSystem->DestroyDevice(_Device);
}

void ArenaCameraObject::Grab_Int(unsigned int*& imgPtr)
{
	try
	{
		//-----設定觸發模式
		string Value;

		auto t_start = std::chrono::high_resolution_clock::now();

		bool triggerArmed = false;

		_GetImgPtr(imgPtr); //因為現在是Free Run所以不需要 trigger

		auto t_end = std::chrono::high_resolution_clock::now();
		double elapsed_time_ms = std::chrono::duration<double, std::milli>(t_end - t_start).count();

		//cout << "取像花費時間: " << elapsed_time_ms << " ms" << endl;
	}
	catch (exception ex)
	{
		cout << ex.what() << endl;
		std::string strGe(ex.what());
		WriteLog(strGe);
	}
	catch (GenICam::GenericException& ge)
	{
		cout << ge.what() << endl;
		std::string strGe(ge.what());
		WriteLog(strGe);
	}

}

void ArenaCameraObject::Grab(void*& imgPtr)
{

	try
	{
		//-----設定觸發模式
		string Value;

		auto t_start = std::chrono::high_resolution_clock::now();

		bool triggerArmed = false;

		_GetImgPtr(imgPtr); //因為現在是Free Run所以不需要 trigger

		auto t_end = std::chrono::high_resolution_clock::now();
		double elapsed_time_ms = std::chrono::duration<double, std::milli>(t_end - t_start).count();

		//cout << "取像花費時間: " << elapsed_time_ms << " ms" << endl;
	}
	catch (exception ex)
	{
		cout << ex.what() << endl;
		std::string strGe(ex.what());
		WriteLog(strGe);
	}
	catch (GenICam::GenericException& ge)
	{
		cout << ge.what() << endl;
		std::string strGe(ge.what());
		WriteLog(strGe);
	}


}

void ArenaCameraObject::SetCameraParam(string NodeName, string Value)
{
	try
	{
		if (_IsSpecialCommand(NodeName, Value))
			return;


		string strVal;
		GetCameraParam(NodeName, strVal);

		if (strVal == Value)
		{
			_UpdateMap(NodeName, Value);
			return;
		}

		//-----判斷是否為特殊字

		if (_isNumeric(Value))
		{

			//---------------------------------之後再優化
			double val = atof(Value.c_str());
			double dVal = atof(strVal.c_str());
			double diff = abs(val - dVal);

			if (diff < 0.01) //視為相同
			{
				_UpdateMap(NodeName, Value);
				return;
			}

			if (NodeName == "ExposureTime" && diff < 10)
			{
				_UpdateMap(NodeName, Value);
				return;
			}

			//----判斷有效位數 的數字是否相同

			if (_IsCurrentWriteSpecialNode(NodeName, Value))
			{
				_UpdateMap(NodeName, Value);
				return;
			}
		}

		const char* cNodeName = NodeName.c_str();
		const char* cValue = Value.c_str();
		Arena::SetNodeValue<GenICam::gcstring>(_Device->GetNodeMap(), cNodeName, cValue);

		_UpdateMap(NodeName, Value);
	}
	catch (std::exception ex)
	{
		cout << "NodeName: " + NodeName + " Value :" + Value + " " + ex.what() << endl;
		string log = "NodeName: " + NodeName + " Value :" + Value + " " + ex.what();
		WriteLog(log);

	}
	catch (GenICam::GenericException& ge)
	{
		std::cout << "\n SetCameraParam GenICam exception thrown: " << ge.what() << "NodeName: " + NodeName + " Value : " + Value << "\n";

		std::string strGe(ge.what());
		string log = " SetCameraParam GenICam exception thrown: " + strGe + "NodeName: " + NodeName + " Value : " + Value;
		WriteLog(log);
	}
}

/*
AcquisitionMode	Continuous
AcquisitionStartMode	Normal
AcquisitionFrameCount	1
AcquisitionBurstFrameCount	1

TriggerSelector	FrameStart
TriggerSource	Software
TriggerActivation	RisingEdge
TriggerOverlap	Off
TriggerDelay	0
TriggerArmed
ISPClockSpeed	Normal
ShortExposureEnable	0
ShortIntervalShutterEnable	0
PacketResendWindowFrameCount	4
GevStreamChannelSelector	0
GevSCPSDoNotFragment	1
GevSCPSPacketSize	9000
GevSCPD	9800
TimerSelector	Timer0
PixelFormat	RGB8
ADCBitDepth	Bits8
Width	5320
Height	4600
OffsetX	0
OffsetY	0
MultipleROIRowsEnable	0
ReverseX	0
ReverseY	0
BinningSelector	Digital
GammaEnable	1
Gamma	1
SharpeningEnable	0
DualADCMode	Off
GradationCompressionMode	Off



*/


void ArenaCameraObject::GetCameraParam(string NodeName, string& Value)
{
	if (_IsParamInMap(NodeName, Value))
	{
		//----如果本來就存在那就吐回這些值就好
		return;
	}

	if (_IsSpecialReturnValue(NodeName, Value))
		return;

	try
	{
		if (_IsCurrentReadSpecialNode(NodeName, Value))
		{
			return;
		}

		const char* cNodeName = NodeName.c_str();
		GenICam::gcstring value = Arena::GetNodeValue<GenICam::gcstring>(_Device->GetNodeMap(), cNodeName);
		Value = string(value);
		_UpdateMap(NodeName, Value);
	}
	catch (std::exception ex)
	{
		//cout << "NodeName: " + NodeName + " Value :" + Value + " " + ex.what() << endl;

		string log = "NodeName: " + NodeName + " Value :" + Value + " " + ex.what();
		WriteLog(log);
	}
	catch (GenICam::GenericException& ge)
	{
		//std::cout << "\n GetCameraParam GenICam exception thrown: " << ge.what() << "NodeName: " + NodeName + " Value : " + Value << "\n";

		std::string strGe(ge.what());
		string log = "GetCameraParam GenICam exception thrown: " + strGe+ "NodeName: " + NodeName + " Value : " + Value;
		WriteLog(log);

	}
}

void ArenaCameraObject::_UpdateMap(string NodeName, string Value)
{
	if (count(_ParamKey_OffenRepeatKey.begin(), _ParamKey_OffenRepeatKey.end(), NodeName))
		return;

	if (_mapParam.find(NodeName) == _mapParam.end())
		_mapParam.insert(pair<string, string>(NodeName, Value));
	else
		_mapParam[NodeName] = Value;
}

void ArenaCameraObject::_CreateMap(string NodeName, string Value)
{
	if (count(_ParamKey_OffenRepeatKey.begin(), _ParamKey_OffenRepeatKey.end(), NodeName))
		return;

	if (_mapParam.find(NodeName) == _mapParam.end())
		_mapParam.insert(pair<string, string>(NodeName, Value));
	else
	{
		//-----重複值
		_ParamKey_OffenRepeatKey.push_back(NodeName);
	}



}

bool ArenaCameraObject::_IsParamInMap(string NodeName, string& CurrentValue)
{
	if (count(_ParamKey_OffenRepeatKey.begin(), _ParamKey_OffenRepeatKey.end(), NodeName))
	{
		CurrentValue = "";
		return false;
	}

	if (_mapParam.find(NodeName) == _mapParam.end())
		return false;
	else
		CurrentValue = _mapParam[NodeName];

	return false;
}

void ArenaCameraObject::_SetCameraParamDouble(string NodeName, double Value)
{
	try
	{
		const char* cNodeName = NodeName.c_str();
		Arena::SetNodeValue<double>(_Device->GetNodeMap(), cNodeName, Value);
	}
	catch (GenICam::GenericException& ge)
	{
		std::cout << "\n _SetCameraParamDouble  GenICam exception thrown: " << ge.what() << "NodeName: " + NodeName + " Value : " + to_string(Value) << "\n";

		std::string strGe(ge.what());
		string log = "_SetCameraParamDouble  GenICam exception thrown: " + strGe + "NodeName: " + NodeName + " Value : " + to_string(Value);
		WriteLog(log);

	}
}

void ArenaCameraObject::_GetCameraParamDouble(string NodeName, double& Value)
{
	try
	{
		const char* cNodeName = NodeName.c_str();
		Value = Arena::GetNodeValue<double>(_Device->GetNodeMap(), cNodeName);
	}
	catch (GenICam::GenericException& ge)
	{
		std::cout << "\n  _GetCameraParamDouble GenICam exception thrown: " << ge.what() << "NodeName: " + NodeName + " Value : " + to_string(Value) << "\n";

		std::string strGe(ge.what());
		string log = "_GetCameraParamDouble  GenICam exception thrown: " + strGe + "NodeName: " + NodeName + " Value : " + to_string(Value);
		WriteLog(log);

	}

}

void ArenaCameraObject::_SetCameraParamInt(string NodeName, int Value)
{
	try
	{
		const char* cNodeName = NodeName.c_str();
		Arena::SetNodeValue<int64_t>(_Device->GetNodeMap(), cNodeName, Value);
	}
	catch (GenICam::GenericException& ge)
	{
		std::cout << "\n _SetCameraParamInt  GenICam exception thrown: " << ge.what() << "NodeName: " + NodeName + " Value : " + to_string(Value) << "\n";

		std::string strGe(ge.what());
		string log = "_SetCameraParamInt  GenICam exception thrown: " + strGe + "NodeName: " + NodeName + " Value : " + to_string(Value);
		WriteLog(log);
	}
}

void ArenaCameraObject::_GetCameraParamInt(string NodeName, int& Value)
{
	const char* cNodeName = NodeName.c_str();
	Value = Arena::GetNodeValue<int64_t>(_Device->GetNodeMap(), cNodeName);
}

void ArenaCameraObject::_SetCameraParamBool(string NodeName, bool Value)
{
	try
	{
		const char* cNodeName = NodeName.c_str();
		Arena::SetNodeValue<bool>(_Device->GetNodeMap(), cNodeName, Value);
	}
	catch (GenICam::GenericException& ge)
	{
		std::cout << "\n _SetCameraParamBool  GenICam exception thrown: " << ge.what() << "NodeName: " + NodeName + " Value : " + to_string(Value) << "\n";

		std::string strGe(ge.what());
		string log = "_SetCameraParamBool  GenICam exception thrown: " + strGe + "NodeName: " + NodeName + " Value : " + to_string(Value);
		WriteLog(log);
	}

}

void ArenaCameraObject::_GetCameraParamBool(string NodeName, bool& Value)
{
	const char* cNodeName = NodeName.c_str();
	Value = Arena::GetNodeValue<bool>(_Device->GetNodeMap(), cNodeName);
}

void ArenaCameraObject::Excute(string ExcuteCmd)
{
	try
	{
		const char* cExcuteCmd = ExcuteCmd.c_str();
		Arena::ExecuteNode(_Device->GetNodeMap(), cExcuteCmd);
	}
	catch (std::exception ex)
	{
		cout << "ExcuteCmd: " + ExcuteCmd + " " + ex.what() << endl;

		std::string strGe(ex.what());
		string log = "ExcuteCmd: " + ExcuteCmd + " " + strGe;
		WriteLog(log);
	}
	catch (GenICam::GenericException& ge)
	{
		cout << "ExcuteCmd: " + ExcuteCmd + " " + ge.what() << endl;

		std::string strGe(ge.what());
		string log = "ExcuteCmd: " + ExcuteCmd + " " + strGe;
		WriteLog(log);

	}
}

void ArenaCameraObject::AcquisitionStart()
{
	if (!_IsStreamStart) //已經開始則跳過
	{
		//SetCameraParam("PixelFormat", "BGR8");
		SetCameraParam("AcquisitionMode", "Continuous");

		Arena::SetNodeValue<bool>(_Device->GetTLStreamNodeMap(), "StreamAutoNegotiatePacketSize", true);
		Arena::SetNodeValue<bool>(_Device->GetTLStreamNodeMap(), "StreamPacketResendEnable", true);

		SetCameraParam("AcquisitionStartMode", "Normal");

		Arena::SetNodeValue<GenICam::gcstring>(_Device->GetTLStreamNodeMap(), "StreamBufferHandlingMode", "NewestOnly");

		SetCameraParam("TriggerSelector", "AcquisitionStart");
		SetCameraParam("TriggerSource", "Software");
		SetCameraParam("TriggerMode", "Off");

		_Device->StartStream();
		_IsStreamStart = true;//方案二 加入執行緒連續取像隨時準備吐出來
	}
}
  

void ArenaCameraObject::AcquisitionStop()
{
	if (_IsStreamStart)
	{
		_Device->StopStream();
		_IsStreamStart = false;
	}
}

void ArenaCameraObject::Save()
{
	_SaveConfig();
}

void ArenaCameraObject::Load()
{
	_LoadConfig();
}

bool ArenaCameraObject::_IsCurrentWriteSpecialNode(string NodeName, string Value)
{
	if (std::find(_ParamKey_ValueIsDouble.begin(), _ParamKey_ValueIsDouble.end(), NodeName) != _ParamKey_ValueIsDouble.end())
	{
		double dVal = atof(Value.c_str());
		_SetCameraParamDouble(NodeName, dVal);
		return true;
	}

	if (std::find(_ParamKey_ValueIsInt.begin(), _ParamKey_ValueIsInt.end(), NodeName) != _ParamKey_ValueIsInt.end())
	{
		int nVal = atoi(Value.c_str());
		_SetCameraParamInt(NodeName, nVal);
		return true;
	}

	if (std::find(_ParamKey_ValueIsBool.begin(), _ParamKey_ValueIsBool.end(), NodeName) != _ParamKey_ValueIsBool.end())
	{
		bool bVal = false;

		if (Value == "true" || Value == "1")
			bVal = true;

		_SetCameraParamBool(NodeName, bVal);
		return true;
	}



	return false;
}

bool ArenaCameraObject::_IsCurrentReadSpecialNode(string NodeName, string& Value)
{
	if (std::find(_ParamKey_ValueIsDouble.begin(), _ParamKey_ValueIsDouble.end(), NodeName) != _ParamKey_ValueIsDouble.end())
	{
		double dVal;
		_GetCameraParamDouble(NodeName, dVal);
		Value = to_string(dVal);
		return true;
	}

	if (std::find(_ParamKey_ValueIsInt.begin(), _ParamKey_ValueIsInt.end(), NodeName) != _ParamKey_ValueIsInt.end())
	{
		int nVal;
		_GetCameraParamInt(NodeName, nVal);
		Value = to_string(nVal);
		return true;
	}

	if (std::find(_ParamKey_ValueIsBool.begin(), _ParamKey_ValueIsBool.end(), NodeName) != _ParamKey_ValueIsBool.end())
	{
		bool bVal;
		_GetCameraParamBool(NodeName, bVal);
		Value = to_string(bVal);
		return true;
	}




	return false;
}


void ArenaCameraObject::_GetImgPtr(unsigned int*& imgPtr)
{
	try
	{
		//ArenaCameraObject::_mtx_Grab.try_lock();
		Arena::IImage* image = _Device->GetImage(2000);
		int indx = image->GetFrameId();
		//std::cout << "\n" << "  image->GetFrameId: " << indx << "\n";

		unsigned int retry_count = 0;
		const unsigned int retry_count_max = 10;

		while (image->IsIncomplete())
		{
			retry_count++;
			_Device->RequeueBuffer(image);
			image = _Device->GetImage(100);
			if (retry_count > retry_count_max)
			{
				return;
			}
		}
		//ArenaCameraObject::_mtx_Grab.unlock();
		//std::cout << "Get Img Ptr" << "\n";


		size_t height = image->GetHeight();
		size_t width = image->GetWidth();
		size_t bits_per_pixel = image->GetBitsPerPixel();
		size_t bytes_per_pixel = bits_per_pixel / 8;
		size_t image_data_size_bytes = width * height * bytes_per_pixel;
		//std::cout << "Ptr  Size:" + to_string(image_data_size_bytes) << "\n";

		memcpy(imgPtr, image->GetData(), image_data_size_bytes);
		_Device->RequeueBuffer(image);
		//std::cout << "_Device->RequeueBuffer(image) Succeed." << "\n";

	}
	catch (GenICam::GenericException& ge)
	{
		//ArenaCameraObject::_mtx_Grab.unlock();
		cout << ge.what() << endl;
		std::string strGe(ge.what());
		WriteLog(strGe);
	}
	catch (...)
	{
	}
}

void ArenaCameraObject::_GetImgPtr(void*& imgPtr)
{
	try
	{
		Arena::IImage* image = _Device->GetImage(2000);
		int indx = image->GetFrameId();
		//std::cout << "\n" << "  image->GetFrameId: " << indx << "\n";

		unsigned int retry_count = 0;
		const unsigned int retry_count_max = 10;

		while (image->IsIncomplete())
		{
			retry_count++;
			_Device->RequeueBuffer(image);
			image = _Device->GetImage(100);
			if (retry_count > retry_count_max)
			{
				return;
			}
		}
		//std::cout << "Get Img Ptr" << "\n";

		size_t height = image->GetHeight();
		size_t width = image->GetWidth();
		size_t bits_per_pixel = image->GetBitsPerPixel();
		size_t bytes_per_pixel = bits_per_pixel / 8;
		size_t image_data_size_bytes = width * height * bytes_per_pixel;

		//std::cout << "Ptr  Size:"+ to_string(image_data_size_bytes) << "\n";

		memcpy(imgPtr, image->GetData(), image_data_size_bytes);
		_Device->RequeueBuffer(image);

		//std::cout << "_Device->RequeueBuffer(image) Succeed."<< "\n";
	}
	catch (GenICam::GenericException& ge)
	{
		cout << ge.what() << endl;

		std::string strGe(ge.what());
		WriteLog(strGe);

	}
	catch (...)
	{
	}

}

void ArenaCameraObject::_LoadConfig()
{
	_mapParam.clear();
	//----需要讀取對應文檔進行設定 因為每台相機的功能不一樣 不可以統一設定 這樣會死人的     
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

	if ((_access(strPath.c_str(), 0)) != -1)
	{
		std::ifstream ifs(strPath, std::ios::in);

		std::string s;
		while (std::getline(ifs, s))
		{
			vector<string> vStr = _split(s, ":");

			if (vStr.size() >= 2)
			{
				_CreateMap(vStr[0], vStr[1]);
				SetCameraParam(vStr[0], vStr[1]);
				_vStringPm.push_back(tuple<string, string>(vStr[0], vStr[1]));
			}
		}
		ifs.close();

	}
	else
	{
		//---檔案不存在 建立檔案 並將目前的參數寫入檔案
		_SaveConfig();
	}


}

void ArenaCameraObject::_SaveConfig()
{
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


	fstream write_file;
	write_file.open(strPath, std::ios::out);

	if (write_file.is_open())
	{
	
		string strVal;

		GetCameraParam("ExposureTime", strVal);
		write_file << "ExposuretTime:" + strVal << "\n";

		GetCameraParam("GainRed", strVal);
		write_file << "GainRed:" + strVal << "\n";
		GetCameraParam("GainGreen", strVal);
		write_file << "GainGreen:" + strVal << "\n";
		GetCameraParam("GainBlue", strVal);
		write_file << "GainBlue:" + strVal << "\n";
		GetCameraParam("GainAll", strVal);
		write_file << "GainAll:" + strVal << "\n";
		GetCameraParam("Gamma", strVal);
		write_file << "Gamma:" + strVal << "\n";

		GetCameraParam("Width", strVal);
		write_file << "Width:" + strVal << "\n";

		GetCameraParam("Height", strVal);
		write_file << "Height:" + strVal << "\n";

		GetCameraParam("GevSCPD", strVal);
		write_file << "GevSCPD:" + strVal << "\n";
		GetCameraParam("PixelFormat", strVal);
		write_file << "PixelFormat:" + strVal << "\n";

		GetCameraParam("ADCBitDepth", strVal);
		write_file << "ADCBitDepth:" + strVal << "\n";
		
		GetCameraParam("DeviceStreamChannelPacketSize", strVal);
		write_file << "DeviceStreamChannelPacketSize:" + strVal << "\n";
		
		GetCameraParam("DeviceLinkThroughputReserve", strVal);
		write_file << "DeviceLinkThroughputReserve:" + strVal << "\n";

		GetCameraParam("AcquisitionFrameRate", strVal);
		write_file << "AcquisitionFrameRate:" + strVal << "\n";

		

		write_file.close();

	}
}

bool ArenaCameraObject::_isNumeric(std::string str)
{
	const char* charArr = str.c_str();
	int dotCount = 0;

	for (int i = 0; i < strlen(charArr); i++)
	{
		if (charArr[i] >= '0' && charArr[i] <= '9')
			continue;
		else if (charArr[i] == '.')
		{
			dotCount++;

			if (dotCount > 1)
				return false;
		}
		else
			return false;

	}

	return true;
}

bool ArenaCameraObject::_IsSpecialCommand(string NodeName, string Value)
{
	//if (count(_ParamKey_SpecialKey.begin(), _ParamKey_SpecialKey.end(), NodeName))
	//	return false;

	if (NodeName == "GainRed")
	{
		SetCameraParam("BalanceRatioSelector", "Red");
		SetCameraParam("BalanceRatio", Value);
		return true;
	}
	else if (NodeName == "GainGreen")
	{
		SetCameraParam("BalanceRatioSelector", "Green");
		SetCameraParam("BalanceRatio", Value);
		return true;
	}
	else if (NodeName == "GainBlue")
	{
		SetCameraParam("BalanceRatioSelector", "Blue");
		SetCameraParam("BalanceRatio", Value);
		return true;
	}
	else if (NodeName == "GainAll")
	{
		SetCameraParam("GainSelector", "All");
		SetCameraParam("Gain", Value);
		return true;
	}



	return false;
}

bool ArenaCameraObject::_containsSubstring(const std::string& mainStr, const std::string& subStr) {
	return mainStr.find(subStr) != std::string::npos;
}

bool ArenaCameraObject::_IsSpecialReturnValue(string NodeName, string& Value)
{
	if (NodeName == "GainRed")
	{
		SetCameraParam("BalanceRatioSelector", "Red");
		GetCameraParam("BalanceRatio", Value);
		return true;
	}
	else if (NodeName == "GainGreen")
	{
		SetCameraParam("BalanceRatioSelector", "Green");
		GetCameraParam("BalanceRatio", Value);
		return true;
	}
	else if (NodeName == "GainBlue")
	{
		SetCameraParam("BalanceRatioSelector", "Blue");
		GetCameraParam("BalanceRatio", Value);
		return true;
	}
	else if (NodeName == "Channels")
	{
		GetCameraParam("PixelFormat", Value);

		int val = 0;

		if (Value == "Mono8" || Value == "Mono16" || _containsSubstring(Value,"Bayer"))
			val = 1;
		else
			val = 3;

		Value = to_string(val);
		_UpdateMap(NodeName, Value);
		return true;

	}
	else if (NodeName == "GainAll")
	{
		SetCameraParam("GainSelector", "All");
		GetCameraParam("Gain", Value);
		_UpdateMap(NodeName, Value);
		return true;

	}

	return false;
}

std::vector<std::string> ArenaCameraObject::_split(const std::string& str, const std::string& pattern)
{
	std::vector<std::string> result;
	std::string::size_type begin, end;

	end = str.find(pattern);
	begin = 0;

	while (end != std::string::npos) {
		if (end - begin != 0) {
			result.push_back(str.substr(begin, end - begin));
		}
		begin = end + pattern.size();
		end = str.find(pattern, begin);
	}

	if (begin != str.length()) {
		result.push_back(str.substr(begin));
	}
	return result;
}
