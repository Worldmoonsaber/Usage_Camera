#include "ICamera.h"
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

ICamera::ICamera()
{
}

ICamera::~ICamera()
{
}

void ICamera::Initialize()
{
	cout << "ICamera Initialize" << endl;
}

void ICamera::Close()
{
	cout << "ICamera Close" << endl;
}

void ICamera::Grab(unsigned int*& imgPtr)
{
	cv::Mat chipImg = cv::imread("C:\\Image\\4X4\\1836\\183601.bmp");

	_Width = chipImg.size().width;
	_Height = chipImg.size().height;
	_Channels = 4;

	cv::cvtColor(chipImg, chipImg, cv::COLOR_RGB2RGBA);

	uint8_t* data = new uint8_t[_Width * _Height * 4];

	int count = 0;

	for (int j = 0; j < _Height; j++)
		for (int i = 0; i < _Width; i++)
		{
			cv::Vec4b pixel = chipImg.at<cv::Vec4b>(j, i);

			data[count * 4] = pixel[0];     // B
			data[count * 4 + 1] = pixel[1];   // G
			data[count * 4 + 2] = pixel[2];   // R
			data[count * 4 + 3] = pixel[3]; // A

			count++;
		}

	imgPtr = (unsigned int*)data;
}



void ICamera::Grab(void*& imgPtr)
{
	//目前沒用到
}


void ICamera::SetCameraParam(string NodeName, string Value)
{
	cout << "NodeName "+ NodeName +" Value " + Value  << endl;
}

void ICamera::GetCameraParam(string NodeName, string& Value)
{
	Value = "回覆測試";
	
	if (NodeName == "Width")
		Value = to_string(_Width);
	else if (NodeName == "Height")
		Value = to_string(_Height);
	else if(NodeName=="Channels")
		Value= to_string(_Channels);

}

void ICamera::AcquisitionStart()
{
	cout << "ICamera AcquisitionStart" << endl;
}

void ICamera::AcquisitionStop()
{
	cout << "ICamera AcquisitionStop" << endl;
}


string ICamera::CameraName()
{
	return _strName;
}

void ICamera::Save()
{
}

void ICamera::Load()
{
}
