
#include "ArenaCamera.h"

#include <direct.h> // _getcwd
#include <stdlib.h> // free, perror
#include <stdio.h>  // printf
#include <string.h> // strlen
#include <io.h> // strlen
#include <string> // strlen

#define TAB "  "


ArenaCameraObj::ArenaCameraObj()
{
	_isInitialized = false;
	Initialize();
}

ArenaCameraObj::~ArenaCameraObj()
{
}

void ArenaCameraObj::Initialize()
{
	if (_isInitialized)
		return;

	try
	{
		_pSystem = Arena::OpenSystem();

		_pSystem->UpdateDevices(100);
		//更新設備

		_deviceInfos = _pSystem->GetDevices();
		_deviceObj.clear();
        _IsStreamStart.clear();

		for (int u = 0; u < _deviceInfos.size(); u++)
		{
            _SelectIndx = u;
			Arena::IDevice* pDevice = _pSystem->CreateDevice(_deviceInfos[u]);
			_deviceObj.push_back(pDevice);
            _IsStreamStart.push_back(false);

			std::cout << "\n" << TAB << "Detected device :" << " (" << u << ") "
				<< _deviceInfos[u].ModelName()<<" Serial Number :"<< _deviceInfos[u].SerialNumber() << "\n";

            _LoadConfig(pDevice, _deviceInfos[u]);

		}

	}
	catch (exception ex)
	{
		std::cout << "\n" << TAB << "Error :" << ex.what()<< "\n";
		throw ex;
	}
	_isInitialized = true;
}

std::vector<Arena::DeviceInfo> ArenaCameraObj::DeviceInfos()
{
	return _deviceInfos;
}

void ArenaCameraObj::Grab(int cameraId, unsigned int*& imgPtr)
{
    _SelectIndx = cameraId;
    Grab(imgPtr);


}

void ArenaCameraObj::Grab(unsigned int*& imgPtr)
{
    try
    {
        //-----設定觸發模式
        string Value;

        Arena::IDevice* pDevice = _deviceObj[_SelectIndx];

        auto t_start = std::chrono::high_resolution_clock::now();

        pDevice->StartStream();
        Excute("AcquisitionStart");

        bool triggerArmed = false;

        do
        {
            triggerArmed = Arena::GetNodeValue<bool>(pDevice->GetNodeMap(), "TriggerArmed");
        } while (triggerArmed == false);

        Excute("TriggerSoftware");

        _GetImgPtr(pDevice, imgPtr);


        Excute("AcquisitionStop");
        pDevice->StopStream();

        auto t_end = std::chrono::high_resolution_clock::now();
        double elapsed_time_ms = std::chrono::duration<double, std::milli>(t_end - t_start).count();

        cout << "取像花費時間: " << elapsed_time_ms << " ms" << endl;
        //----目前平均花費時間 500ms 理論上這段程式碼可以優化
    }
    catch (exception ex)
    {
        cout << ex.what() << endl;
    }
}

void ArenaCameraObj::Close()
{
    for (int u = 0; u < _deviceInfos.size(); u++)
    {
        _SelectIndx = u;
        SetCameraParam("PixelFormat", "BayerRGB8");//<---不宜在這邊進行設定 很容易出問題
        SetCameraParam("AcquisitionMode", "Continuous");
        SetCameraParam("AcquisitionStartMode", "Normal");
        SetCameraParam("TriggerSelector", "AcquisitionStart");
        SetCameraParam("TriggerMode", "Off");
        SetCameraParam("TriggerSource", "Software");

        _pSystem->DestroyDevice(_deviceObj[u]);
    }

    Arena::CloseSystem(_pSystem);
    _deviceObj.clear();

}

void ArenaCameraObj::ConsolePrintDeviceInfo()
{
	std::cout << "\n" << TAB << "Current Devices " << "\n";


	for (int u = 0; u < _deviceInfos.size(); u++)
	{
		std::cout << "\n" << TAB  << " (" << u << ") "
			<< _deviceInfos[u].ModelName() << "\n";
	}
}

