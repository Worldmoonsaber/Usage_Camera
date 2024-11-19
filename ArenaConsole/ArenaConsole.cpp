
//#include "ArenaApi.h"
//
//#include "stdafx.h"
#include "opencv2\opencv.hpp"
#include <math.h>
#include <iostream>
#include "conio.h"
#include "ArenaCamera.h"

using std::cout;
using namespace std;
using namespace Arena;
using namespace cv;
#define TAB "  "




ArenaCameraObj* obj;

int main()//int argc, char** argv
{
	obj = new ArenaCameraObj();
	obj->Initialize();

	//system("PAUSE");
	
	if (obj->DeviceInfos().size() == 0)
	{
		cout << "無法偵測到相機:\n" << endl;
		system("PAUSE");
		return 0;
	}

	while (true)
	{
		//選擇相機
		cout << "請選擇相機編號:\n" << endl;

		int selectIndex = _getch()-48;

		if (selectIndex == -21)
		{
			cout << "取消流程 程式結束 \n" << endl;
			system("PAUSE");
			return 0;
		}
		else if (selectIndex<0 || selectIndex> obj->DeviceInfos().size())
		{
			cout << "不存在的編號 \n" << endl;
			obj->ConsolePrintDeviceInfo();
		}
		else
		{
			obj->SelectCameraId(selectIndex);
			break;
		}
	}


	while (true)
	{
		//選擇相機
		cout << "任意鍵取像 ( Esc 鍵關閉流程)" << endl;

		int selectIndex = _getch() - 48;

		if (selectIndex == -21)
		{
			cout << "取消流程 程式結束 \n" << endl;
			system("PAUSE");
			return 0;
		}
		else 
		{
			uint8_t* ptr=nullptr;
			obj->Grab(ptr);
		}
	}
	//int n;
	//char c;
	//cout << "Press any key:\n";
	//_getch();
	//cout << "You pressed a key.\n";
	//cout << "Press another key:\n";
	//c = _getch();
	//cout << "You pressed " << c << "\n";
	//cout << "Press another key again:\n";
	//n = _getch();
	//cout << "You pressed " << n << "\n";    //ASCII

	//bool exceptionThrown = false;

	//std::cout << "Cpp_ShadingCorrection\n";

	///*try
	//{*/

	//Arena::ISystem* pSystem = Arena::OpenSystem();

	//pSystem->UpdateDevices(100);
	////更新設備
	//std::vector<Arena::DeviceInfo> deviceInfos = pSystem->GetDevices();

	///*選取設備
	// std::cout << "\n"
	//		   << TAB1 << "Only one device detected: "
	//		   << deviceInfos[0].ModelName() << TAB1
	//		   << deviceInfos[0].SerialNumber() << TAB1
	//		   << deviceInfos[0].IpAddressStr() << ".\n";
	//*/

	//std::cout << "\n" << "Start to list cam: " << endl;

	//for (int i = 0; i < deviceInfos.size(); i++)
	//{
	//	std::cout << "\n" << TAB1 << "Detected device :" << " (" << i << ") "
	//		<< deviceInfos[i].ModelName() << "\n";
	//}
	//bool valid = false;
	//int  camindex = 0;





	//std::cout << "Cpp_ShadingCorrection\n";

	///*try
	//{*/

	//Arena::ISystem* pSystem = Arena::OpenSystem();

	//pSystem->UpdateDevices(100);
	////更新設備
	//std::vector<Arena::DeviceInfo> deviceInfos = pSystem->GetDevices();

	///*選取設備
	// std::cout << "\n"
	//		   << TAB1 << "Only one device detected: "
	//		   << deviceInfos[0].ModelName() << TAB1
	//		   << deviceInfos[0].SerialNumber() << TAB1
	//		   << deviceInfos[0].IpAddressStr() << ".\n";
	//*/

	//std::cout << "\n" << "Start to list cam: " << endl;

	//for (int i = 0; i < deviceInfos.size(); i++)
	//{
	//	std::cout << "\n" << TAB1 << "Detected device :" << " (" << i << ") "
	//		<< deviceInfos[i].ModelName() << "\n";
	//}
	//bool valid = false;
	//int  camindex = 0;

	return 0;
}
