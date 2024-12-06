#pragma once
#include <string>
#include <iostream>
#include<fstream>
#include<cstring>
#include <map>
#include <mutex>
#include <vector>
#include <ctime>
using namespace std;

class ICamera
{

public:
	ICamera();
	~ICamera();

	virtual void Initialize();
	virtual void Close();

	virtual void Grab_Int(unsigned  int*& imgPtr);

	virtual void Grab(void*& imgPtr);


	virtual void SetCameraParam(string NodeName, string Value);
	virtual void GetCameraParam(string NodeName, string& Value);

	virtual void AcquisitionStart();
	virtual void AcquisitionStop();

	virtual void Excute(string strCmd);


	string CameraName();
	//-----�ثe�٨S���Q��ܦ��Ī����� �NLog�T���ߨ� SubVi �o�����d�쥼�ӧ�i �Ȯɤ��i��W��
	virtual void Save();
	virtual void Load();

	//static void UpdateLog(string str);
	//static vector<string> vStringLog;

protected:
	string _strName; //�~�ӫ�i�H�Q�ݨ�

private:
	int _Width=5320;//���եμƭ�
	int _Height=4600;//���եμƭ�
	int _Channels=4;//���եμƭ�
};

static std::vector<string> _cameraManager_StringLog;
static std::mutex _mtx_Log;

static void _icamera_upDateLog(string str)
{
	//��Bug�Ȯɤ��� ���O�d���f
	//std::lock_guard<std::mutex> lock(_mtx_Log);
	//_cameraManager_StringLog.push_back(str);

	//if (_cameraManager_StringLog.size() > 100)
	//	_cameraManager_StringLog.erase(_cameraManager_StringLog.begin());

	//_mtx_Log.unlock();
}

static void _icamera_getLog(string strLog[])
{
	_mtx_Log.lock();

	for (int i = 0; i < _cameraManager_StringLog.size(); i++)
		strLog[i] = _cameraManager_StringLog[i];

	_cameraManager_StringLog.clear();
	_mtx_Log.unlock();

}