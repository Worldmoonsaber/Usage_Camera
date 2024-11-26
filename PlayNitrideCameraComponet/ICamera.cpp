#include "ICamera.h"
#include <opencv2/imgcodecs.hpp>

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
	cout << "ICamera Grab" << endl;

	cv::Mat chipImg = cv::imread("C:\\Image\\4X4\\1836\\183601.bmp");

	imgPtr = (unsigned int*)chipImg.data;
	_Width = chipImg.size().width;
	_Height = chipImg.size().height;

}

void ICamera::Grab(unsigned char*& imgPtr)
{
}


void ICamera::SetCameraParam(string NodeName, string Value)
{
	cout << "NodeName "+ NodeName +" Value " + Value  << endl;
}

void ICamera::GetCameraParam(string NodeName, string& Value)
{
	Value = "¦^ÂÐ´ú¸Õ";
	
	if (NodeName == "Width")
		Value = to_string(_Width);
	else if (NodeName == "Height")
		Value = to_string(_Height);

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
	return "NoName";
}
