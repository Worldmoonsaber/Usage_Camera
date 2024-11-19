
#include "ArenaCamera.h"

#define TAB "  "


ArenaCameraObj::ArenaCameraObj()
{
	isInitialized = false;
	Initialize();
}

ArenaCameraObj::~ArenaCameraObj()
{
}

void ArenaCameraObj::Initialize()
{
	if (isInitialized)
		return;

	try
	{
		pSystem = Arena::OpenSystem();

		pSystem->UpdateDevices(100);
		//更新設備

		deviceInfos = pSystem->GetDevices();

		deviceObj.clear();

		for (int u = 0; u < deviceInfos.size(); u++)
		{
			Arena::IDevice* pDevice = pSystem->CreateDevice(deviceInfos[u]);
			deviceObj.push_back(pDevice);

			std::cout << "\n" << TAB << "Detected device :" << " (" << u << ") "
				<< deviceInfos[u].ModelName() << "\n";
		}
	}
	catch (exception ex)
	{
		std::cout << "\n" << TAB << "Error :" << ex.what()<< "\n";
		throw ex;
	}
	isInitialized = true;
}

std::vector<Arena::DeviceInfo> ArenaCameraObj::DeviceInfos()
{
	return deviceInfos;
}

void ArenaCameraObj::Grab(int cameraId, uint8_t* ImgPtr)
{

}

void ArenaCameraObj::Grab(uint8_t* ImgPtr)
{
	//-----設定觸發模式
    string Value;

	Arena::IDevice* pDevice = deviceObj[_SelectIndx];


    SetCameraParam("PixelFormat", "BGR8");
    SetCameraParam("AcquisitionMode", "SingleFrame");
    SetCameraParam("AcquisitionStartMode", "Normal");
    SetCameraParam("TriggerSelector", "AcquisitionStart");
    SetCameraParam("TriggerMode", "On");
    SetCameraParam("TriggerSource", "Software");

    std::cout << TAB << "Start stream\n";
    pDevice->StartStream();

    Excute("AcquisitionStart");

    bool triggerArmed = false;

    do
    {
        triggerArmed = Arena::GetNodeValue<bool>(pDevice->GetNodeMap(), "TriggerArmed");
    } while (triggerArmed == false);


    Excute("TriggerSoftware");

    Mat img=GetCvImg(pDevice);
    Mat img2;
    resize(img, img2,Size(532,460));

    imshow("debug", img2);
    cv::waitKey(0);
    std::cout << TAB << "Stop stream\n";

    Arena::ExecuteNode(
        pDevice->GetNodeMap(),
        "AcquisitionStop");


    pDevice->StopStream();



}

void ArenaCameraObj::Close()
{
}

void ArenaCameraObj::ConsolePrintDeviceInfo()
{
	std::cout << "\n" << TAB << "Current Devices " << "\n";


	for (int u = 0; u < deviceInfos.size(); u++)
	{
		std::cout << "\n" << TAB  << " (" << u << ") "
			<< deviceInfos[u].ModelName() << "\n";
	}
}

void ArenaCameraObj::SelectCameraId(int cameraId)
{
	_SelectIndx = cameraId;
}

