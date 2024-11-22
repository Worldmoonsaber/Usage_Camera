#include "ArenaCameraObject.h"
#include <direct.h>
#include <stdlib.h> // free, perror
#include <stdio.h>  // printf
#include <string.h> // strlen
#include <io.h> // strlen
#include <string> // strlen
#include <chrono>


// Arena::ISystem* pSystem 同時只能存在一個
ArenaCameraObject::ArenaCameraObject(Arena::ISystem* pSystem,Arena::DeviceInfo device)
{
	_deviceInfo = device;

	_IsStreamStart = false;
	_pSystem = pSystem;
	_Device = _pSystem->CreateDevice(_deviceInfo);
	_strName = _deviceInfo.ModelName() + " " + _deviceInfo.SerialNumber();
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
	_pSystem->DestroyDevice(_Device);

	if (_IsStreamStart)
	{
		_Device->StopStream();
		_IsStreamStart = false;
	}

}

void ArenaCameraObject::Grab(unsigned int*& imgPtr)
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

		cout << "取像花費時間: " << elapsed_time_ms << " ms" << endl;
	}
	catch (exception ex)
	{
		cout << ex.what() << endl;	}
	catch (GenICam::GenericException& ge)
	{
		cout << ge.what() << endl;
	}
}

void ArenaCameraObject::SetCameraParam(string NodeName, string Value)
{
	if (_isNumeric(Value))
	{
		double dVal = atof(Value.c_str());
		SetCameraParam(NodeName, dVal);
		return;
	}

	try
	{
		string outputVal;
		GetCameraParam(NodeName, outputVal);

		if (Value != outputVal)
		{
			const char* cNodeName = NodeName.c_str();
			const char* cValue = Value.c_str();
			Arena::SetNodeValue<GenICam::gcstring>(_Device->GetNodeMap(), cNodeName, cValue);
		}
	}
	catch (std::exception ex)
	{
		cout << "NodeName: " + NodeName + " Value :" + Value + " " + ex.what() << endl;
	}
	catch (GenICam::GenericException& ge)
	{
		std::cout << "\nGenICam exception thrown: " << ge.what() << "NodeName: " + NodeName + " Value : " + Value << "\n";
	}
}

void ArenaCameraObject::GetCameraParam(string NodeName, string& Value)
{
	//---看看map有沒有如果有直接吐map裡面的值 這樣比較有效率


	try
	{
		const char* cNodeName = NodeName.c_str();
		GenICam::gcstring value = Arena::GetNodeValue<GenICam::gcstring>(_Device->GetNodeMap(), cNodeName);
		Value = string(value);
	}
	catch (std::exception ex)
	{
		cout << "NodeName: " + NodeName + " Value :" + Value + " " + ex.what() << endl;
	}
	catch (GenICam::GenericException& ge)
	{
		std::cout << "\nGenICam exception thrown: " << ge.what() << "NodeName: " + NodeName + " Value : " + Value << "\n";
	}
}

void ArenaCameraObject::SetCameraParam(string NodeName, double Value)
{
}

void ArenaCameraObject::GetCameraParam(string NodeName, double& Value)
{
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
	}
	catch (GenICam::GenericException& ge)
	{
		cout << "ExcuteCmd: " + ExcuteCmd + " " + ge.what() << endl;
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
		_IsStreamStart = true;
	}
}

void ArenaCameraObject::AcquisitionStop()
{
	if (_IsStreamStart)
	{
		//Excute("AcquisitionStop");
		_Device->StopStream();
		_IsStreamStart = false;
	}
}

void ArenaCameraObject::Save()
{
}

bool ArenaCameraObject::_IsNodeValueDouble(string str)
{
	return false;
}

void ArenaCameraObject::_GetImgPtr(unsigned int*& imgPtr)
{
	try
	{
		Arena::IImage* image = _Device->GetImage(2000);
		int indx = image->GetFrameId();
		std::cout << "\n"  << "  image->GetFrameId: " << indx << "\n";

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

		size_t height = image->GetHeight();
		size_t width = image->GetWidth();
		size_t bits_per_pixel = image->GetBitsPerPixel();
		size_t bytes_per_pixel = bits_per_pixel / 8;
		size_t image_data_size_bytes = width * height * bytes_per_pixel;

		memcpy(imgPtr, image->GetData(), image_data_size_bytes);
		_Device->RequeueBuffer(image);
	}
	catch (GenICam::GenericException& ge)
	{
		cout << ge.what() << endl;
	}
	catch (...)
	{
	}
}

void ArenaCameraObject::_LoadConfig()
{
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
		//---檔案存在 讀取檔案
				//---檔案存在 讀取檔案
		//fstream read_file;
		//read_file.open(strPath, ios::in);

		//string data;
		//while (std::getline(read_file, data))
		//{
		//	cout << data << '\n';
		//	vector<std::string> vStr = split(data, ":");
		//	SetCameraParam(vStr[0], vStr[1]);
		//}

		//read_file.close();

		std::ifstream ifs(strPath, std::ios::in);

		std::string s;
		while (std::getline(ifs, s)) 
		{
			vector<string> vStr = _split(s, ":");
		}
		ifs.close();

	}
	else
	{
		//---檔案不存在 建立檔案 並將目前的參數寫入檔案
		fstream write_file;
		write_file.open(strPath, std::ios::out);

		//std::vector<std::string> str;
		//str.push_back("001:xxA\n");
		//str.push_back("002:xxB\n");
		//str.push_back("003:xxC\n");

		//for (auto& s : str)
		//    write_file << s;

		write_file.close();
	}
}

bool ArenaCameraObject::_isNumeric(std::string const& str)
{
	auto it = str.begin();
	while (it != str.end() && std::isdigit(*it)) {
		it++;
	}
	return !str.empty() && it == str.end();
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
