#include "OmronCameraObject.h"
#include <StApi_TLFunctions.h>


OmronCameraObject::OmronCameraObject()
{
}

//OmronCameraObject::OmronCameraObject(CIStDevicePtr device)
//{
////	_device = device;
//}

OmronCameraObject::OmronCameraObject(IStDeviceReleasable* obj,int indx)
{
	pIStDeviceReleasable = obj;
	CIStDevicePtr pIStDevice(pIStDeviceReleasable);
	_strName = pIStDevice->GetIStDeviceInfo()->GetDisplayName();
	uint32_t ps=pIStDevice->GetDataStreamCount();

	if(indx< ps)
		_StreamPtr=pIStDevice->CreateIStDataStream(indx);
}

void OmronCameraObject::Initialize()
{
}

void OmronCameraObject::Close()
{
}

void OmronCameraObject::Grab_Int(unsigned int*& imgPtr)
{
}

void OmronCameraObject::Grab(void*& imgPtr)
{
}

void OmronCameraObject::SetCameraParam(string NodeName, string Value)
{
	if (NodeName == "PixelFormat")
	{

	}
	else if (NodeName == "ExposureTime")
	{

	}

}

void OmronCameraObject::GetCameraParam(string NodeName, string& Value)
{
	if (NodeName == "PixelFormat")
	{

	}
	else if (NodeName == "ExposureTime")
	{

	}
}

void OmronCameraObject::Excute(string ExcuteCmd)
{
}

void OmronCameraObject::AcquisitionStart()
{
}

void OmronCameraObject::AcquisitionStop()
{
}

void OmronCameraObject::Save()
{
}

void OmronCameraObject::Load()
{
}