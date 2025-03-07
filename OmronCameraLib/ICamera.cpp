#include "ICamera.h"

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

void ICamera::Grab_Int(unsigned int*& imgPtr)
{

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

void ICamera::Excute(string strCmd)
{
	cout << "ICamera Excutr "+strCmd << endl;

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
