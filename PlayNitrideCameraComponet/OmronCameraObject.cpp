#include "OmronCameraObject.h"
#include <StApi_TLFunctions.h>


OmronCameraObject::OmronCameraObject()
{
}

OmronCameraObject::OmronCameraObject(CIStDevicePtr device)
{
//	_device = device;
}

OmronCameraObject::~OmronCameraObject()
{
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
}

void OmronCameraObject::GetCameraParam(string NodeName, string& Value)
{
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