void ArenaCameraObj::SelectCameraId(int cameraId)
{
    Arena::IDevice* pDevice;

	_SelectIndx = cameraId;
    pDevice = _deviceObj[_SelectIndx];

    SetCameraParam("PixelFormat", "BGR8");
    SetCameraParam("AcquisitionMode", "SingleFrame");
    SetCameraParam("AcquisitionStartMode", "Normal");
    SetCameraParam("TriggerSelector", "AcquisitionStart");
    SetCameraParam("TriggerMode", "On");
    SetCameraParam("TriggerSource", "Software");

}

void ArenaCameraObj::SetCameraParam(string NodeName, string Value)
{
    try
    {
        Arena::IDevice* pDevice = _deviceObj[_SelectIndx];

        string outputVal;
        GetCameraParam(NodeName, outputVal);

        if (Value != outputVal)
        {
            const char* cNodeName = NodeName.c_str();
            const char* cValue = Value.c_str();
            Arena::SetNodeValue<GenICam::gcstring>(pDevice->GetNodeMap(), cNodeName, cValue);
        }
    }
    catch(Exception ex)
    {
        cout << "NodeName: " + NodeName + " Value :" + Value + " " + ex.what() << endl;
    }
}

void ArenaCameraObj::GetCameraParam(string NodeName, string &Value)
{
    try
    {
        Arena::IDevice* pDevice = _deviceObj[_SelectIndx];
        const char* cNodeName = NodeName.c_str();
        GenICam::gcstring value = Arena::GetNodeValue<GenICam::gcstring>(pDevice->GetNodeMap(), cNodeName);
        Value = string(value);
    }
    catch (Exception ex)
    {
        cout << "NodeName: " + NodeName+" Value :" + Value +" " + ex.what() << endl;
    }
}

void ArenaCameraObj::SetCameraParam(int cameraId, string NodeName, string Value)
{
    _SelectIndx = cameraId;
    SetCameraParam(NodeName,Value);
}

void ArenaCameraObj::GetCameraParam(int cameraId, string NodeName, string& Value)
{
    _SelectIndx = cameraId;
    GetCameraParam(NodeName,Value);
}

void ArenaCameraObj::Excute(string ExcuteCmd)
{
    try
    {
        Arena::IDevice* pDevice = _deviceObj[_SelectIndx];

        const char* cExcuteCmd = ExcuteCmd.c_str();

        Arena::ExecuteNode(pDevice->GetNodeMap(),cExcuteCmd);
    }
    catch (Exception ex)
    {
        cout << "ExcuteCmd: "+ ExcuteCmd+" " + ex.what() << endl;
    }

}

void ArenaCameraObj::_GetImgPtr(Arena::IDevice* pDevice, unsigned int*& imgPtr)
{

    Arena::IImage* image = pDevice->GetImage(2000);

    std::cout << "\n" << TAB << "image->GetFrameId: " << image->GetFrameId() << "\n";

    unsigned int retry_count = 0;
    const unsigned int retry_count_max = 100;
    while (image->IsIncomplete())
    {
        retry_count++;
        pDevice->RequeueBuffer(image);
        image = pDevice->GetImage(2000);
        if (retry_count > retry_count_max)
        {
            return;
        }
    }

    uint64_t pixel_format = image->GetPixelFormat();
    size_t height = image->GetHeight();
    size_t width = image->GetWidth();
    size_t bits_per_pixel = image->GetBitsPerPixel();

    int format = CV_8UC1;

    if (bits_per_pixel == 8)
        format = CV_8UC1;
    else
        format = CV_8UC3;

    size_t bytes_per_pixel = bits_per_pixel / 8;
    size_t image_data_size_bytes = width * height * bytes_per_pixel;

    imgPtr = (unsigned int *)malloc(width * height * 8 * 4);

    memcpy(imgPtr, image->GetData(), image_data_size_bytes);

    return;
}