void ArenaCameraObj::ConfigureTriggerAndAcquireImage(Arena::IDevice* pDevice)
{
    // get node values that will be changed in order to return their values at
    // the end of the example
    GenICam::gcstring triggerModeInitial = Arena::GetNodeValue<GenICam::gcstring>(pDevice->GetNodeMap(), "TriggerMode");
    GenICam::gcstring triggerSourceInitial = Arena::GetNodeValue<GenICam::gcstring>(pDevice->GetNodeMap(), "TriggerSource");

    // Set trigger selector
    //    Set the trigger selector to FrameStart. When triggered, the device will
    //    start acquiring a single frame. This can also be set to
    //    AcquisitionStart or FrameBurstStart.
    std::cout << TAB << "Set trigger selector to FrameStart\n";

    Arena::SetNodeValue<GenICam::gcstring>(
        pDevice->GetNodeMap(),
        "TriggerSelector",
        "AcquisitionStart");

    // Set trigger mode
    //    Enable trigger mode before setting the source and selector and before
    //    starting the stream. Trigger mode cannot be turned on and off while the
    //    device is streaming.
    std::cout << TAB << "Enable trigger mode\n";

    Arena::SetNodeValue<GenICam::gcstring>(
        pDevice->GetNodeMap(),
        "TriggerMode",
        "On");

    // Set trigger source
    //    Set the trigger source to software in order to trigger images without
    //    the use of any additional hardware. Lines of the GPIO can also be used
    //    to trigger.
    std::cout << TAB << "Set trigger source to Software\n";

    Arena::SetNodeValue<GenICam::gcstring>(
        pDevice->GetNodeMap(),
        "TriggerSource",
        "Software");

    // enable stream auto negotiate packet size
    Arena::SetNodeValue<bool>(
        pDevice->GetTLStreamNodeMap(),
        "StreamAutoNegotiatePacketSize",
        true);

    // enable stream packet resend
    Arena::SetNodeValue<bool>(
        pDevice->GetTLStreamNodeMap(),
        "StreamPacketResendEnable",
        true);

    // Start stream
    //    When trigger mode is off and the acquisition mode is set to stream
    //    continuously, starting the stream will have the camera begin acquiring
    //    a steady stream of images. However, with trigger mode enabled, the
    //    device will wait for the trigger before acquiring any.
    std::cout << TAB << "Start stream\n";

    pDevice->StartStream();

    // Trigger Armed
    //    Continually check until trigger is armed. Once the trigger is armed, it
    //    is ready to be executed.
    std::cout << TAB << "Wait until trigger is armed\n";
    bool triggerArmed = false;

    do
    {
        triggerArmed = Arena::GetNodeValue<bool>(pDevice->GetNodeMap(), "TriggerArmed");
    } while (triggerArmed == false);

    // Trigger an image
    //    Trigger an image manually, since trigger mode is enabled. This triggers
    //    the camera to acquire a single image. A buffer is then filled and moved
    //    to the output queue, where it will wait to be retrieved.
    std::cout << TAB<< "Trigger image\n";

    Arena::ExecuteNode(
        pDevice->GetNodeMap(),
        "TriggerSoftware");

    // Get image
    //    Once an image has been triggered, it can be retrieved. If no image has
    //    been triggered, trying to retrieve an image will hang for the duration
    //    of the timeout and then throw an exception.
    std::cout << TAB << "Get image";

    Arena::IImage* pImage = pDevice->GetImage(100);


    // requeue buffer
    std::cout << TAB << "Requeue buffer\n";

    pDevice->RequeueBuffer(pImage);

    // Stop the stream
    std::cout << TAB << "Stop stream\n";

    pDevice->StopStream();

    // return nodes to their initial values
    Arena::SetNodeValue<GenICam::gcstring>(pDevice->GetNodeMap(), "TriggerSource", triggerSourceInitial);
    Arena::SetNodeValue<GenICam::gcstring>(pDevice->GetNodeMap(), "TriggerMode", triggerModeInitial);
}

void ArenaCameraObj::SetCameraParam(string NodeName, string Value)
{
    Arena::IDevice* pDevice = deviceObj[_SelectIndx];

    string outputVal;
    GetCameraParam(NodeName, outputVal);

    if (Value != outputVal)
    {
        const char* cNodeName = NodeName.c_str();
        const char* cValue = Value.c_str();
        Arena::SetNodeValue<GenICam::gcstring>(pDevice->GetNodeMap(), cNodeName, cValue);
    }
}

void ArenaCameraObj::GetCameraParam(string NodeName, string &Value)
{
    Arena::IDevice* pDevice = deviceObj[_SelectIndx];

    const char* cNodeName = NodeName.c_str();
    GenICam::gcstring value = Arena::GetNodeValue<GenICam::gcstring>(pDevice->GetNodeMap(), cNodeName);
    Value = string(value);
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
    Arena::IDevice* pDevice = deviceObj[_SelectIndx];

    const char* cExcuteCmd = ExcuteCmd.c_str();

    Arena::ExecuteNode(
        pDevice->GetNodeMap(),
        cExcuteCmd);
}

Mat GetCvImg(Arena::IDevice* pDevice)
{
        Arena::IImage* image = pDevice->GetImage(2000);
    
        unsigned int retry_count = 0;
        const unsigned int retry_count_max = 50;
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
