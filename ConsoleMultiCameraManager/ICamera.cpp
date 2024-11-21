#include "ICamera.h"

ICamera::ICamera()
{
	//cout << "ICamera Initialize" << endl;

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
}

void ICamera::Grab(unsigned int*& imgPtr)
{
}

void ICamera::Grab(unsigned char*& imgPtr)
{
}

void ICamera::SetCameraParam(string NodeName, string Value)
{
}

void ICamera::GetCameraParam(string NodeName, string& Value)
{
}

void ICamera::AcquisitionStart()
{
}

void ICamera::AcquisitionStop()
{
}



string ICamera::CameraName()
{
	return _strName;
}