void ArenaCameraObj::_LoadConfig(Arena::IDevice* pDevice, Arena::DeviceInfo info)
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

    strPath = strPath +string(info.ModelName())+string(info.SerialNumber())+".txt";

    if ((_access(strPath.c_str(), 0)) != -1) 
    {
        //---檔案存在 讀取檔案
                //---檔案存在 讀取檔案
        fstream read_file;
        read_file.open(strPath, ios::in);

        string data;
        while (std::getline(read_file, data))
        {
            cout << data << '\n';
            vector<std::string> vStr = split(data, ":");
            SetCameraParam(vStr[0], vStr[1]);
        }

        read_file.close();
    }
    else
    {
        //---檔案不存在 建立檔案 並將目前的參數寫入檔案

    }

}

Mat GetCvImg(Arena::IDevice* pDevice)
{
        Arena::IImage* image = pDevice->GetImage(2000);
        
        std::cout << "\n" << TAB << "image->GetFrameId: "<< image->GetFrameId() << "\n";


        unsigned int retry_count = 0;
        const unsigned int retry_count_max = 100;
        while (image->IsIncomplete())
        {
            retry_count++;
            pDevice->RequeueBuffer(image);
            image = pDevice->GetImage(2000);
            if (retry_count > retry_count_max)
            {
                throw exception("Cannot get valid image data after %u retries\n", retry_count_max);
            }
        }
   
        uint64_t pixel_format = image->GetPixelFormat();
        size_t height = image->GetHeight();
        size_t width = image->GetWidth();
    
    
        if (pixel_format == GVSP_Mono12Packed || pixel_format == GVSP_BayerBG12Packed ||
            pixel_format == GVSP_BayerGB12Packed || pixel_format == GVSP_BayerGR12Packed ||
            pixel_format == GVSP_BayerRG12Packed)
        {
            Mat cv_image = Unpack12BitImage(image);
    
            pDevice->RequeueBuffer(image);
    
            return cv_image;
        }
    
        size_t bits_per_pixel = image->GetBitsPerPixel();

        int format = CV_8UC1;

        if (bits_per_pixel == 8)
            format = CV_8UC1;
        else if(bits_per_pixel == 16)
            format = CV_16UC1;
        else 
            format = CV_8UC3;


        Mat cv_image(
            static_cast<int>(height), static_cast<int>(width), CV_8UC3, Scalar::all(0));

        size_t bytes_per_pixel = bits_per_pixel / 8;
        size_t image_data_size_bytes = width * height * bytes_per_pixel;
    
        // Copy bytes from Arena image buffer to opencv image buffer
        memcpy(cv_image.data, image->GetData(), image_data_size_bytes);
    
        // free image
        pDevice->RequeueBuffer(image);
    
        return cv_image;
}

Mat Unpack12BitImage(Arena::IImage* image)
{
    size_t buff_size = image->GetSizeOfBuffer();
    size_t width = image->GetWidth();
    size_t height = image->GetHeight();

    size_t n_pixels = width * height;
    size_t expected_12bit_packed_buff_size = n_pixels * 3 / 2;
    if (buff_size < expected_12bit_packed_buff_size)
    {
        throw exception("Error unpacking 12-bit image!");
    }

    cv::Mat image16_bit((int)height, (int)width, CV_16UC1);

    const uint8_t* input_data = image->GetData();
    uint16_t* out_data16 = (uint16_t*)image16_bit.data;

    for (size_t i = 0; i < n_pixels; i += 2)
    {
        // Unpack 3 bytes into two output pixels
        out_data16[i] = ((uint16_t)input_data[0] << 4) + ((uint16_t)input_data[1] & 0x0F);
        out_data16[i + 1] = ((uint16_t)input_data[2] << 4) + (((uint16_t)input_data[1] & 0xF0) >> 4);
        input_data += 3;
    }

    return image16_bit;
}



std::vector<std::string> ArenaCameraObj::split(const std::string& str, const std::string& pattern) {
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