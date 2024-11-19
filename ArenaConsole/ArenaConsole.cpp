
//#include "ArenaApi.h"
//
//#include "stdafx.h"
#include "opencv2\opencv.hpp"
#include <math.h>
#include <iostream>
#include "conio.h"
#include "ArenaCamera.h"

#include <direct.h> // _getcwd
#include <stdlib.h> // free, perror
#include <stdio.h>  // printf
#include <string.h> // strlen
#include <io.h> // strlen
#include <string> // strlen


using std::cout;
using namespace std;
using namespace Arena;
using namespace cv;
#define TAB "  "


static void thread_WorkContent(void* ptr)
{
	Mat image_input(4600, 5320, CV_8UC3, ptr); // THIS IS THE INPUT IMAGE, POINTER TO DATA			

	resize(image_input, image_input, Size(532, 460));

	imshow("debug", image_input);
}



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
			unsigned int* ptr=nullptr;
			obj->Grab(ptr);

			//thread thr= thread(thread_WorkContent,&ptr);

			//Mat image_input(4600, 5320, CV_8UC3, ptr); // THIS IS THE INPUT IMAGE, POINTER TO DATA			

			//resize(image_input, image_input, Size(532, 460));

			//imshow("debug", image_input);
			//cv:waitKey(0);
		}
	}

	return 0;
}
