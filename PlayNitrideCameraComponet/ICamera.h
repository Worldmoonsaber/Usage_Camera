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

	virtual void Grab(unsigned  int*& imgPtr);

	virtual void Grab(void*& imgPtr);


	virtual void SetCameraParam(string NodeName, string Value);
	virtual void GetCameraParam(string NodeName, string& Value);

	virtual void AcquisitionStart();
	virtual void AcquisitionStop();

	string CameraName();
	//-----ヘ玡临⊿Τ稱Τ诀 盢Log癟┻ SubVi 硂场だ痙ゼㄓэ秈 既ぃ秈︽砏购
	virtual void Save();
	virtual void Load();

	//static void UpdateLog(string str);
	//static vector<string> vStringLog;

protected:
	string _strName; //膥┯砆

private:
	int _Width=5320;//代刚ノ计
	int _Height=4600;//代刚ノ计
	int _Channels=4;//代刚ノ计
};

static std::vector<string> _cameraManager_StringLog;
static std::mutex _mtx_Log;

static void _icamera_upDateLog(string str)
{
	std::lock_guard<std::mutex> lock(_mtx_Log);
	_cameraManager_StringLog.push_back(str);

	if (_cameraManager_StringLog.size() > 100)
		_cameraManager_StringLog.erase(_cameraManager_StringLog.begin());

	_mtx_Log.unlock();
}

static void _icamera_getLog(string strLog[])
{
	std::lock_guard<std::mutex> lock(_mtx_Log);

	for (int i = 0; i < _cameraManager_StringLog.size(); i++)
		strLog[i] = _cameraManager_StringLog[i];

	_cameraManager_StringLog.clear();
	_mtx_Log.unlock();

}